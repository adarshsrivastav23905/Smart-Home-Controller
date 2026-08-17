# Test Execution Report
## Smart Home Controller

### Test Environment
- **Platform:** Wokwi Simulator + PlatformIO
- **MCU:** ESP32 DevKit V4 (virtual)
- **Date:** August 2026
- **Tested By:** Adarsh Srivastav

---

### Test Summary

| Category | Tests Planned | Tests Executed | Passed | Failed |
|---|---|---|---|---|
| Sensor Interfacing | 3 | 3 | 3 | 0 |
| Auto Lighting | 4 | 4 | 4 | 0 |
| Climate Control | 3 | 3 | 3 | 0 |
| Security System | 3 | 3 | 3 | 0 |
| Manual Override | 2 | 2 | 2 | 0 |
| UI / Display | 2 | 2 | 2 | 0 |
| **Total** | **17** | **17** | **17** | **0** |

---

### Detailed Test Results

| Test ID | Feature | Action / Scenario | Expected Result | Actual Result | Status |
|---|---|---|---|---|---|
| TC-01 | DHT22 | Change temperature in simulation | Serial logs show new temperature | As expected | PASSED |
| TC-02 | LDR | Change lux in simulation | ADC value changes (0-4095) | As expected | PASSED |
| TC-03 | PIR | Trigger motion | Serial shows "Motion: YES" | As expected | PASSED |
| TC-04 | Lighting | Dark (LDR > 2000) + Motion | Light turns ON | As expected | PASSED |
| TC-05 | Lighting | Bright (LDR < 2000) + Motion | Light stays OFF | As expected | PASSED |
| TC-06 | Lighting | Motion stops | Light stays ON for 30s timeout | As expected | PASSED |
| TC-07 | Lighting | Timeout expires | Light turns OFF | As expected | PASSED |
| TC-08 | Fan Auto | Temp rises >= 30.0°C | Fan turns ON | As expected | PASSED |
| TC-09 | Fan Hys. | Temp drops to 29.0°C | Fan STAYS ON (Hysteresis working) | As expected | PASSED |
| TC-10 | Fan Auto | Temp drops <= 28.0°C | Fan turns OFF | As expected | PASSED |
| TC-11 | Security | Toggle Security button | Mode changes to ARMED, Green LED ON | As expected | PASSED |
| TC-12 | Security | Motion while ARMED | Buzzer + Red LED ON (ALERT) | As expected | PASSED |
| TC-13 | Security | Motion stops while ALERT | Alarm LATCHES (stays ON) | As expected | PASSED |
| TC-14 | Override | Press Light override button | Light turns ON, Mode = MANUAL | As expected | PASSED |
| TC-15 | Override | Press Fan override button | Fan turns ON, Mode = MANUAL | As expected | PASSED |
| TC-16 | OLED | Normal Operation | Displays telemetry on 6 lines | As expected | PASSED |
| TC-17 | OLED | Intruder Alert | Displays "!!! INTRUDER !!!" | As expected | PASSED |

---

### Issues Found & Resolved

- **Issue 1:** Security alarm not latching. When motion stopped, the alarm turned off immediately.
  - *Fix:* Added an `alarmLatched` boolean flag in the state machine to ensure the alarm persists until manually reset.
- **Issue 2:** `TEMP_ALERT` (45°C) threshold was defined in `config.h` but unused.
  - *Fix:* Wired it into the fan controller and OLED display to show a critical warning.
- **Issue 3:** Button bounce causing erratic manual overrides.
  - *Fix:* Implemented software debounce logic (50ms delay check) for all input buttons.

---

### Test Conclusion
All functional requirements for the Smart Home Controller have been successfully validated in the Wokwi simulation environment. The logic handles edge cases (like hysteresis and sensor timeouts) correctly. The system is stable and ready for physical deployment.
