/**
 * ============================================================
 *  SMART HOME CONTROLLER - Standalone Arduino Sketch
 * ============================================================
 *  This is a monolithic single-file version of the Smart Home
 *  Controller for use with Arduino IDE. It contains the same
 *  logic as the modular PlatformIO project, combined into one
 *  .ino file for easy upload.
 *
 *  Features:
 *    - PIR motion-based automatic lighting
 *    - LDR ambient light detection
 *    - DHT22 temperature-based fan control with hysteresis
 *    - 3-state security alarm system
 *    - Manual override via push buttons
 *    - SSD1306 OLED real-time status display
 *    - UART serial telemetry output
 *
 *  Target: ESP32 DevKit V4
 *  Required Libraries (install via Arduino Library Manager):
 *    - DHT sensor library by Adafruit
 *    - Adafruit Unified Sensor
 *    - Adafruit SSD1306
 *    - Adafruit GFX Library
 *
 *  Author: Adarsh Srivastav
 * ============================================================
 */

#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ============================================================
//  PIN DEFINITIONS (ESP32 DevKit V4)
// ============================================================

// Sensor Input Pins
#define PIR_PIN              27   // PIR Motion Sensor (Digital Input)
#define LDR_PIN              34   // LDR Light Sensor  (Analog Input, ADC1_CH6)
#define DHT_PIN               4   // DHT22 Temp/Humidity (One-Wire Data)

// Output Pins
#define LIGHT_PIN            25   // Room Light LED (Digital Output)
#define FAN_PIN              26   // Fan Indicator LED (Digital Output)
#define BUZZER_PIN           14   // Security Buzzer (Digital Output)
#define RED_LED_PIN          12   // Red Alert LED (Digital Output)
#define GREEN_LED_PIN        13   // Green Status LED (Digital Output)

// OLED Display I2C
#define OLED_SDA             21   // I2C Data Line
#define OLED_SCL             22   // I2C Clock Line
#define OLED_WIDTH          128   // Display width in pixels
#define OLED_HEIGHT          64   // Display height in pixels
#define OLED_ADDR          0x3C   // I2C address

// Manual Switch Pins (Active LOW with INPUT_PULLUP)
#define MANUAL_OVERRIDE_PIN  19   // Manual Override Toggle
#define MANUAL_LIGHT_PIN     32   // Manual Light Switch
#define MANUAL_FAN_PIN       33   // Manual Fan Switch
#define SECURITY_MODE_PIN    18   // Security Mode Toggle

// DHT Sensor Type
#define DHT_TYPE           DHT22

// ============================================================
//  THRESHOLDS
// ============================================================
#define LDR_DARK_THRESHOLD  1000  // Below this = dark (ESP32 ADC: 0-4095)
#define TEMP_FAN_ON         30.0  // Fan ON above this (°C)
#define TEMP_FAN_OFF        28.0  // Fan OFF below this (°C) - hysteresis
#define TEMP_ALERT          45.0  // Critical temperature alert (°C)

// ============================================================
//  TIMING CONSTANTS (milliseconds)
// ============================================================
#define MOTION_TIMEOUT       30000  // Light OFF after 30s no motion
#define SENSOR_READ_INTERVAL  2000  // Read sensors every 2s
#define DISPLAY_UPDATE_INTERVAL 500 // Refresh OLED every 500ms
#define SERIAL_PRINT_INTERVAL 3000  // Serial telemetry every 3s
#define BUZZER_BEEP_INTERVAL   500  // Buzzer pulse toggle interval
#define DEBOUNCE_DELAY          50  // Button debounce delay (ms)

// ============================================================
//  DATA STRUCTURES
// ============================================================

struct SensorData {
    bool  motionDetected;
    int   ldrValue;
    bool  isDark;
    float temperature;
    float humidity;
    bool  sensorError;
};

struct ManualState {
    bool overrideActive;
    bool manualLightOn;
    bool manualFanOn;
    bool securityModeOn;
};

enum SecurityState {
    SECURITY_DISABLED,
    SECURITY_ARMED,
    SECURITY_ALERT
};

// ============================================================
//  GLOBAL OBJECTS
// ============================================================
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

// ============================================================
//  STATE VARIABLES
// ============================================================

// Sensor data
SensorData sensorData;
ManualState manualState;

// Light controller state
bool lightState = false;
unsigned long lastMotionTime = 0;

// Fan controller state
bool fanState = false;
bool criticalTemp = false;

// Security state
SecurityState securityState = SECURITY_DISABLED;
unsigned long lastBuzzerToggle = 0;
bool buzzerOn = false;
bool alarmLatched = false;

// Output states
bool lightOn = false;
bool fanOn = false;

// Timing variables
unsigned long lastSensorRead = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastSerialPrint = 0;

// ============================================================
//  SENSOR FUNCTIONS
// ============================================================

void initSensors() {
    pinMode(PIR_PIN, INPUT);
    dht.begin();
}

