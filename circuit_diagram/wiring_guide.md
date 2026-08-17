# Wiring Guide

## Step-by-Step Wiring Instructions

> [!WARNING]
> For student safety, use LOW-VOLTAGE LEDs as simulated loads. Do NOT connect mains AC appliances directly.

### 1. ESP32 ↔ PIR Sensor (HC-SR501)
- **VCC**: Connect to ESP32 **5V/VIN** (Red wire)
- **GND**: Connect to ESP32 **GND** (Black wire)
- **OUT**: Connect to ESP32 **GPIO 27** (Yellow wire)
- *Notes*: Needs 5V for reliable operation. Output is 3.3V compatible.

### 2. ESP32 ↔ LDR (Photoresistor)
- Connect a **10kΩ resistor** and the **LDR** in series.
- **Top of LDR**: Connect to ESP32 **3.3V**
- **Bottom of Resistor**: Connect to ESP32 **GND**
- **Junction**: Connect to ESP32 **GPIO 34** (Blue wire)

### 3. ESP32 ↔ DHT22 Sensor
- **VCC (Pin 1)**: Connect to ESP32 **3.3V** (Red wire)
- **DATA (Pin 2)**: Connect to ESP32 **GPIO 4** (Green wire)
- **GND (Pin 4)**: Connect to ESP32 **GND** (Black wire)
- *Notes*: Some modules include a built-in pull-up resistor. If bare sensor, add 10kΩ pull-up between VCC and DATA.

### 4. ESP32 ↔ OLED (SSD1306)
- **VCC**: Connect to ESP32 **3.3V**
- **GND**: Connect to ESP32 **GND**
- **SDA**: Connect to ESP32 **GPIO 21**
- **SCL**: Connect to ESP32 **GPIO 22**

### 5. ESP32 ↔ LEDs (Simulated Loads)
For each LED, wire a 330Ω resistor in series to limit current:
- **Yellow LED (Room Light)**: Anode to **GPIO 25**, Cathode to Resistor → **GND**
- **Cyan LED (Fan)**: Anode to **GPIO 26**, Cathode to Resistor → **GND**
- **Red LED (Alert)**: Anode to **GPIO 12**, Cathode to Resistor → **GND**
- **Green LED (Status)**: Anode to **GPIO 13**, Cathode to Resistor → **GND**

### 6. ESP32 ↔ Buzzer (Active)
- **Positive (+)**: Connect to **GPIO 14**
- **Negative (-)**: Connect to **GND**

### 7. ESP32 ↔ Push Buttons
No external resistors needed. Use `INPUT_PULLUP` in code.
- **Override**: Button to **GPIO 19** and **GND**
- **Light**: Button to **GPIO 32** and **GND**
- **Fan**: Button to **GPIO 33** and **GND**
- **Security**: Button to **GPIO 18** and **GND**

## Complete Wiring Summary Table

| Component | Pin / Terminal | ESP32 Connection | Wire Color Recommendation |
|-----------|----------------|------------------|---------------------------|
| PIR | OUT | GPIO 27 | Yellow |
| LDR | Divider Junction | GPIO 34 | Blue |
| DHT22 | DATA | GPIO 4 | Green |
| OLED | SDA | GPIO 21 | Orange |
| OLED | SCL | GPIO 22 | White |
| Yellow LED | Anode | GPIO 25 | Yellow |
| Cyan LED | Anode | GPIO 26 | Cyan/Blue |
| Red LED | Anode | GPIO 12 | Red |
| Green LED | Anode | GPIO 13 | Green |
| Buzzer | + | GPIO 14 | Purple |
| Button 1 | Terminal 1 | GPIO 19 | Gray |
| Button 2 | Terminal 1 | GPIO 32 | Gray |
| Button 3 | Terminal 1 | GPIO 33 | Gray |
| Button 4 | Terminal 1 | GPIO 18 | Gray |
| All | Power/VCC | 3.3V / 5V | Red |
| All | Ground/GND | GND | Black |
