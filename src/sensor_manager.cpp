/**
 * ============================================================
 *  SENSOR MANAGER - Implementation
 * ============================================================
 *  Reads PIR motion sensor, LDR light sensor, and DHT22
 *  temperature/humidity sensor. Provides validated data
 *  with safe defaults on sensor failure.
 * ============================================================
 */

#include "sensor_manager.h"
#include "config.h"

// Create DHT sensor object with pin and type from config
static DHT dht(DHT_PIN, DHT_TYPE);

/**
 * Initialize all sensor pins and start the DHT22 library.
 */
void initSensors() {
    // PIR sensor outputs HIGH when motion is detected
    pinMode(PIR_PIN, INPUT);

    // LDR is read via analogRead() - no pinMode needed for ADC on ESP32
    // (GPIO34 is input-only on ESP32, which is ideal for analog reading)

    // Initialize DHT22 sensor communication
    dht.begin();
}

/**
 * Read all sensors and package readings into a SensorData struct.
 *
 * PIR Sensor:
 *   - Digital read: HIGH = motion detected, LOW = no motion
 *   - HC-SR501 has built-in sensitivity and delay potentiometers
 *
 * LDR Sensor:
 *   - Analog read through voltage divider circuit
 *   - ESP32 ADC is 12-bit: values range from 0 to 4095
 *   - Circuit: VCC → LDR → ADC_PIN → 10K_resistor → GND
 *   - Dark room = high LDR resistance = low voltage at ADC = low value
 *   - Bright room = low LDR resistance = high voltage at ADC = high value
 *
 * DHT22 Sensor:
 *   - Reads temperature (°C) and humidity (%) via one-wire protocol
 *   - Returns NaN on communication failure
 *   - We substitute safe defaults (25°C, 50%) on error
 */
SensorData readAllSensors() {
    SensorData data;

    // ---- PIR Motion Sensor ----
    // HC-SR501 outputs HIGH (3.3V) when motion is detected
    data.motionDetected = (digitalRead(PIR_PIN) == HIGH);

    // ---- LDR Light Sensor ----
    // Read the analog voltage at the LDR/resistor junction
    data.ldrValue = analogRead(LDR_PIN);
    // Determine if room is dark based on configured threshold
    // With wokwi-photoresistor-sensor: Voltage is HIGH in the dark, LOW in the light
    data.isDark = (data.ldrValue > LDR_DARK_THRESHOLD);

    // ---- DHT22 Temperature & Humidity ----
    data.temperature = dht.readTemperature();   // Celsius
    data.humidity    = dht.readHumidity();       // Percent

    // ---- Sensor Error Handling ----
    // DHT22 returns NaN (Not a Number) when read fails
    data.sensorError = isnan(data.temperature) || isnan(data.humidity);

    if (data.sensorError) {
        // Use safe default values to prevent erratic behavior
        data.temperature = 25.0;  // Room temperature default
        data.humidity    = 50.0;  // Normal humidity default
        Serial.println(F("[WARN] DHT22 read failed! Using safe defaults (25°C, 50%)"));
    }

    return data;
}
