/**
 * ============================================================
 *  SMART HOME CONTROLLER - CENTRAL CONFIGURATION
 * ============================================================
 *  All hardware pin assignments, sensor thresholds, and
 *  timing constants are defined here for easy modification.
 *
 *  Target MCU: ESP32 DevKit V4
 *  Framework:  Arduino
 *  Author:     Adarsh Srivastav
 * ============================================================
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
//  SENSOR INPUT PINS
// ============================================================
#define PIR_PIN              27   // PIR Motion Sensor (Digital Input, HIGH = motion)
#define LDR_PIN              34   // LDR Light Sensor  (Analog Input, ADC1_CH6)
#define DHT_PIN               4   // DHT22 Temp/Humidity Sensor (One-Wire Data)

// ============================================================
//  OUTPUT PINS
// ============================================================
#define LIGHT_PIN            25   // Room Light LED      (Digital Output)
#define FAN_PIN              26   // Fan Indicator LED    (Digital Output)
#define BUZZER_PIN           14   // Security Buzzer      (Digital Output)
#define RED_LED_PIN          12   // Red Alert LED        (Digital Output)
#define GREEN_LED_PIN        13   // Green Status LED     (Digital Output)

// ============================================================
//  OLED DISPLAY (I2C - SSD1306 128x64)
// ============================================================
#define OLED_SDA             21   // I2C Data Line
#define OLED_SCL             22   // I2C Clock Line
#define OLED_WIDTH          128   // Display width  (pixels)
#define OLED_HEIGHT          64   // Display height (pixels)
#define OLED_ADDR          0x3C   // I2C slave address

// ============================================================
//  MANUAL SWITCH PINS (Active LOW with INPUT_PULLUP)
//  Press button = connects pin to GND = LOW = active
// ============================================================
#define MANUAL_OVERRIDE_PIN  19   // Manual Override Toggle Switch
#define MANUAL_LIGHT_PIN     32   // Manual Light Control Switch
#define MANUAL_FAN_PIN       33   // Manual Fan Control Switch
#define SECURITY_MODE_PIN    18   // Security Mode Arm/Disarm Switch

// ============================================================
//  DHT SENSOR TYPE
// ============================================================
#define DHT_TYPE           DHT22  // DHT22 (AM2302) - higher accuracy

// ============================================================
//  SENSOR THRESHOLDS
// ============================================================

// LDR Threshold (ESP32 12-bit ADC: 0-4095)
// Wokwi Photoresistor Sensor Module Logic:
// Dark = high LDR resistance = HIGH ADC reading (Voltage rises)
// Bright = low LDR resistance = LOW ADC reading (Voltage drops)
#define LDR_DARK_THRESHOLD  2000  // Above this value = room is dark

// Temperature Thresholds (°C) with Hysteresis
// Hysteresis prevents rapid ON/OFF switching near threshold
#define TEMP_FAN_ON         30.0  // Fan turns ON above this temperature
#define TEMP_FAN_OFF        28.0  // Fan turns OFF below this temperature
#define TEMP_ALERT          45.0  // Critical temperature alert threshold

// ============================================================
//  TIMING CONSTANTS (milliseconds)
//  All timing uses millis() for non-blocking operation
// ============================================================
#define MOTION_TIMEOUT       30000  // Light OFF after 30s with no motion
#define SENSOR_READ_INTERVAL  2000  // Read sensors every 2 seconds
#define DISPLAY_UPDATE_INTERVAL 500 // Refresh OLED every 500ms
#define SERIAL_PRINT_INTERVAL 3000  // Print serial telemetry every 3 seconds
#define BUZZER_BEEP_INTERVAL   500  // Buzzer ON/OFF toggle interval (pulsing)
#define DEBOUNCE_DELAY          50  // Button debounce delay

// ============================================================
//  SERIAL COMMUNICATION
// ============================================================
#define SERIAL_BAUD_RATE   115200  // UART baud rate for Serial Monitor

#endif // CONFIG_H
