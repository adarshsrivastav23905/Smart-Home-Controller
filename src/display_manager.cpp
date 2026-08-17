/**
 * ============================================================
 *  DISPLAY MANAGER - Implementation
 * ============================================================
 *  Drives a 128x64 SSD1306 OLED display over I2C bus.
 *
 *  Display Layout (128x64 pixels, 6 text lines at size 1):
 *    Line 0 (Y= 0): "=== SMART HOME ==="       (Title)
 *    Line 1 (Y=10): "Temp: 29.5C  H:65%"        (Sensors)
 *    Line 2 (Y=20): "Light:ON   Room:DARK"       (Light)
 *    Line 3 (Y=30): "Fan:OFF  Motion:YES"        (Fan)
 *    Line 4 (Y=40): "Mode: AUTO"                 (Mode)
 *    Line 5 (Y=50): "Security: ARMED"            (Security)
 *
 *  I2C Communication:
 *    SDA = GPIO 21 (data line)
 *    SCL = GPIO 22 (clock line)
 *    Address = 0x3C (default for most SSD1306 modules)
 *
 *  This demonstrates:
 *    - I2C bus communication protocol
 *    - Graphics library usage
 *    - Real-time display updates
 *    - Status visualization
 * ============================================================
 */

#include "display_manager.h"
#include "config.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Create display object: width, height, I2C reference, reset pin (-1 = none)
static Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, -1);

/**
 * Initialize the OLED display and show a boot splash screen.
 *
 * The splash screen displays for 2 seconds while other
 * subsystems finish initialization. This gives the user
 * visual confirmation that the system is starting up.
 */
void initDisplay() {
    // Initialize I2C bus with ESP32 SDA/SCL pins
    Wire.begin(OLED_SDA, OLED_SCL);

    // Initialize the SSD1306 OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("[ERROR] SSD1306 OLED initialization failed!"));
        Serial.println(F("[ERROR] Check I2C wiring: SDA=GPIO21, SCL=GPIO22"));
        return;
    }

    // ---- Boot Splash Screen ----
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10, 0);
    display.println(F("=================="));
    display.setCursor(10, 10);
    display.println(F("  SMART HOME"));
    display.setCursor(10, 20);
    display.println(F("  CONTROLLER"));
    display.setCursor(10, 30);
    display.println(F("=================="));
    display.setCursor(10, 42);
    display.println(F("Initializing..."));
    display.setCursor(10, 54);
    display.println(F("Adarsh Srivastav"));

    display.display();
    delay(2000);  // Show splash for 2 seconds
}

/**
 * Update the OLED display with current system status.
 *
 * This function completely redraws the display each time
 * it is called. The SSD1306 library handles the I2C
 * communication to transfer the frame buffer to the display.
 *
 * Call frequency: every DISPLAY_UPDATE_INTERVAL ms (500ms)
 */
void updateDisplay(float temperature, float humidity, bool lightOn, bool fanOn,
                   bool motionDetected, bool isDark, bool manualOverride,
                   const char* securityStatus) {

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Get current time to flash text every 500ms
    bool flashState = (millis() / 500) % 2 == 0;

    // ---- HIGHEST PRIORITY: INTRUDER ALERT OVERRIDE ----
    // If the alarm is triggered, take over the entire display
    if (strcmp(securityStatus, "*** ALERT! ***") == 0) {
        if (flashState) {
            display.setTextSize(2); // Large text
            display.setCursor(15, 15);
            display.println(F("INTRUDER"));
            display.setCursor(30, 35);
            display.println(F("ALERT!"));
        }
        display.display();
        return; // Skip drawing normal UI
    }

    // ---- SECOND PRIORITY: CRITICAL TEMPERATURE OVERRIDE ----
    // If temp >= 45C, display warning
    if (temperature >= TEMP_ALERT) {
        if (flashState) {
            display.setTextSize(2);
            display.setCursor(5, 10);
            display.println(F("TEMP > 45C"));
            display.setCursor(20, 30);
            display.println(F("DANGER!!"));
            
            display.setTextSize(1);
            display.setCursor(5, 50);
            display.print(F("Current: "));
            display.print(temperature, 1);
            display.print(F(" C"));
        }
        display.display();
        return; // Skip drawing normal UI
    }

    // ---- NORMAL DASHBOARD UI ----
    display.setTextSize(1);

    // Line 0: Title Bar
    display.setCursor(0, 0);
    display.println(F("=== SMART HOME ==="));

    // Line 1: Temperature & Humidity
    display.setCursor(0, 10);
    display.print(F("Temp:"));
    display.print(temperature, 1);        // 1 decimal place
    display.print(F("C  H:"));
    display.print((int)humidity);
    display.println(F("%"));

    // Line 2: Light Status & Room Brightness
    display.setCursor(0, 20);
    display.print(F("Light:"));
    display.print(lightOn ? "ON " : "OFF");
    display.print(F("  Room:"));
    display.println(isDark ? "DARK" : "BRIGHT");

    // Line 3: Fan Status & Motion Detection
    display.setCursor(0, 30);
    display.print(F("Fan:"));
    display.print(fanOn ? "ON " : "OFF");
    display.print(F("  Motion:"));
    display.println(motionDetected ? "YES" : "NO");

    // Line 4: Operating Mode
    display.setCursor(0, 40);
    display.print(F("Mode: "));
    display.println(manualOverride ? "MANUAL" : "AUTO");

    // Line 5: Security Status
    display.setCursor(0, 50);
    display.print(F("Security: "));
    display.println(securityStatus);

    // Transfer frame buffer to physical display via I2C
    display.display();
}
