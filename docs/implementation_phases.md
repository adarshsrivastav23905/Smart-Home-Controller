# Implementation Phases

This document details the step-by-step methodology for constructing the Smart Home Controller. The project is divided into 15 logical phases, each with specific objectives, steps, expected outputs, and debugging strategies.

---

## 1. Development Environment Setup
- **Objective**: Configure the required software stack.
- **Implementation Steps**:
  1. Install VS Code and the PlatformIO extension.
  2. Install the Wokwi Simulator extension for VS Code.
  3. Initialize a new PlatformIO project for the ESP32 DevKit V4.
  4. Configure `platformio.ini` with library dependencies.
- **Code Involved**: `platformio.ini`
- **Expected Output**: A compilable empty project.
- **Common Errors**: Missing Python path for PlatformIO, wrong board selected.
- **Debugging Tips**: Ensure Python is in System PATH. Clean and Rebuild project.
- **Proof to Capture**: Screenshot of a successful build log.

## 2. Component Selection
- **Objective**: Identify and document the necessary hardware.
- **Implementation Steps**:
  1. Select microcontroller (ESP32).
  2. Select sensors (DHT22, LDR, HC-SR501).
  3. Select output devices (OLED, LEDs, Buzzer).
  4. Document GPIO pin assignments.
- **Code Involved**: `include/config.h` (Pin Definitions)
- **Expected Output**: A comprehensive list of pin mappings.
- **Common Errors**: Assigning input-only pins (34-39) as outputs.
- **Debugging Tips**: Refer to the ESP32 official pinout diagram.
- **Proof to Capture**: Table of pin mappings.

## 3. Circuit Design
- **Objective**: Wire the components in the simulator.
- **Implementation Steps**:
  1. Place ESP32 on the virtual breadboard.
  2. Connect VCC and GND rails.
  3. Wire sensors and actuators to assigned GPIOs.
  4. Add pull-up/pull-down resistors where necessary.
- **Code Involved**: `diagram.json` (Wokwi Configuration)
- **Expected Output**: A fully wired visual circuit layout.
- **Common Errors**: Missing GND connections, incorrect I2C wiring (SDA/SCL swapped).
- **Debugging Tips**: Trace every wire back to the ESP32 datasheet.
- **Proof to Capture**: Screenshot of the `diagram.json` visual editor.

## 4. PIR Sensor Interfacing
- **Objective**: Detect motion reliably.
- **Implementation Steps**:
  1. Configure PIR pin as `INPUT`.
  2. Read state using `digitalRead()`.
  3. Print "Motion Detected" to Serial Monitor.
- **Code Involved**: `src/main.cpp`, `src/sensors.cpp`
- **Expected Output**: Serial Monitor shows motion events.
- **Common Errors**: Floating inputs causing false triggers.
- **Debugging Tips**: Use an external pull-down resistor in hardware, or software delays for debouncing.
- **Proof to Capture**: Serial Monitor output showing 1 and 0 values.

## 5. LDR Interfacing
- **Objective**: Measure ambient light intensity.
- **Implementation Steps**:
  1. Set up a voltage divider with the LDR and a 10k resistor.
  2. Read analog value using `analogRead()`.
  3. Calibrate threshold for "Dark" and "Bright".
- **Code Involved**: `src/main.cpp`, `src/sensors.cpp`
- **Expected Output**: ADC values between 0-4095 on Serial Monitor.
- **Common Errors**: Incorrect resistor in voltage divider causing saturated readings.
- **Debugging Tips**: Measure voltage with a multimeter. Adjust threshold based on ambient conditions.
- **Proof to Capture**: Graph or log of varying LDR values.

## 6. Temperature Sensor Interfacing
- **Objective**: Read temperature and humidity data.
- **Implementation Steps**:
  1. Include DHT library.
  2. Initialize DHT sensor object.
  3. Read temperature using `dht.readTemperature()`.
  4. Handle `NaN` reading errors.
- **Code Involved**: `src/main.cpp`, `src/sensors.cpp`
- **Expected Output**: Accurate °C readings printed to Serial.
- **Common Errors**: Missing 10k pull-up resistor on the data line. Reading too frequently.
- **Debugging Tips**: Ensure a minimum 2-second delay between DHT readings.
- **Proof to Capture**: Serial output showing Temp and Humidity.

## 7. Automatic Light Control
- **Objective**: Automate lights based on motion and darkness.
- **Implementation Steps**:
  1. Combine LDR and PIR logic.
  2. Implement non-blocking `millis()` timeout.
  3. Turn Light LED ON/OFF based on logic rules.
- **Code Involved**: `src/automation.cpp`
- **Expected Output**: Light turns ON in dark + motion, OFF after 30s timeout.
- **Common Errors**: Using `delay()` blocking the entire program loop.
- **Debugging Tips**: Log the elapsed time `(millis() - lastMotion)` to Serial to verify timeout logic.
- **Proof to Capture**: Video snippet of light responding to Wokwi LDR/PIR manipulation.

