/**
 * ============================================================
 *  DISPLAY MANAGER - Header
 * ============================================================
 *  Manages the SSD1306 128x64 OLED display via I2C.
 *  Displays real-time system status including:
 *    - Temperature and humidity
 *    - Light and fan states
 *    - Motion detection status
 *    - Operating mode (AUTO/MANUAL)
 *    - Security system status
 * ============================================================
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>

/**
 * Initialize the OLED display and show boot splash screen.
 * Must be called once in setup() after Wire.begin().
 */
void initDisplay();

/**
 * Update the OLED with current system status.
 * Call this periodically (every DISPLAY_UPDATE_INTERVAL ms).
 *
 * @param temperature     Current temperature in °C
 * @param humidity        Current humidity in %
 * @param lightOn         true if room light is ON
 * @param fanOn           true if fan is ON
 * @param motionDetected  true if PIR detects motion
 * @param isDark          true if room is dark
 * @param manualOverride  true if manual mode is active
 * @param securityStatus  Human-readable security state string
 */
void updateDisplay(float temperature, float humidity, bool lightOn, bool fanOn,
                   bool motionDetected, bool isDark, bool manualOverride,
                   const char* securityStatus);

#endif // DISPLAY_MANAGER_H
