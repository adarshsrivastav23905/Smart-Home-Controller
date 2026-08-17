# System Architecture

## Complete System Architecture

### 1. System Block Diagram

```text
                         SMART HOME CONTROLLER SYSTEM
                                       │
                                       ▼
                         ┌──────────────────────────┐
                         │    ESP32 DevKit V4       │
                         │  Xtensa Dual-Core MCU    │
                         └────────────┬─────────────┘
                                      │
        ┌─────────────────────────────┼─────────────────────────────┐
        │                             │                             │
        ▼                             ▼                             ▼
 ┌───────────────┐             ┌───────────────┐             ┌───────────────┐
 │ Input Layer   │             │ Control Logic │             │ Output Layer  │
 │               │             │               │             │               │
 │ • PIR Sensor  │             │ • Thresholds  │             │ • Yellow LED  │
 │ • LDR Sensor  │────────────▶│ • Modes       │────────────▶│ • Cyan LED    │
 │ • DHT22       │             │ • Timers      │             │ • Buzzer      │
 │ • Push Buttons│             │ • Overrides   │             │ • Red LED     │
 │   (x4)        │             │ • Alerts      │             │ • Green LED   │
 └───────────────┘             └───────────────┘             │ • OLED Display│
                                                             └───────────────┘
```

### 2. Input/Output Table

| Component | ESP32 GPIO | Direction | Type | Description |
|-----------|------------|-----------|------|-------------|
| PIR Sensor | GPIO 27 | Input | Digital | Motion detection (HC-SR501) |
| LDR Sensor | GPIO 34 | Input | Analog | Light level detection (ADC1_CH6) |
| DHT22 Sensor | GPIO 4 | Input/Output | One-Wire | Temperature and humidity |
| Override Button | GPIO 19 | Input | Digital (Pull-up) | Toggle manual/auto mode |
| Light Button | GPIO 32 | Input | Digital (Pull-up) | Manual light control |
| Fan Button | GPIO 33 | Input | Digital (Pull-up) | Manual fan control |
| Security Button | GPIO 18 | Input | Digital (Pull-up) | Toggle security mode |
| Yellow LED | GPIO 25 | Output | Digital | Room light simulated load |
| Cyan LED | GPIO 26 | Output | Digital | Fan simulated load |
| Active Buzzer | GPIO 14 | Output | Digital | Alarm/Alert sounder |
| Red LED | GPIO 12 | Output | Digital | Alert status indicator |
| Green LED | GPIO 13 | Output | Digital | Normal status indicator |
| OLED SDA | GPIO 21 | Bidirectional | I2C | I2C Data line for SSD1306 |
| OLED SCL | GPIO 22 | Output | I2C | I2C Clock line for SSD1306 |

### 3. Sensor Table

| Sensor | Type | Range | Accuracy | Interface | Pin |
|--------|------|-------|----------|-----------|-----|
| DHT22 | Temp/Humidity | -40°C to 80°C, 0-100% RH | ±0.5°C, ±2% RH | One-Wire Digital | GPIO 4 |
| HC-SR501 | PIR Motion | 3-7 meters, 110° angle | N/A | Digital (High/Low) | GPIO 27 |
| Photoresistor | Light (LDR) | 0-4095 (12-bit ADC) | N/A | Analog Voltage Divider | GPIO 34 |

### 4. Threshold Table

| Parameter | Value | Units | Purpose |
|-----------|-------|-------|---------|
| LDR_DARK_THRESHOLD | 1000 | ADC (0-4095) | Below this, room is considered dark |
| TEMP_FAN_ON | 30.0 | °C | Turn fan on at or above this temp |
| TEMP_FAN_OFF | 28.0 | °C | Turn fan off at or below this temp |
| TEMP_ALERT | 45.0 | °C | Critical temperature threshold for alarm |
| MOTION_TIMEOUT | 30000 | ms | Time to keep light on after last motion |

### 5. Control Logic Table

| Rule | Inputs | Conditions | Outputs |
|------|--------|------------|---------|
| Auto Light | LDR, PIR, Auto Mode | LDR < 1000 AND PIR == HIGH | Yellow LED ON |
| Auto Fan ON | DHT22, Auto Mode | Temp >= 30.0°C | Cyan LED ON |
| Auto Fan OFF | DHT22, Auto Mode | Temp <= 28.0°C | Cyan LED OFF |
| Security Alarm | PIR, Security Mode | PIR == HIGH AND Security == ON | Buzzer ON, Red LED ON |
| Manual Override | Buttons, Over. Mode | Override == ON, Button Pressed | Appropriate Load Toggled |

### 6. Timing Architecture Table

| Interval Name | Value | Purpose |
|---------------|-------|---------|
| SENSOR_READ_INTERVAL | 2000 ms | Frequency of reading DHT22 and LDR |
| DISPLAY_UPDATE_INTERVAL | 500 ms | OLED refresh rate |
| SERIAL_PRINT_INTERVAL | 3000 ms | Serial debug output rate |
| BUZZER_BEEP_INTERVAL | 500 ms | Alarm toggle frequency |
| MOTION_TIMEOUT | 30000 ms | Delay before turning off auto light |
