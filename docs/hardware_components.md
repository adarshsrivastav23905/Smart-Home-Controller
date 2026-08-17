# Smart Home Controller: Hardware Components

## Section 5: Hardware Component Specifications

This document details the physical hardware components required to build the Smart Home Controller. Understanding the electrical specifications and communication protocols of each component is essential for safe and reliable system integration.

### 1. ESP32 DevKit V4 (Microcontroller)
- **Purpose**: The central brain of the system. It runs the firmware, processes sensor data, and controls all outputs.
- **Input/Output Type**: Supports Digital I/O, Analog Input (ADC), PWM, I2C, SPI, UART.
- **Communication Method**: USB for programming/serial monitor; Wi-Fi/Bluetooth (optional for advanced features).
- **Expected Behavior**: Executes the compiled C++ code in a continuous loop at high speed.
- **Project Usage**: Reads DHT22, LDR, PIR, and buttons; drives LEDs, Buzzer, and OLED.
- **Specifications**: 
  - CPU: Xtensa Dual-Core 32-bit LX6 microprocessor, 160 or 240 MHz.
  - Memory: 520 KB SRAM.
  - Logic Level: 3.3V (Inputs are generally NOT 5V tolerant).
  - GPIOs: 34 programmable pins.

### 2. HC-SR501 PIR Motion Sensor
- **Purpose**: Detects human or animal motion by sensing changes in infrared radiation.
- **Input/Output Type**: Digital Output.
- **Communication Method**: Outputs a HIGH logic signal (3.3V) when motion is detected.
- **Expected Behavior**: Pin goes HIGH when a warm body moves across its field of view, remains HIGH for a set delay, then goes LOW.
- **Project Usage**: Triggers the security alarm if the system is in the `ARMED` state.
- **Specifications**:
  - Operating Voltage: 4.5V - 20V (typically powered via 5V pin).
  - Output Level: 3.3V (HIGH), 0V (LOW) - safe for ESP32.
  - Range: Adjustable, typically 3 to 7 meters.
  - Delay Time: Adjustable via onboard potentiometer (0.3s to 5 mins).

### 3. LDR (Light Dependent Resistor / Photoresistor)
- **Purpose**: Detects ambient light intensity.
- **Input/Output Type**: Analog (Variable Resistance).
- **Communication Method**: Analog voltage via a voltage divider circuit.
- **Expected Behavior**: Resistance decreases as light intensity increases.
- **Project Usage**: Determines if it is "dark" to automatically turn on the room light (Yellow LED).
- **Specifications**:
  - Resistance Range: Typically a few hundred ohms in bright light to over 1 MΩ in total darkness.
  - Circuit: Wired in series with a 10kΩ fixed resistor. The junction connects to ESP32 GPIO 34.

### 4. DHT22 / AM2302 Temperature and Humidity Sensor
- **Purpose**: Accurately measures ambient air temperature and relative humidity.
- **Input/Output Type**: Digital Data (Serial).
- **Communication Method**: Proprietary single-wire bidirectional serial protocol.
- **Expected Behavior**: Outputs a 40-bit data stream representing temp, humidity, and a checksum when polled.
- **Project Usage**: Controls the fan logic (turns Fan LED on if Temp >= 30°C) and provides data for the OLED dashboard.
- **Specifications**:
  - Operating Voltage: 3.3V to 5.5V.
  - Temp Range: -40°C to 80°C (Accuracy: ±0.5°C).
  - Humidity Range: 0% to 100% RH (Accuracy: ±2-5%).
  - Sampling Rate: Max 0.5 Hz (One reading every 2 seconds).

### 5. Relay Module (Conceptual / Advanced Use)
- **Purpose**: Acts as an electrically operated switch to control high-power AC devices safely using low-voltage DC signals.
- **Input/Output Type**: Digital Input (Control).
- **Communication Method**: MCU sends a HIGH/LOW signal to the relay's optocoupler/transistor.
- **Expected Behavior**: A click is heard when energized, closing the high-voltage circuit.
- **Project Usage**: Used in real deployments to control actual ceiling fans and bulbs. *Note: We use LEDs in this project to simulate relays for safety.*
- **Specifications**: Control voltage 3.3V/5V. Load capacity: Typically 10A at 250V AC.