SensorData readAllSensors() {
    SensorData data;

    data.motionDetected = (digitalRead(PIR_PIN) == HIGH);
    data.ldrValue = analogRead(LDR_PIN);
    data.isDark = (data.ldrValue < LDR_DARK_THRESHOLD);
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();
    data.sensorError = isnan(data.temperature) || isnan(data.humidity);

    if (data.sensorError) {
        data.temperature = 25.0;
        data.humidity = 50.0;
        Serial.println(F("[WARN] DHT22 read failed! Using safe defaults."));
    }

    return data;
}

// ============================================================
//  LIGHT CONTROLLER FUNCTIONS
// ============================================================

void initLightController() {
    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);
    lightState = false;
}

bool updateLightControl(bool motionDetected, bool isDark,
                        bool manualOverride, bool manualLightOn) {
    if (manualOverride) {
        lightState = manualLightOn;
    } else {
        if (motionDetected && isDark) {
            lightState = true;
            lastMotionTime = millis();
        } else if (motionDetected && !isDark) {
            lastMotionTime = millis();
            lightState = false;
        } else if (!motionDetected && lightState) {
            if (millis() - lastMotionTime >= MOTION_TIMEOUT) {
                lightState = false;
            }
        }
    }

    digitalWrite(LIGHT_PIN, lightState ? HIGH : LOW);
    return lightState;
}

// ============================================================
//  FAN CONTROLLER FUNCTIONS
// ============================================================

void initFanController() {
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    fanState = false;
    criticalTemp = false;
}

bool updateFanControl(float temperature, bool manualOverride, bool manualFanOn) {
    criticalTemp = (temperature >= TEMP_ALERT);
    if (manualOverride) {
        fanState = manualFanOn;
    } else {
        if (temperature >= TEMP_FAN_ON) {
            fanState = true;
        } else if (temperature <= TEMP_FAN_OFF) {
            fanState = false;
        }
    }

    digitalWrite(FAN_PIN, fanState ? HIGH : LOW);
    return fanState;
}

// ============================================================
//  SECURITY MANAGER FUNCTIONS
// ============================================================

void initSecurityManager() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);

    securityState = SECURITY_DISABLED;
}

SecurityState updateSecurity(bool securityModeOn, bool motionDetected) {
    if (!securityModeOn) {
        securityState = SECURITY_DISABLED;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        buzzerOn = false;
        alarmLatched = false;
    } else if (securityModeOn && (motionDetected || alarmLatched)) {
        securityState = SECURITY_ALERT;
        alarmLatched = true;
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);

        if (millis() - lastBuzzerToggle >= BUZZER_BEEP_INTERVAL) {
            buzzerOn = !buzzerOn;
            digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
            lastBuzzerToggle = millis();
        }
    } else {
        securityState = SECURITY_ARMED;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        buzzerOn = false;
    }

    return securityState;
}

const char* getSecurityStateString() {
    switch (securityState) {
        case SECURITY_DISABLED: return "DISABLED";
        case SECURITY_ARMED:    return "ARMED";
        case SECURITY_ALERT:    return "!! ALERT !!";
        default:                return "UNKNOWN";
    }
}

// ============================================================
//  MANUAL OVERRIDE FUNCTIONS
// ============================================================

static unsigned long mo_lastChangeTime[4] = {0, 0, 0, 0};
static int mo_lastState[4] = {HIGH, HIGH, HIGH, HIGH};
static int mo_stableState[4] = {HIGH, HIGH, HIGH, HIGH};

void initManualOverride() {
    pinMode(MANUAL_OVERRIDE_PIN, INPUT_PULLUP);
    pinMode(MANUAL_LIGHT_PIN, INPUT_PULLUP);
    pinMode(MANUAL_FAN_PIN, INPUT_PULLUP);
    pinMode(SECURITY_MODE_PIN, INPUT_PULLUP);
}

ManualState readManualSwitches() {
    ManualState state;
    unsigned long currentMillis = millis();

    int currentStates[4] = {
        digitalRead(MANUAL_OVERRIDE_PIN),
        digitalRead(MANUAL_LIGHT_PIN),
        digitalRead(MANUAL_FAN_PIN),
        digitalRead(SECURITY_MODE_PIN)
    };

    for (int i = 0; i < 4; i++) {
        if (currentStates[i] != mo_lastState[i]) {
            mo_lastChangeTime[i] = currentMillis;
        }
        if ((currentMillis - mo_lastChangeTime[i]) >= DEBOUNCE_DELAY) {
            if (currentStates[i] != mo_stableState[i]) {
                mo_stableState[i] = currentStates[i];
            }
        }
        mo_lastState[i] = currentStates[i];
    }

    state.overrideActive = (mo_stableState[0] == LOW);
    state.manualLightOn  = (mo_stableState[1] == LOW);
    state.manualFanOn    = (mo_stableState[2] == LOW);
    state.securityModeOn = (mo_stableState[3] == LOW);
    return state;
}

// ============================================================
//  DISPLAY FUNCTIONS
// ============================================================

