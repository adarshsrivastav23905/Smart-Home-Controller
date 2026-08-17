/**
 * ============================================================
 *  SECURITY MANAGER - Implementation
 * ============================================================
 *  Three-state security system:
 *
 *  State Machine:
 *    DISABLED ──(security switch ON)──► ARMED
 *    ARMED ────(motion detected)──────► ALERT
 *    ALERT ────(security switch OFF)──► DISABLED
 *    ARMED ────(security switch OFF)──► DISABLED
 *
 *  During ALERT state:
 *    - Buzzer pulses ON/OFF (non-blocking using millis())
 *    - Red LED turns ON
 *    - Green LED turns OFF
 *    - Display shows "INTRUDER ALERT"
 *
 *  Security has the HIGHEST PRIORITY in the system.
 *  During an alert, the main loop forces:
 *    - Light ON (to illuminate the intruder area)
 *    - Fan OFF  (not relevant during emergency)
 *
 *  This demonstrates:
 *    - State machine design
 *    - Priority-based control logic
 *    - Non-blocking buzzer pulsing
 *    - Multiple output coordination
 * ============================================================
 */

#include "security_manager.h"
#include "config.h"

// Module-level state variables
static SecurityState currentState = SECURITY_DISABLED;
static unsigned long lastBuzzerToggle = 0;  // For non-blocking buzzer pulsing
static bool buzzerOn = false;               // Current buzzer ON/OFF toggle
static bool alarmLatched = false;           // Tracks if the alarm has been triggered

/**
 * Initialize all security-related output pins.
 * Green LED starts ON (system normal), Red LED and Buzzer start OFF.
 */
void initSecurityManager() {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    // Default state: system normal
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);  // Green ON = system normal

    currentState = SECURITY_DISABLED;
}

/**
 * Update security system state and drive outputs accordingly.
 *
 * The buzzer uses non-blocking pulsing (millis()-based toggle)
 * instead of delay() to avoid blocking the main control loop.
 * This is critical for responsive manual switch reading.
 */
SecurityState updateSecurity(bool securityModeOn, bool motionDetected) {

    if (!securityModeOn) {
        // ---- SECURITY DISABLED ----
        // Turn off all alerts, show green (normal)
        currentState = SECURITY_DISABLED;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        buzzerOn = false;
        alarmLatched = false;

    } else if (securityModeOn && (motionDetected || alarmLatched)) {
        // ---- INTRUDER ALERT! ----
        // Security is armed AND motion is detected (or alarm is latched)
        currentState = SECURITY_ALERT;
        alarmLatched = true;

        // Red LED ON, Green LED OFF
        digitalWrite(RED_LED_PIN, HIGH);
        digitalWrite(GREEN_LED_PIN, LOW);

        // Pulsating buzzer using non-blocking timing
        // The buzzer toggles ON/OFF every BUZZER_BEEP_INTERVAL ms
        // This creates an attention-grabbing pulsing alarm sound
        if (millis() - lastBuzzerToggle >= BUZZER_BEEP_INTERVAL) {
            buzzerOn = !buzzerOn;
            digitalWrite(BUZZER_PIN, buzzerOn ? HIGH : LOW);
            lastBuzzerToggle = millis();
        }

    } else {
        // ---- SECURITY ARMED (no motion) ----
        // Security mode ON but no intrusion detected
        currentState = SECURITY_ARMED;
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);  // Green = all clear
        buzzerOn = false;
    }

    return currentState;
}

/**
 * Get the current security state without updating.
 */
SecurityState getSecurityState() {
    return currentState;
}

/**
 * Get a human-readable string for the current security state.
 * Used by the display and serial modules.
 */
const char* getSecurityStateString() {
    switch (currentState) {
        case SECURITY_DISABLED: return "DISABLED";
        case SECURITY_ARMED:    return "ARMED";
        case SECURITY_ALERT:    return "!! ALERT !!";
        default:                return "UNKNOWN";
    }
}