### 6. LEDs (Light Emitting Diodes)
- **Purpose**: Provide visual indication of system states and simulate home appliances.
- **Input/Output Type**: Digital Output.
- **Communication Method**: Driven directly by GPIO pins.
- **Expected Behavior**: Emits light when forward-biased with sufficient voltage and current.
- **Project Usage**: 
  - Yellow LED = Room Light
  - Cyan (or Blue) LED = Ceiling Fan
  - Red LED = Security Alert
  - Green LED = Security Armed Status
- **Specifications**: Forward voltage typically 1.8V to 3.3V depending on color. Max continuous current ~20mA.

### 7. Active Buzzer
- **Purpose**: Emits an audible alarm or notification sound.
- **Input/Output Type**: Digital Output.
- **Communication Method**: Driven HIGH to emit sound.
- **Expected Behavior**: Emits a continuous tone at a fixed frequency when powered.
- **Project Usage**: Sounds the alarm during a security breach (PIR motion when ARMED).
- **Specifications**: Operating voltage 3.3V or 5V. Contains an internal oscillator circuit (unlike a passive buzzer which requires a PWM signal).

### 8. SSD1306 OLED Display
- **Purpose**: Provides a local user interface and telemetry dashboard.
- **Input/Output Type**: Digital Serial.
- **Communication Method**: I2C protocol (SDA, SCL lines).
- **Expected Behavior**: Displays text, graphics, and bitmaps as commanded by the MCU.
- **Project Usage**: Shows real-time temperature, humidity, light level, fan/light status, and security mode.
- **Specifications**:
  - Resolution: 128x64 pixels.
  - Driver IC: SSD1306.
  - Interface: I2C (Address usually `0x3C`).
  - Operating Voltage: 3.3V.

### 9. Push Buttons / Tactile Switches
- **Purpose**: Allow physical user input to interact with the system.
- **Input/Output Type**: Digital Input.
- **Communication Method**: Physically closes a circuit to pull a GPIO pin to GND.
- **Expected Behavior**: Momentary contact (Normally Open).
- **Project Usage**: Toggling manual override mode, manually controlling the light, manually controlling the fan, and changing the security state.
- **Specifications**: Utilizes the ESP32's internal pull-up resistors (`INPUT_PULLUP`).

### 10. Resistors (330Ω and 10kΩ)
- **Purpose**: Limit current to protect components or set voltage levels.
- **Expected Behavior**: Follows Ohm's Law ($V = I \times R$).
- **Project Usage**: 
  - 330Ω resistors are wired in series with the LEDs to limit current to safe levels (~10mA) and prevent burning out the LED or the ESP32 GPIO.
  - 10kΩ resistor is used with the LDR to form a voltage divider network.

### 11. Breadboard & 12. Jumper Wires
- **Purpose**: Allow for solderless prototyping and circuit testing.
- **Expected Behavior**: Internal metal clips connect components in horizontal rows and vertical power rails.
- **Project Usage**: The physical foundation upon which the student prototype is constructed.

### 13. Power Supply
- **Purpose**: Provides stable DC voltage and current to the entire system.
- **Project Usage**: Typically, the system is powered via the ESP32's micro-USB port (5V from a PC or charger). The ESP32's onboard AMS1117-3.3 voltage regulator drops this to the 3.3V required for the logic circuits.

> [!WARNING]
> When transitioning from the simulated project to physical hardware, ensure you calculate total current draw. If driving multiple relays, the ESP32's onboard 3.3V regulator may not supply enough current, requiring an external power supply.

> [!CAUTION]
> Do NOT connect mains AC voltage (110V/220V) to any breadboard circuit. Mains voltage is lethal. Always use opto-isolated relay modules enclosed in safe electrical boxes when controlling real appliances.
