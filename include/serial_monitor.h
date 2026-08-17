/**
 * ============================================================
 *  SERIAL MONITOR - Header
 * ============================================================
 *  Provides formatted serial telemetry output via UART for
 *  debugging, monitoring, and demonstration purposes.
 *
 *  Output Format:
 *    [TELEMETRY] Temp: 29.5°C | LDR: 450 (DARK) | Motion: YES
 *                | Light: ON | Fan: OFF | Mode: AUTO | Security: ARMED
 * ============================================================
 */

#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include <Arduino.h>

/**
 * Initialize UART serial communication.
 * Must be called first in setup().
 */
void initSerialMonitor();

/**
 * Print the startup banner with system information.
 * Called once after all subsystems are initialized.
 */
void printStartupBanner();

/**
 * Print formatted telemetry data to Serial Monitor.
 * Call this periodically (every SERIAL_PRINT_INTERVAL ms).
 *
 * @param temperature     Current temperature in °C
 * @param humidity        Current humidity in %
 * @param ldrValue        Raw LDR ADC reading (0-4095)
 * @param isDark          true if room is dark
 * @param motionDetected  true if PIR detects motion
 * @param lightOn         true if light is ON
 * @param fanOn           true if fan is ON
 * @param manualOverride  true if manual mode is active
 * @param securityStatus  Human-readable security state string
 */
void printTelemetry(float temperature, float humidity, int ldrValue, bool isDark,
                    bool motionDetected, bool lightOn, bool fanOn,
                    bool manualOverride, const char* securityStatus);

#endif // SERIAL_MONITOR_H
