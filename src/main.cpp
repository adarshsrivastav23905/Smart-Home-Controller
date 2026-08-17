/**
 * ============================================================
 *  SMART HOME CONTROLLER - Main Application
 * ============================================================
 *  Central orchestrator that integrates all subsystem modules:
 *    - Sensor Manager    (PIR, LDR, DHT22)
 *    - Light Controller  (Motion + Darkness auto-light)
 *    - Fan Controller    (Temperature-based with hysteresis)
 *    - Security Manager  (3-state alarm system)
 *    - Manual Override   (Push button manual control)
 *    - Display Manager   (OLED status display)
 *    - Serial Monitor    (UART telemetry output)
 *
 *  Priority Hierarchy:
 *    1. SECURITY (highest) - Overrides everything during alert
 *    2. MANUAL OVERRIDE    - User has direct control
 *    3. AUTOMATIC          - Sensor-based automation
 *
 *  Timing Architecture:
 *    All operations use non-blocking millis() timing.
 *    No delay() calls in the main loop, ensuring responsive
 *    switch reading and real-time security monitoring.
 *
 *  Target: ESP32 DevKit V4 | Framework: Arduino
 *  Author: Adarsh Srivastav
 * ============================================================
 */

#include <Arduino.h>
#include "config.h"
#include "sensor_manager.h"
#include "light_controller.h"
#include "fan_controller.h"
#include "security_manager.h"
#include "manual_override.h"
#include "display_manager.h"
#include "serial_monitor.h"

// ============================================================
//  TIMING VARIABLES (non-blocking cooperative scheduling)
// ============================================================
unsigned long lastSensorRead    = 0;  // Last sensor read timestamp
unsigned long lastDisplayUpdate = 0;  // Last OLED refresh timestamp
unsigned long lastSerialPrint   = 0;  // Last serial output timestamp

// ============================================================
//  SYSTEM STATE VARIABLES
// ============================================================
SensorData  sensorData;   // Latest readings from all sensors
ManualState manualState;   // Current state of all manual switches
bool        lightOn = false;  // Current light ON/OFF state
bool        fanOn   = false;  // Current fan ON/OFF state

// ============================================================
//  SETUP - Runs once at power-on / reset
// ============================================================
void setup() {
    // 1. Initialize serial communication FIRST (for debug output)
    initSerialMonitor();
    printStartupBanner();

    // 2. Initialize all sensor inputs
    initSensors();
    Serial.println(F("[INIT] Sensors initialized (PIR=GPIO27, LDR=GPIO34, DHT22=GPIO4)"));

    // 3. Initialize output controllers
    initLightController();
    Serial.println(F("[INIT] Light controller initialized (LED=GPIO25)"));

    initFanController();
    Serial.println(F("[INIT] Fan controller initialized (LED=GPIO26)"));

    initSecurityManager();
    Serial.println(F("[INIT] Security manager initialized (Buzzer=GPIO14, Red=GPIO12, Green=GPIO13)"));

    // 4. Initialize manual input switches
    initManualOverride();
    Serial.println(F("[INIT] Manual override switches initialized (Override=GPIO19, Light=GPIO32, Fan=GPIO33, Security=GPIO18)"));

    // 5. Initialize OLED display (shows splash screen)
    initDisplay();
    Serial.println(F("[INIT] OLED Display initialized (SDA=GPIO21, SCL=GPIO22, Addr=0x3C)"));

    // 6. System ready
    Serial.println(F(""));
    Serial.println(F("[SYS] *** All subsystems ready. Entering main control loop... ***"));
    Serial.println(F(""));
}

// ============================================================
//  LOOP - Runs repeatedly (cooperative multitasking)
// ============================================================
void loop() {
    unsigned long currentMillis = millis();

    // ================================================================
    //  STEP 1: READ SENSORS (periodic, every SENSOR_READ_INTERVAL ms)
    // ================================================================
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;
        sensorData = readAllSensors();
    }

    // ================================================================
    //  STEP 2: READ MANUAL SWITCHES (every loop for responsiveness)
    // ================================================================
    // Manual switches are read every loop iteration to ensure
    // instant response to user button presses. This is lightweight
    // (just digitalRead calls) so it doesn't impact performance.
    manualState = readManualSwitches();

    // ================================================================
    //  STEP 3: SECURITY SYSTEM (HIGHEST PRIORITY)
    // ================================================================
    // Security is always evaluated first because safety takes
    // precedence over comfort (lights/fan) and user convenience.
    SecurityState secState = updateSecurity(
        manualState.securityModeOn,
        sensorData.motionDetected
    );

    // ================================================================
    //  STEP 4: LIGHT & FAN CONTROL (with priority handling)
    // ================================================================
    if (secState == SECURITY_ALERT) {
        // ---- SECURITY ALERT OVERRIDE ----
        // During an intrusion alert:
        //   - Force light ON to illuminate the intruder area
        //   - Force fan OFF (irrelevant during emergency)
        // This overrides BOTH manual and automatic control
        digitalWrite(LIGHT_PIN, HIGH);
        lightOn = true;
        digitalWrite(FAN_PIN, LOW);
        fanOn = false;

    } else {
        // ---- NORMAL OPERATION ----
        // Priority: Manual Override > Automatic Control
        // (handled inside each controller's update function)

        lightOn = updateLightControl(
            sensorData.motionDetected,
            sensorData.isDark,
            manualState.overrideActive,
            manualState.manualLightOn
        );

        fanOn = updateFanControl(
            sensorData.temperature,
            manualState.overrideActive,
            manualState.manualFanOn
        );
    }

    // ================================================================
    //  STEP 5: UPDATE OLED DISPLAY (periodic)
    // ================================================================
    if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        lastDisplayUpdate = currentMillis;
        updateDisplay(
            sensorData.temperature,
            sensorData.humidity,
            lightOn,
            fanOn,
            sensorData.motionDetected,
            sensorData.isDark,
            manualState.overrideActive,
            getSecurityStateString()
        );
    }

    // ================================================================
    //  STEP 6: PRINT SERIAL TELEMETRY (periodic)
    // ================================================================
    if (currentMillis - lastSerialPrint >= SERIAL_PRINT_INTERVAL) {
        lastSerialPrint = currentMillis;

        // Print alert line during security breach
        if (secState == SECURITY_ALERT) {
            Serial.println(F("[ALERT] !!! INTRUDER DETECTED !!! Security alarm active."));
        }

        if (isCriticalTemp()) {
            Serial.println(F("[ALERT] CRITICAL TEMPERATURE DETECTED! Temp >= 45°C"));
        }

        printTelemetry(
            sensorData.temperature,
            sensorData.humidity,
            sensorData.ldrValue,
            sensorData.isDark,
            sensorData.motionDetected,
            lightOn,
            fanOn,
            manualState.overrideActive,
            getSecurityStateString()
        );
    }
}