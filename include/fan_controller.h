/**
 * ============================================================
 *  FAN CONTROLLER - Header
 * ============================================================
 *  Temperature-based fan control with hysteresis to prevent
 *  rapid ON/OFF switching near the threshold boundary.
 *
 *  Automation Rule:
 *    IF temperature >= TEMP_FAN_ON  → Fan ON
 *    IF temperature <= TEMP_FAN_OFF → Fan OFF
 *    Between thresholds             → Maintain current state
 *    IF manual override active      → Use manual switch
 * ============================================================
 */

#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <Arduino.h>

/**
 * Initialize the fan output pin.
 * Must be called once in setup().
 */
void initFanController();

/**
 * Update fan state based on temperature and manual override.
 *
 * @param temperature    current temperature in °C from DHT22
 * @param manualOverride true if manual override switch is active
 * @param manualFanOn    true if manual fan switch is pressed
 * @return current fan state (true = ON, false = OFF)
 */
bool updateFanControl(float temperature, bool manualOverride, bool manualFanOn);

/**
 * Get the current fan state without updating.
 * @return true if fan is currently ON
 */
bool getFanState();

/**
 * Check if the critical temperature alert is active.
 * @return true if temperature is >= TEMP_ALERT
 */
bool isCriticalTemp();

#endif // FAN_CONTROLLER_H
