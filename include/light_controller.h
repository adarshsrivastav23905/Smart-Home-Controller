/**
 * ============================================================
 *  LIGHT CONTROLLER - Header
 * ============================================================
 *  Automatic light control based on motion detection and
 *  ambient light level. Supports manual override mode.
 *
 *  Automation Rule:
 *    IF motion detected AND room is dark → Light ON
 *    IF no motion for MOTION_TIMEOUT   → Light OFF
 *    IF manual override active         → Use manual switch
 * ============================================================
 */

#ifndef LIGHT_CONTROLLER_H
#define LIGHT_CONTROLLER_H

#include <Arduino.h>

/**
 * Initialize the light output pin.
 * Must be called once in setup().
 */
void initLightController();

/**
 * Update light state based on sensor inputs and manual override.
 *
 * @param motionDetected  true if PIR sensor detects motion
 * @param isDark          true if room ambient light is below threshold
 * @param manualOverride  true if manual override switch is active
 * @param manualLightOn   true if manual light switch is pressed (only used when override active)
 * @return current light state (true = ON, false = OFF)
 */
bool updateLightControl(bool motionDetected, bool isDark, bool manualOverride, bool manualLightOn);

/**
 * Get the current light state without updating.
 * @return true if light is currently ON
 */
bool getLightState();

#endif // LIGHT_CONTROLLER_H
