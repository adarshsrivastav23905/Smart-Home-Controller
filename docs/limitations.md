# Limitations & Future Improvements

This document outlines the known boundaries of the current Smart Home Controller iteration and establishes a roadmap for future development, transitioning the project from a prototype into a production-ready IoT product.

---

## Current Limitations

The V1.0 prototype successfully demonstrates core logic, but it possesses several constraints inherent to its educational and prototyping scope:

1. **Virtual Prototype Constraints**
   - The current iteration has been tested exclusively via Wokwi Simulation. It lacks real-world physical stress testing regarding wire resistance, power supply noise, or EMI (Electromagnetic Interference).
2. **Basic Switch Implementation**
   - The manual override and control inputs use simple momentary push buttons. While basic software debouncing is used, it lacks robust state-toggling mechanisms or physical latching switches.
3. **Sensor Filtering**
   - The Light Dependent Resistor (LDR) relies on instantaneous single-sample readings (`analogRead()`). This makes it highly susceptible to temporary shadows or flickering light. No moving average or low-pass filter algorithms are implemented yet.
4. **Offline Operation**
   - The ESP32 is a powerful Wi-Fi and Bluetooth-enabled chip, yet the current firmware operates 100% offline. No internet connectivity, remote access, or smartphone integration exists.
5. **Data Volatility**
   - There is no data logging. Sensor history, alert events, and temperature trends are lost the moment the microcontroller resets or loses power.
6. **Display Density**
   - The 0.96" SSD1306 OLED (128x64 resolution) severely limits information density. Long status messages or detailed graphs cannot be displayed simultaneously with sensor readings.
7. **No Power Management**
   - The code runs in a continuous active loop (`while(1)` equivalent). It does not utilize ESP32 Deep Sleep or Light Sleep modes, making it inefficient for battery-powered scenarios.
8. **Basic Security Implementation**
   - The security alarm relies entirely on a single PIR motion sensor. It lacks comprehensive perimeter defense (door/window magnetic reed switches) and visual verification (camera).

---

## Future Improvements

To evolve this project into an advanced, commercial-grade smart home ecosystem, the following features are planned for future versions (V2.0+):

1. **Wi-Fi and Network Connectivity**
   - Activate the ESP32's built-in 2.4GHz Wi-Fi radio to allow local network access and remote system management.
2. **MQTT IoT Protocol Integration**
   - Implement MQTT publish/subscribe architecture to stream real-time telemetry to a cloud broker (e.g., AWS IoT, Mosquitto), enabling seamless cloud integration.
3. **Mobile Dashboard (Blynk/Flutter)**
   - Develop a companion mobile application to view real-time data, receive push notifications for security alerts, and remotely toggle the manual overrides.
4. **Multi-Room Sensor Mesh Network**
   - Transition from a single-node setup to a master-slave architecture. Use ESP-NOW or BLE to connect multiple satellite sensor nodes in different rooms to the central controller.
5. **Voice Assistant Integration**
   - Link the controller to Google Assistant and Amazon Alexa APIs for voice-command capabilities (e.g., *"Hey Google, turn on Override Mode"*).
6. **Power Consumption Monitoring**
   - Integrate ACS712 or INA219 current sensors to monitor real-time wattage usage of connected appliances (Lights/Fans) and calculate energy costs.
7. **Machine Learning & Edge AI**
   - Train a tinyML model (via Edge Impulse) on historical occupancy and temperature data to predict user habits, turning on ACs or lights *before* the user enters a room.
8. **OTA (Over-The-Air) Firmware Updates**
   - Implement OTA flashing to allow wireless firmware upgrades, eliminating the need to connect a physical USB cable when pushing new code.
9. **Battery Backup & RTC**
   - Add a Li-Ion battery with a TP4056 charging circuit and a DS3231 Real-Time Clock module to ensure the system keeps running and timekeeping during grid power failures.
10. **Commercial Ecosystem Support**
    - Integrate the Matter protocol or Apple HomeKit compatibility to ensure the controller plays nicely with off-the-shelf smart devices from major brands.
