# Interview Preparation Guide

This document contains 10 core interview questions and 5 bonus questions, along with strong, detailed answers that demonstrate hands-on experience with the Smart Home Controller project.

---

### Core Questions

**Q1: Explain your project.**
**Answer:** I built a modular Smart Home Controller using the ESP32 microcontroller. The system monitors the environment and automates lighting and climate control. It uses a PIR sensor and an LDR to turn on lights (a yellow LED on GPIO 25) only when it's dark and motion is detected, implementing a 30-second timeout. It also uses a DHT22 to control a fan (a cyan LED on GPIO 26) based on temperature, using a 2°C hysteresis logic to prevent rapid toggling around the 30°C threshold. Additionally, I implemented a latching security alarm system and a manual override feature with debounced buttons. An I2C OLED display provides real-time telemetry. The entire system is written in C++ using an object-oriented approach in PlatformIO and verified using the Wokwi simulator.

**Q2: Why did you choose the ESP32 over a standard Arduino Uno?**
**Answer:** While the Uno is great for basics, the ESP32 is much more powerful. It operates at 240 MHz compared to the Uno's 16 MHz, has far more RAM, and operates on 3.3V logic. Most importantly, the ESP32 has built-in Wi-Fi and Bluetooth. Although my current version operates offline, using the ESP32 makes the project future-proof; I can easily upgrade it to an IoT device that connects to AWS or a local MQTT broker without changing the core hardware.

**Q3: How did you handle the analog inputs in your project?**
**Answer:** I used the ESP32's built-in ADC (Analog-to-Digital Converter) to read the LDR on GPIO 34. The ESP32 ADC is 12-bit, meaning it returns values from 0 to 4095. I set up a voltage divider with the LDR and a pull-down resistor. When the light level drops, the resistance increases, changing the voltage. I defined a threshold (`LDR_DARK_THRESHOLD = 1000`); if the analog read is below 1000, the system considers it "dark" and enables the motion-activated lighting logic.

**Q4: Explain how you interfaced the DHT22 sensor.**
**Answer:** The DHT22 is a digital sensor that uses a proprietary one-wire protocol. Instead of writing the complex timing logic manually, I used the Adafruit DHT library. I connected it to GPIO 4. In my code, I instantiate the DHT object and call `readTemperature()` and `readHumidity()`. It takes about 2 seconds between readings, so I implemented a non-blocking timer using `millis()` to poll it every 2000ms, ensuring my main loop isn't blocked by sensor delays.

**Q5: What is the purpose of the 30-second timeout on the PIR sensor?**
**Answer:** A PIR sensor only sends a HIGH signal when motion is actively occurring. If someone is sitting still reading a book, the sensor might read LOW. If the lights turned off immediately, it would be highly annoying. By implementing a 30,000 ms timeout using `millis()`, the system resets a timer every time motion is detected. The light only turns off if exactly 30 seconds have passed with zero motion detected, creating a much better user experience.

**Q6: Explain the logic behind your fan control. What is hysteresis?**
**Answer:** My fan turns on when the temperature hits 30.0°C. If I just used a simple threshold, and the room was hovering at exactly 30.0°C, a slight breeze could cause the sensor to read 29.9, turning the fan off, then 30.0, turning it on. This rapid toggling would damage a real relay or motor. To fix this, I implemented 2°C hysteresis. The fan turns ON at 30.0°C, but it won't turn OFF until the temperature drops below 28.0°C. This creates a stable deadband.

**Q7: How did you implement manual override and why is priority important?**
**Answer:** I connected push buttons to GPIOs (e.g., GPIO 19, 32, 33) using `INPUT_PULLUP`. When pressed, they ground the pin. I used software debouncing to ensure clean signals. Priority is critical in automation: if the system is in 'Auto' mode, the sensors dictate the state. But if a user physically presses a button, the system enters 'Manual' mode, bypassing sensor logic. However, I ensured that the Security Alarm has the absolute highest priority—an intruder alert will override everything else.

**Q8: Describe the security system state machine.**
**Answer:** The security system has three states: DISABLED, ARMED, and ALERT. A physical button on GPIO 18 toggles between DISABLED and ARMED. In the ARMED state, a green LED on GPIO 13 turns on. If the PIR detects motion while ARMED, the state transitions to ALERT. In ALERT, a buzzer (GPIO 14) and red LED (GPIO 12) activate. Crucially, I made the ALERT state 'latching'—even if the intruder stops moving, the alarm stays on until the user manually disarms it.

**Q9: How does your OLED display communicate with the ESP32?**
**Answer:** The 128x64 OLED uses the I2C (Inter-Integrated Circuit) protocol. It requires only two data wires: SDA (Serial Data) on GPIO 21 and SCL (Serial Clock) on GPIO 22, plus power and ground. It operates at the address 0x3C. I used the Adafruit SSD1306 library to communicate over I2C, allowing me to easily clear the buffer, set the cursor, print text showing telemetry (temp, light, mode), and push the buffer to the display.

**Q10: How did you test your system without physical hardware?**
**Answer:** I used the Wokwi online simulator. It allows me to build the circuit virtually and run my compiled C++ code directly on a simulated ESP32. I could click on the virtual LDR to change lux values, click the DHT22 to slide temperature up and down, and simulate button presses. This allowed me to safely test edge cases, like verify the hysteresis logic and the alarm latching mechanism, before touching any real hardware.

---

### Bonus Questions

**B1: What would you improve in future versions?**
I would add Wi-Fi capabilities to send telemetry to a cloud dashboard like Blynk or ThingsBoard. I'd also replace the simple LEDs with relay modules to control real 220V AC appliances.

**B2: How would you add Wi-Fi to this?**
Since the ESP32 has built-in Wi-Fi, I would include the `<WiFi.h>` library, connect to a local SSID, and use an MQTT client library like `PubSubClient` to publish sensor data and subscribe to commands from a smartphone app.

**B3: What is the difference between `delay()` and `millis()`?**
`delay()` is a blocking function; it stops the microcontroller from doing anything else (like reading buttons or sensors) while it waits. `millis()` returns the number of milliseconds since boot. By comparing the current `millis()` to a stored timestamp, I can create non-blocking timers, allowing the ESP32 to multitask efficiently.

**B4: How does I2C work?**
I2C is a synchronous, multi-master, multi-slave packet switched, single-ended, serial communication bus. It uses SDA (Data) and SCL (Clock). Every device has a unique address (like 0x3C for my OLED). The master (ESP32) generates the clock and addresses the specific slave to read or write data.

**B5: Why use `INPUT_PULLUP` for buttons?**
A floating pin (unconnected) will pick up environmental noise and read randomly HIGH or LOW. `INPUT_PULLUP` enables the ESP32's internal pull-up resistor (connecting the pin to 3.3V). So, the pin reads HIGH normally. The button connects the pin to ground. When pressed, it reads LOW. This provides a stable, predictable signal without needing external resistors.
