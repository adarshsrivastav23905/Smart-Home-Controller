/**
 * ============================================================
 *  SERIAL MONITOR - Implementation
 * ============================================================
 *  Provides formatted UART serial telemetry for debugging,
 *  monitoring, and demonstration via Arduino Serial Monitor.
 *
 *  Baud Rate: 115200 (configured in config.h)
 *
 *  Output includes:
 *    - Startup system banner
 *    - Periodic telemetry with all sensor/actuator states
 *    - Warning messages for sensor errors
 *
 *  This demonstrates:
 *    - UART serial communication
 *    - Formatted diagnostic output
 *    - System state telemetry
 * ============================================================
 */

#include "serial_monitor.h"
#include "config.h"

/**
 * Initialize UART serial communication at configured baud rate.
 */
void initSerialMonitor() {
    Serial.begin(SERIAL_BAUD_RATE);
    // Note: Removed the while (!Serial) blocking loop for Wokwi compatibility
    // In real hardware this waits for USB connection, but in simulation it can block
    delay(100);  // Brief delay to allow serial to initialize
}

/**
 * Print the startup banner with system identification.
 * Called once after all subsystems are initialized.
 */
void printStartupBanner() {
    Serial.println(F(""));
    Serial.println(F("=========================================================="));
    Serial.println(F("  SMART HOME CONTROLLER - EMBEDDED SYSTEM PLATFORM"));
    Serial.println(F("  Architecture: ESP32 DevKit V4 / Arduino Framework"));
    Serial.println(F("  Subsystems: PIR + LDR + DHT22 + OLED + Manual Override"));
    Serial.println(F("  Author: Adarsh Srivastav"));
    Serial.println(F("=========================================================="));
    Serial.println(F("[SYS] System Initialized Successfully. Running main loop..."));
    Serial.println(F(""));
}

/**
 * Print formatted telemetry data to the Serial Monitor.
 *
 * Output format:
 *   [TELEMETRY] Temp: 29.5°C | Humidity: 65.0% | LDR: 450 (DARK)
 *               | Motion: YES | Light: ON | Fan: OFF
 *               | Mode: AUTO | Security: ARMED
 *
 * Call frequency: every SERIAL_PRINT_INTERVAL ms (3000ms)
 */
void printTelemetry(float temperature, float humidity, int ldrValue, bool isDark,
                    bool motionDetected, bool lightOn, bool fanOn,
                    bool manualOverride, const char* securityStatus) {

    Serial.print(F("[TELEMETRY] Temp: "));
    Serial.print(temperature, 1);
    Serial.print(F(" C | Humidity: "));
    Serial.print(humidity, 1);
    Serial.print(F("% | LDR: "));
    Serial.print(ldrValue);
    Serial.print(F(" ("));
    Serial.print(isDark ? "DARK" : "BRIGHT");
    Serial.print(F(") | Motion: "));
    Serial.print(motionDetected ? "YES" : "NO");
    Serial.print(F(" | Light: "));
    Serial.print(lightOn ? "ON" : "OFF");
    Serial.print(F(" | Fan: "));
    Serial.print(fanOn ? "ON" : "OFF");
    Serial.print(F(" | Mode: "));
    Serial.print(manualOverride ? "MANUAL" : "AUTO");
    Serial.print(F(" | Security: "));
    Serial.println(securityStatus);
}
