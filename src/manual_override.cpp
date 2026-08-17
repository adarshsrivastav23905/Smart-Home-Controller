/**
 * ============================================================
 *  MANUAL OVERRIDE - Implementation
 * ============================================================
 *  Reads four push button switches connected to ESP32 GPIOs
 *  with internal pull-up resistors.
 *
 *  Wiring: GPIO_PIN ──── Button ──── GND
 *  (No external resistor needed — internal pull-up is used)
 *
 *  Switch Logic (Active LOW):
 *    Button NOT pressed → pin reads HIGH (pulled up to 3.3V)
 *    Button PRESSED     → pin reads LOW  (connected to GND)
 *
 *  In Wokwi simulation, these are push buttons that can be
 *  clicked to toggle state. In real hardware, use momentary
 *  push buttons or toggle switches as preferred.
 *
 *  This demonstrates:
 *    - Internal pull-up resistor configuration
 *    - Active LOW switch reading
 *    - Digital GPIO input
 * ============================================================
 */

#include "manual_override.h"
#include "config.h"

// Variables for software debounce
static unsigned long lastChangeTime[4] = {0, 0, 0, 0};
static int lastState[4] = {HIGH, HIGH, HIGH, HIGH};
static int stableState[4] = {HIGH, HIGH, HIGH, HIGH};

/**
 * Configure all four switch pins with internal pull-up resistors.
 *
 * INPUT_PULLUP enables the MCU's internal ~45K pull-up resistor,
 * which keeps the pin HIGH when the button is not pressed.
 * This eliminates the need for external pull-up resistors.
 */
void initManualOverride() {
    pinMode(MANUAL_OVERRIDE_PIN, INPUT_PULLUP);
    pinMode(MANUAL_LIGHT_PIN,    INPUT_PULLUP);
    pinMode(MANUAL_FAN_PIN,      INPUT_PULLUP);
    pinMode(SECURITY_MODE_PIN,   INPUT_PULLUP);
}

/**
 * Read all four manual switches and return their states.
 *
 * Since we use INPUT_PULLUP, the logic is inverted:
 *   digitalRead() == LOW  → button is PRESSED  → feature is ACTIVE
 *   digitalRead() == HIGH → button is RELEASED → feature is INACTIVE
 */
// Arrays to track previous states for falling-edge detection
static int lastStableState[4] = {HIGH, HIGH, HIGH, HIGH};

// Latching boolean states for the 4 features
static bool overrideLatched = false;
static bool lightLatched = false;
static bool fanLatched = false;
static bool securityLatched = false;

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
        // Software debounce logic
        if (currentStates[i] != lastState[i]) {
            lastChangeTime[i] = currentMillis;
        }

        if ((currentMillis - lastChangeTime[i]) >= DEBOUNCE_DELAY) {
            if (currentStates[i] != stableState[i]) {
                stableState[i] = currentStates[i];
            }
        }
        
        lastState[i] = currentStates[i];

        // Edge detection: Toggle on falling edge (HIGH to LOW transition = button pressed)
        if (lastStableState[i] == HIGH && stableState[i] == LOW) {
            if (i == 0) overrideLatched = !overrideLatched;
            if (i == 1) lightLatched = !lightLatched;
            if (i == 2) fanLatched = !fanLatched;
            if (i == 3) securityLatched = !securityLatched;
        }
        
        lastStableState[i] = stableState[i];
    }

    // Return the latched/toggled states instead of the raw momentary switch states
    state.overrideActive = overrideLatched;
    state.manualLightOn  = lightLatched;
    state.manualFanOn    = fanLatched;
    state.securityModeOn = securityLatched;

    return state;
}
