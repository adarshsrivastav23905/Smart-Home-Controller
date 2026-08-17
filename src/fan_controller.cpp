/**
 * ============================================================
 *  FAN CONTROLLER - Implementation
 * ============================================================
 *  Temperature-based automatic fan control with hysteresis.
 *
 *  Why Hysteresis?
 *    Without hysteresis, if the threshold is 30°C and the
 *    actual temperature fluctuates between 29.9°C and 30.1°C,
 *    the fan would rapidly switch ON and OFF every few seconds.
 *    This is called "chattering" and can damage relay contacts.
 *
 *    With hysteresis (ON at 30°C, OFF at 28°C), the fan turns
 *    ON at 30°C and only turns OFF when temperature drops to
 *    28°C, creating a stable dead-band of 2°C.
 *
 *  This demonstrates:
 *    - Analog sensor processing
 *    - Threshold-based control with hysteresis
 *    - State machine concepts
 *    - GPIO digital output control
 * ============================================================
 */

#include "fan_controller.h"
#include "config.h"

// Module-level state variable
static bool fanState = false;     // Current fan ON/OFF state
static bool criticalTemp = false; // Tracks if temperature is critically high

/**
 * Initialize the fan output pin to OFF state.
 */
void initFanController() {
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);  // Start with fan OFF
    fanState = false;
    criticalTemp = false;
}

/**
 * Update fan state based on temperature with hysteresis.
 *
 * Priority: Manual Override > Automatic Control
 *
 * Temperature Hysteresis Band:
 *   ┌──────────────────────────────────────────┐
 *   │  Temp ≥ 30°C (TEMP_FAN_ON)  → Fan ON    │
 *   │  Temp ≤ 28°C (TEMP_FAN_OFF) → Fan OFF   │
 *   │  28°C < Temp < 30°C → No change (dead)  │
 *   └──────────────────────────────────────────┘
 */
bool updateFanControl(float temperature, bool manualOverride, bool manualFanOn) {

    criticalTemp = (temperature >= TEMP_ALERT);

    if (manualOverride) {
        // ---- MANUAL MODE ----
        // Directly use the manual switch state
        fanState = manualFanOn;

    } else {
        // ---- AUTOMATIC MODE WITH HYSTERESIS ----

        if (temperature >= TEMP_FAN_ON) {
            // Temperature has risen above the upper threshold
            fanState = true;

        } else if (temperature <= TEMP_FAN_OFF) {
            // Temperature has dropped below the lower threshold
            fanState = false;
        }
        // Between TEMP_FAN_OFF and TEMP_FAN_ON: maintain current state
        // This is the hysteresis dead-band — prevents chattering
    }

    // Apply the decided state to the physical output pin
    digitalWrite(FAN_PIN, fanState ? HIGH : LOW);

    return fanState;
}

/**
 * Get the current fan state without performing any updates.
 */
bool getFanState() {
    return fanState;
}

/**
 * Check if the critical temperature alert is active.
 */
bool isCriticalTemp() {
    return criticalTemp;
}
