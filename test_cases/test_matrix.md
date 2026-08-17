# Testing Strategy and Test Matrix

## Test Case Table

| Test ID | Category | Test Scenario | Input Conditions | Expected Output | Pass/Fail |
|---------|----------|---------------|------------------|-----------------|-----------|
| TC-01 | Init | System Boot & Initialization | Power ON | OLED shows boot screen, Green LED ON | [ ] |
| TC-02 | Auto | Bright Room + No Motion | LDR < 2000, PIR = LOW | Light OFF, Green LED ON | [ ] |
| TC-03 | Auto | Dark Room + No Motion | LDR > 2000, PIR = LOW | Light OFF | [ ] |
| TC-04 | Auto | Dark Room + Motion Detected | LDR > 2000, PIR = HIGH | Light ON | [ ] |
| TC-05 | Auto | Motion Stops in Dark Room | LDR > 2000, PIR = LOW | Light stays ON for 30s, then OFF | [ ] |
| TC-06 | Auto | Bright Room + Motion | LDR < 2000, PIR = HIGH | Light stays OFF | [ ] |
| TC-07 | Temp | Temperature ≥ 30°C | DHT Temp ≥ 30°C | Fan ON | [ ] |
| TC-08 | Temp | Temperature ≤ 28°C | DHT Temp ≤ 28°C | Fan OFF | [ ] |
| TC-09 | Temp | Temp 29°C (Hysteresis) | Change Temp to 29°C | Fan state unchanged | [ ] |
| TC-10 | Temp | Critical Temp ≥ 45°C | DHT Temp ≥ 45°C | Buzzer ON, Red LED ON | [ ] |
| TC-11 | Sec | Security Disabled | PIR = HIGH | No alarm | [ ] |
| TC-12 | Sec | Security Armed + No Motion | Security=ON, PIR=LOW | System armed, no alarm | [ ] |
| TC-13 | Sec | Security Armed + Motion | Security=ON, PIR=HIGH| ALERT! Buzzer+Red LED ON | [ ] |
| TC-14 | Sec | Security Alert Latching | PIR = LOW after alarm| Alarm remains active | [ ] |
| TC-15 | Sec | Security Disarm | Press Sec Button | Alert clears, Green LED ON | [ ] |
| TC-16 | Man | Manual Override → Light | Override=ON, Light Btn | Toggle Light state | [ ] |
| TC-17 | Man | Manual Override → Fan | Override=ON, Fan Btn | Toggle Fan state | [ ] |
| TC-18 | Error | DHT22 Sensor Failure | Disconnect Data pin | Safe defaults, OLED Error | [ ] |
| TC-19 | Stress | Rapid sensor changes | Toggle inputs quickly | System remains stable | [ ] |
| TC-20 | Power | System Reset/Power cycle | Reset button press | Normal boot sequence | [ ] |

## Debugging Guide

### Common Issues and Solutions
- **OLED not displaying**
  - Check I2C address (default 0x3C).
  - Verify SDA (GPIO 21) and SCL (GPIO 22) wiring.
- **DHT22 returning NaN**
  - Check data pin connection.
  - Ensure read frequency is not less than 2 seconds (sensor limitation).
- **PIR always HIGH**
  - Allow 60 seconds warm-up time after power on.
  - Adjust potentiometer on physical module.
- **Buttons not responding**
  - Verify `INPUT_PULLUP` is set.
  - Check wiring to GND.
- **Buzzer always on**
  - Check pin conflict or logic inversion (some active buzzers trigger on LOW).
