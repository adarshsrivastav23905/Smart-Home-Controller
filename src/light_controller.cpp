/**
 * ============================================================
 *  LIGHT CONTROLLER - Implementation
 * ============================================================
 *  Automatic light control using motion + ambient light.
 *
 *  AUTO MODE logic:
 *    1. Motion detected + room is dark → Light ON, reset timer
 *    2. Motion detected + room is bright → Light stays OFF
 *    3. No motion + light was ON → Start countdown timer
 *    4. Timer expires (MOTION_TIMEOUT) → Light OFF
 *
 *  MANUAL MODE logic:
 *    Light state directly follows the manual light switch.
 *
 *  This demonstrates:
 *    - Digital input reading (PIR)
 *    - Analog threshold comparison (LDR)
 *    - Non-blocking timing with millis()
 *    - State management
 *    - GPIO digital output control
 * ============================================================
 */

#include "light_controller.h"
#include "config.h"

// Module-level state variables (static = file scope only)
static bool lightState = false;           // Current light ON/OFF state
static unsigned long lastMotionTime = 0;  // Timestamp of last motion detection

/**
 * Initialize the light output pin to OFF state.
 */
void initLightController() {
    pinMode(LIGHT_PIN, OUTPUT);
    digitalWrite(LIGHT_PIN, LOW);  // Start with light OFF
    lightState = false;
}

/**
 * Update light state based on current conditions.
 *
 * Priority: Manual Override > Automatic Control
 *
 * The no-motion timeout ensures the light doesn't stay ON
 * indefinitely after someone leaves the room. This saves
 * energy — a key feature of smart home systems.
 */
bool updateLightControl(bool motionDetected, bool isDark,
                        bool manualOverride, bool manualLightOn) {

    if (manualOverride) {
        // ---- MANUAL MODE ----
        // Directly use the manual switch state
        lightState = manualLightOn;

    } else {
        // ---- AUTOMATIC MODE ----

        if (motionDetected && isDark) {
            // Someone is in a dark room → turn light ON
            lightState = true;
            lastMotionTime = millis();  // Reset the no-motion timer

        } else if (motionDetected && !isDark) {
            // Someone is in a bright room → no need for artificial light
            lastMotionTime = millis();  // Reset timer (person is present)
            // Don't change lightState — if light was ON from before, keep it
            // Actually, in bright room we don't need light
            lightState = false;

        } else if (!motionDetected && lightState) {
            // No motion detected but light is still ON
            // Check if the no-motion timeout has elapsed
            if (millis() - lastMotionTime >= MOTION_TIMEOUT) {
                lightState = false;  // Turn OFF after timeout
            }
            // Otherwise, keep light ON (person might still be in room)
        }
        // If no motion and light already OFF → do nothing
    }

    // Apply the decided state to the physical output pin
    digitalWrite(LIGHT_PIN, lightState ? HIGH : LOW);

    return lightState;
}

/**
 * Get the current light state without performing any updates.
 */
bool getLightState() {
    return lightState;
}
