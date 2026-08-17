# Simulation Test Scenarios

## Overview
Demonstrate the Smart Home Controller using these 8 test scenarios.

## Scenarios

| Scenario | LDR Value | DHT Temp | PIR | Override | Security | Light Switch | Fan Switch | → Light | → Fan | → Buzzer | → Red | → Green | → OLED |
|----------|-----------|----------|-----|----------|----------|--------------|------------|---------|-------|----------|-------|---------|--------|
| 1. Bright Room + No Motion | Bright | 25°C | OFF | OFF | OFF | N/A | N/A | OFF | OFF | OFF | OFF | ON | Normal |
| 2. Dark Room + Motion | Dark | 25°C | ON | OFF | OFF | N/A | N/A | ON | OFF | OFF | OFF | ON | Light ON|
| 3. High Temperature | Bright | 32°C | OFF | OFF | OFF | N/A | N/A | OFF | ON | OFF | OFF | ON | Fan ON |
| 4. Normal Temperature| Bright | 27°C | OFF | OFF | OFF | N/A | N/A | OFF | OFF | OFF | OFF | ON | Normal |
| 5. Hysteresis Dead-Band| Bright | 29°C | OFF | OFF | OFF | N/A | N/A | OFF | NC* | OFF | OFF | ON | NC* |
| 6. Security + Motion | Bright | 25°C | ON | OFF | ON | N/A | N/A | OFF | OFF | ON | ON | OFF | ALERT |
| 7. Security Latching | Bright | 25°C | OFF | OFF | ON | N/A | N/A | OFF | OFF | ON | ON | OFF | ALERT |
| 8. Manual Override | Bright | 25°C | OFF | ON | OFF | ON | ON | ON | ON | OFF | OFF | ON | MANUAL |

*\*NC = No Change (maintains previous state)*

## Detailed Execution

### 1. Bright Room + No Motion (idle state)
- **Inputs**: LDR = bright, PIR = OFF
- **Expected**: All loads off. Green LED ON.

### 2. Dark Room + Motion Detected
- **Inputs**: LDR > 2000, PIR = ON
- **Expected**: Yellow LED turns ON.

### 3. High Temperature
- **Inputs**: DHT22 = 30°C+
- **Expected**: Cyan LED turns ON.

### 4. Normal Temperature
- **Inputs**: DHT22 = 28°C-
- **Expected**: Cyan LED turns OFF.

### 5. Hysteresis Dead-Band
- **Inputs**: DHT22 = 29°C
- **Expected**: Fan maintains its current state (ON if coming from 30°C, OFF if coming from 28°C).

### 6. Security Mode ON + Motion
- **Inputs**: Security Button pressed once. PIR = ON.
- **Expected**: Buzzer ON, Red LED ON, Green LED OFF, OLED displays "SECURITY ALERT".

### 7. Security Alert Latching
- **Inputs**: PIR goes back to OFF.
- **Expected**: Alarm continues until reset.

### 8. Manual Override ON
- **Inputs**: Override Button pressed.
- **Expected**: OLED shows "MANUAL". Light and Fan buttons control respective LEDs directly, bypassing sensor logic.
