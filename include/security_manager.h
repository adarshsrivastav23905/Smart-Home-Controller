/**
 * ============================================================
 *  SECURITY MANAGER - Header
 * ============================================================
 *  Security alert system with three operational states:
 *    DISABLED - Security mode OFF, normal operation
 *    ARMED    - Security mode ON, monitoring for motion
 *    ALERT    - Intrusion detected! Buzzer + Red LED active
 *
 *  Security has the HIGHEST priority in the system.
 *  When an alert is triggered, it overrides light/fan control.
 * ============================================================
 */

#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include <Arduino.h>

/**
 * Security system operational states.
 */
enum SecurityState {
    SECURITY_DISABLED,  // Security mode is OFF
    SECURITY_ARMED,     // Security mode ON, no intrusion
    SECURITY_ALERT      // INTRUDER DETECTED - alarm active
};

/**
 * Initialize security output pins (buzzer, red LED, green LED).
 * Must be called once in setup().
 */
void initSecurityManager();

/**
 * Update security state based on mode switch and motion.
 *
 * @param securityModeOn  true if security mode switch is active
 * @param motionDetected  true if PIR detects motion
 * @return current SecurityState enum value
 */
SecurityState updateSecurity(bool securityModeOn, bool motionDetected);

/**
 * Get the current security state without updating.
 * @return current SecurityState enum value
 */
SecurityState getSecurityState();

/**
 * Get human-readable security state string for display.
 * @return "DISABLED", "ARMED", or "!! ALERT !!"
 */
const char* getSecurityStateString();

#endif // SECURITY_MANAGER_H
