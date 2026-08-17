# Wokwi Virtual Simulation Guide

## Overview
This document outlines the 17 steps required to simulate the Smart Home Controller project using Wokwi.

## Step-by-step Setup
1. **Create ESP32 project**: Go to wokwi.com and create a new ESP32 project.
2. **Add PIR sensor**: Click '+' and add a PIR motion sensor.
3. **Add LDR**: Add a photoresistor sensor module (`wokwi-photoresistor-sensor`). Wire VCC to 3.3V, GND to GND, and AO to GPIO 34.
4. **Add DHT22**: Add the DHT22 temperature and humidity sensor.
5. **Add Yellow LED**: Add an LED, color yellow, and a 330Ω resistor.
6. **Add Cyan LED**: Add an LED, color cyan, and a 330Ω resistor.
7. **Add Red LED**: Add an LED, color red, and a 330Ω resistor.
8. **Add Green LED**: Add an LED, color green, and a 330Ω resistor.
9. **Add Buzzer**: Add an active buzzer.
10. **Add SSD1306 OLED**: Add the SSD1306 OLED Display (I2C).
11. **Add 4 Push Buttons**: Add push buttons for Override, Light, Fan, and Security.
12. **Connect all components**: Refer to `wiring_guide.md` and complete all connections.
13. **Paste code**: Copy `smart_home_controller.ino` into `sketch.ino`.
14. **Start simulation**: Click the "Play" button. Ensure OLED turns on.
15. **Verify Light**: Adjust LDR slider (dark) and trigger PIR to verify yellow LED turns ON.
16. **Verify Fan**: Adjust DHT22 slider (>30°C) to verify cyan LED turns ON.
17. **Verify Alarm**: Press security button, then trigger PIR. Verify buzzer and red LED.

## Wokwi VS Code Extension
Use the Wokwi VS Code extension with `diagram.json`. This JSON defines the visual layout and connections. Start the simulator directly in VS Code.

## Expected Outputs
- **Serial Monitor**: "System Initialized", followed by sensor readings every 3 seconds.
- **OLED Display**: Shows "SMART HOME" title, current Temp/Humidity, Auto/Manual mode, and Light/Fan status.
