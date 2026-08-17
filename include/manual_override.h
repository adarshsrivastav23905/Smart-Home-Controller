/**
 * ============================================================
 *  MANUAL OVERRIDE - Header
 * ============================================================
 *  Reads manual push button switches for:
 *    - Manual Override toggle (enables/disables manual mode)
 *    - Manual Light switch (controls light when override is ON)
 *    - Manual Fan switch (controls fan when override is ON)
 *    - Security Mode switch (arms/disarms security system)
 *
 *  All switches use INPUT_PULLUP configuration:
 *    Button pressed  = pin reads LOW  = active
 *    Button released = pin reads HIGH = inactive
 * ============================================================
 */

#ifndef MANUAL_OVERRIDE_H
#define MANUAL_OVERRIDE_H

#include <Arduino.h>

/**
 * Structure holding the state of all manual switches.
 */
struct ManualState {
    bool overrideActive;    // True if manual override mode is enabled
    bool manualLightOn;     // True if manual light switch is pressed
    bool manualFanOn;       // True if manual fan switch is pressed
    bool securityModeOn;    // True if security mode switch is pressed
};

/**
 * Initialize all switch pins with internal pull-up resistors.
 * Must be called once in setup().
 */
void initManualOverride();

/**
 * Read the current state of all manual switches.
 * @return ManualState struct with all switch states
 */
ManualState readManualSwitches();

#endif // MANUAL_OVERRIDE_H
