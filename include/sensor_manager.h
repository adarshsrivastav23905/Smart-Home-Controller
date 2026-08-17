/**
 * ============================================================
 *  SENSOR MANAGER - Header
 * ============================================================
 *  Manages all sensor inputs: PIR motion sensor, LDR light
 *  sensor, and DHT22 temperature/humidity sensor.
 *
 *  Provides a unified SensorData structure containing all
 *  sensor readings for use by the control modules.
 * ============================================================
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <DHT.h>

/**
 * Structure to hold all sensor readings in one place.
 * This is populated by readAllSensors() and consumed by
 * the light, fan, security, and display modules.
 */
struct SensorData {
    bool  motionDetected;   // PIR: true if human motion detected
    int   ldrValue;         // LDR: raw ADC value (0-4095 on ESP32)
    bool  isDark;           // Derived: true if ldrValue < threshold
    float temperature;      // DHT22: temperature in degrees Celsius
    float humidity;         // DHT22: relative humidity in percent
    bool  sensorError;      // true if DHT22 read failed (NaN values)
};

/**
 * Initialize all sensor pins and the DHT22 library.
 * Must be called once in setup().
 */
void initSensors();

/**
 * Read all sensors and return a populated SensorData struct.
 * If DHT22 fails, safe default values are used (25°C, 50%).
 */
SensorData readAllSensors();

#endif // SENSOR_MANAGER_H