## 8. Automatic Fan Control
- **Objective**: Automate fan cooling with hysteresis.
- **Implementation Steps**:
  1. Implement upper threshold ($30.0^\circ C$) to turn fan ON.
  2. Implement lower threshold ($28.0^\circ C$) to turn fan OFF.
  3. Leave the state unchanged between $28-30^\circ C$.
- **Code Involved**: `src/automation.cpp`
- **Expected Output**: Fan turns ON/OFF smoothly without rapid toggling.
- **Common Errors**: Setting exact thresholds (e.g., $30^\circ C$) causing infinite toggling around the boundary.
- **Debugging Tips**: Explicitly print the fan state variable during threshold crossings.
- **Proof to Capture**: Serial log showing fan state transitions across thresholds.

## 9. Security Alert System
- **Objective**: Implement a latched alarm state.
- **Implementation Steps**:
  1. Read Security Switch state.
  2. Create Finite State Machine logic.
  3. On motion, latch into ALERT state, pulse buzzer using `millis()`.
- **Code Involved**: `src/security.cpp`
- **Expected Output**: Continuous buzzer pulsing and red LED flashing when intruded, until reset.
- **Common Errors**: Alarm stops when motion stops (failure to latch).
- **Debugging Tips**: Ensure the state variable (`alarmLatched`) is strictly evaluated and only reset by the switch.
- **Proof to Capture**: State diagram verification in code execution.

## 10. Manual Override
- **Objective**: Allow user to manually control outputs.
- **Implementation Steps**:
  1. Read Manual Override Switch (`INPUT_PULLUP`).
  2. Use `if/else` priority logic to bypass sensor checks.
  3. Read secondary switches for manual light/fan state.
- **Code Involved**: `src/automation.cpp`
- **Expected Output**: Actuators follow physical switches when Override is active.
- **Common Errors**: Switch bouncing causing unpredictable states.
- **Debugging Tips**: Implement basic software debouncing or wait delays.
- **Proof to Capture**: Demo showing manual switches controlling LEDs.

## 11. OLED Display Integration
- **Objective**: Provide a real-time status dashboard.
- **Implementation Steps**:
  1. Initialize I2C communication.
  2. Set up Adafruit SSD1306 display object.
  3. Format strings for Temp, Light, Motion, and System State.
  4. Update display every 500ms.
- **Code Involved**: `src/display.cpp`
- **Expected Output**: Clear text dashboard on the OLED screen.
- **Common Errors**: I2C address mismatch (default is usually 0x3C). Display overlapping text.
- **Debugging Tips**: Run an I2C scanner sketch. Always call `display.clearDisplay()` before writing.
- **Proof to Capture**: Screenshot of the formatted OLED screen in Wokwi.

## 12. Complete System Integration
- **Objective**: Merge all subsystems.
- **Implementation Steps**:
  1. Refactor code into modular functions/files.
  2. Set up `setup()` and `loop()` in `main.cpp`.
  3. Ensure no blocking `delay()` functions remain.
- **Code Involved**: `src/main.cpp`
- **Expected Output**: All features work concurrently.
- **Common Errors**: Stack overflows or variable scope issues during merge.
- **Debugging Tips**: Test subsystems individually one last time before merging.
- **Proof to Capture**: Code structure screenshot.

## 13. Testing and Debugging
- **Objective**: Validate system reliability and edge cases.
- **Implementation Steps**:
  1. Test priority hierarchy (Security > Manual > Auto).
  2. Test timeout boundaries.
  3. Review serial logs for unexpected behavior.
- **Code Involved**: All files.
- **Expected Output**: 100% pass on the decision matrix table.
- **Common Errors**: Edge cases failing (e.g., triggering manual override during an active security alert).
- **Debugging Tips**: Follow the execution path step-by-step using Serial prints.
- **Proof to Capture**: Verification checklist.

## 14. Virtual Simulation
- **Objective**: Run full tests in Wokwi without hardware.
- **Implementation Steps**:
  1. Tune `diagram.json` layout for presentation.
  2. Manipulate virtual sliders (Temp, Light).
  3. Click PIR to simulate motion.
- **Code Involved**: `diagram.json`
- **Expected Output**: Smooth, visual simulation of a smart home environment.
- **Common Errors**: Simulation freezing due to tight while-loops.
- **Debugging Tips**: Add `yield()` or small `delay(1)` inside heavy loops.
- **Proof to Capture**: GIF or video of the Wokwi simulation running.

## 15. GitHub Documentation
- **Objective**: Publish the project professionally.
- **Implementation Steps**:
  1. Write `README.md`.
  2. Compile all `docs/` files (Logic, Phases, Requirements).
  3. Add diagrams, tables, and screenshots.
  4. Commit and push to remote repository.
- **Code Involved**: `README.md`, `docs/*.md`
- **Expected Output**: A professional public GitHub repository.
- **Common Errors**: Broken markdown links, missing image assets.
- **Debugging Tips**: Preview markdown locally before pushing.
- **Proof to Capture**: Link to the final GitHub repo.