void initDisplay() {
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("[ERROR] SSD1306 OLED init failed!"));
        return;
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 0);
    display.println(F("=================="));
    display.setCursor(10, 10);
    display.println(F("  SMART HOME"));
    display.setCursor(10, 20);
    display.println(F("  CONTROLLER"));
    display.setCursor(10, 30);
    display.println(F("=================="));
    display.setCursor(10, 42);
    display.println(F("Initializing..."));
    display.setCursor(10, 54);
    display.println(F("Adarsh Srivastav"));
    display.display();
    delay(2000);
}

void updateDisplay(float temperature, float humidity, bool lightOn, bool fanOn,
                   bool motionDetected, bool isDark, bool manualOverride,
                   const char* securityStatus) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println(F("=== SMART HOME ==="));

    display.setCursor(0, 10);
    display.print(F("Temp:"));
    display.print(temperature, 1);
    display.print(F("C  H:"));
    display.print((int)humidity);
    display.println(F("%"));

    display.setCursor(0, 20);
    display.print(F("Light:"));
    display.print(lightOn ? "ON " : "OFF");
    display.print(F("  Room:"));
    display.println(isDark ? "DARK" : "BRIGHT");

    display.setCursor(0, 30);
    display.print(F("Fan:"));
    display.print(fanOn ? "ON " : "OFF");
    display.print(F("  Motion:"));
    display.println(motionDetected ? "YES" : "NO");

    display.setCursor(0, 40);
    display.print(F("Mode: "));
    display.println(manualOverride ? "MANUAL" : "AUTO");

    display.setCursor(0, 50);
    display.print(F("Security: "));
    display.println(securityStatus);

    display.display();
}

// ============================================================
//  SERIAL TELEMETRY FUNCTIONS
// ============================================================

void printStartupBanner() {
    Serial.println(F(""));
    Serial.println(F("=========================================================="));
    Serial.println(F("  SMART HOME CONTROLLER - EMBEDDED SYSTEM PLATFORM"));
    Serial.println(F("  Architecture: ESP32 DevKit V4 / Arduino Framework"));
    Serial.println(F("  Subsystems: PIR + LDR + DHT22 + OLED + Manual Override"));
    Serial.println(F("  Author: Adarsh Srivastav"));
    Serial.println(F("=========================================================="));
    Serial.println(F("[SYS] System Initialized Successfully."));
    Serial.println(F(""));
}

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

// ============================================================
//  SETUP
// ============================================================
void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    printStartupBanner();

    initSensors();
    Serial.println(F("[INIT] Sensors initialized (PIR, LDR, DHT22)"));

    initLightController();
    Serial.println(F("[INIT] Light controller initialized"));

    initFanController();
    Serial.println(F("[INIT] Fan controller initialized"));

    initSecurityManager();
    Serial.println(F("[INIT] Security manager initialized"));

    initManualOverride();
    Serial.println(F("[INIT] Manual override switches initialized"));

    initDisplay();
    Serial.println(F("[INIT] OLED Display initialized"));

    Serial.println(F("[SYS] All subsystems ready. Entering main loop..."));
    Serial.println(F(""));
}

// ============================================================
//  MAIN LOOP
// ============================================================
void loop() {
    unsigned long currentMillis = millis();

    // Step 1: Read sensors periodically
    if (currentMillis - lastSensorRead >= SENSOR_READ_INTERVAL) {
        lastSensorRead = currentMillis;
        sensorData = readAllSensors();
    }

    // Step 2: Read manual switches every loop
    manualState = readManualSwitches();

    // Step 3: Security system (highest priority)
    SecurityState secState = updateSecurity(
        manualState.securityModeOn,
        sensorData.motionDetected
    );

    // Step 4: Light & Fan control
    if (secState == SECURITY_ALERT) {
        // Security override: light ON, fan OFF
        digitalWrite(LIGHT_PIN, HIGH);
        lightOn = true;
        digitalWrite(FAN_PIN, LOW);
        fanOn = false;
    } else {
        lightOn = updateLightControl(
            sensorData.motionDetected, sensorData.isDark,
            manualState.overrideActive, manualState.manualLightOn
        );
        fanOn = updateFanControl(
            sensorData.temperature,
            manualState.overrideActive, manualState.manualFanOn
        );
    }

    // Step 5: Update OLED display periodically
    if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
        lastDisplayUpdate = currentMillis;
        updateDisplay(sensorData.temperature, sensorData.humidity,
                      lightOn, fanOn, sensorData.motionDetected,
                      sensorData.isDark, manualState.overrideActive,
                      getSecurityStateString());
    }

    // Step 6: Serial telemetry periodically
    if (currentMillis - lastSerialPrint >= SERIAL_PRINT_INTERVAL) {
        lastSerialPrint = currentMillis;
        if (secState == SECURITY_ALERT) {
            Serial.println(F("[ALERT] !!! INTRUDER DETECTED !!! Alarm active."));
        }
        if (criticalTemp) {
            Serial.println(F("[ALERT] CRITICAL TEMPERATURE DETECTED! Temp >= 45°C"));
        }
        printTelemetry(sensorData.temperature, sensorData.humidity,
                       sensorData.ldrValue, sensorData.isDark,
                       sensorData.motionDetected, lightOn, fanOn,
                       manualState.overrideActive, getSecurityStateString());
    }
}
