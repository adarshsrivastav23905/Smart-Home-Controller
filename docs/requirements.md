# Project Requirements

This document outlines the software environment, hardware components, and system prerequisites required to build, compile, and run the Smart Home Controller.

---

## System Requirements

To develop and simulate this project smoothly, your computer should meet the following minimum specifications:

- **Operating System**: Windows 10/11, macOS 11+, or Linux (Ubuntu 20.04+)
- **Processor**: Intel Core i3 / AMD Ryzen 3 or equivalent
- **Memory (RAM)**: Minimum 4 GB, Recommended 8 GB
- **Storage**: Minimum 2 GB of free disk space for IDE, toolchains, and project files.
- **Internet**: Required for downloading PlatformIO frameworks, library dependencies, and running the Wokwi Web Simulation.

---

## Software Requirements

The project relies on a modern embedded C++ development stack.

| Requirement | Version | Purpose |
|---|---|---|
| **VS Code** | 1.80+ | Primary Integrated Development Environment (IDE) for code editing. |
| **PlatformIO IDE** | 3.3+ (Ext) | Build system, dependency manager, and serial monitor for ESP32. |
| **Arduino IDE** | 2.2+ | Alternative IDE, useful for quick sketches and I2C scanning. |
| **Wokwi Extension** | Latest | Enables running virtual hardware simulations directly inside VS Code. |
| **Git** | 2.30+ | Version control system to manage code history and GitHub integration. |
| **Adafruit Unified Sensor**| 1.1.9+ | Core underlying driver for the DHT sensor library. |
| **DHT sensor library** | 1.4.4+ | Library for reading temperature and humidity from the DHT22. |
| **Adafruit GFX Library** | 1.11.5+ | Core graphics library providing drawing primitives for the OLED. |
| **Adafruit SSD1306** | 2.5.7+ | Specific driver for the 128x64 I2C OLED display. |

---

## Hardware Requirements (Physical Build)

If transitioning from simulation to a physical prototype, the following components are required. Prices are estimated in INR and subject to market variations.

| Component | Quantity | Estimated Cost (INR) | Purchase Source / Notes |
|---|:---:|:---:|---|
| **ESP32 DevKit V4 (38-pin)** | 1 | ₹400 - ₹550 | Amazon / Local Electronics Store |
| **DHT22 Temperature/Humidity Sensor** | 1 | ₹250 - ₹350 | More accurate than DHT11 |
| **HC-SR501 PIR Motion Sensor** | 1 | ₹80 - ₹120 | Adjustable sensitivity & delay |
| **LDR (Light Dependent Resistor)** | 1 | ₹10 - ₹20 | 5mm photoresistor |
| **0.96" OLED Display (I2C, SSD1306)**| 1 | ₹200 - ₹250 | 128x64 pixel resolution |
| **Active Buzzer (5V)** | 1 | ₹20 - ₹30 | For security alarms |
| **LEDs (Red, Green, Yellow, Cyan)** | 4 | ₹10 - ₹20 | Actuator indicators |
| **Push Buttons (Momentary)** | 3 | ₹30 - ₹50 | For Override, Light, Fan toggles |
| **Slide Switch (SPDT)** | 1 | ₹15 - ₹25 | For Security System ARM/DISARM |
| **Resistors (10kΩ, 330Ω)** | ~10 | ₹10 - ₹20 | Pull-ups, voltage dividers, LED limiters |
| **Breadboard (830 Tie-Points)** | 1 | ₹100 - ₹150 | Prototyping base |
| **Jumper Wires (M-M, M-F)** | 1 Set| ₹100 - ₹150 | Dupont cables |
| **Micro-USB Data Cable** | 1 | ₹50 - ₹100 | For programming the ESP32 |

**Total Estimated Hardware Cost:** ₹1,275 - ₹1,835 INR

---

## Virtual Requirements (Wokwi Simulation)

> [!IMPORTANT]
> **No Physical Hardware Required!**
> This project has been fully configured for virtual execution using the **Wokwi Simulator**.

By using the provided `diagram.json`, you can run the entire system in your browser or VS Code without spending any money on physical hardware. The simulation provides:
- Virtual sliders to change Temperature and Light levels.
- Clickable PIR modules to simulate human motion.
- Real-time rendering of the OLED display and LEDs.
- Accurate code execution matching actual ESP32 hardware behavior.
