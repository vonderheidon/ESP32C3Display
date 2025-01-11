#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <U8g2lib.h>

// Pins: clock=6, data=5.
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// SSD1306 controller has 132x64 pixel buffer
const unsigned int BufferWidth  = 132;
const unsigned int BufferHeight = 64;
const unsigned int ScreenWidth  = 72;
const unsigned int ScreenHeight = 40;
const unsigned int xOffset = (BufferWidth - ScreenWidth) / 2;
const unsigned int yOffset = (BufferHeight - ScreenHeight) / 2;

const char* ssid = "your-ssid";  // Change this to your WiFi SSID
const char* password = "your-password";  // Change this to your WiFi password

void setup() {
  u8g2.begin();
  u8g2.setContrast(255);
  u8g2.setBusClock(400000);
  u8g2.setFont(u8g_font_helvR08);

  WiFi.hostname("esp32c3");
  WiFi.begin(ssid, password);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);  // Really important config, without this I can't connect on WiFi.

  u8g2.clearBuffer();
  u8g2.drawStr(xOffset, yOffset + 10, "Connecting");
  u8g2.sendBuffer();

  int attempts = 0;
  int dots = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(250);
    attempts++;
    u8g2.clearBuffer();
    u8g2.drawStr(xOffset, yOffset + 10, "Connecting");
    if (attempts % 2 == 0) {
        dots++;
        if (dots > 3) {
            dots = 0;
        }
    }
    for (int i = 0; i < dots; i++) {
        int x_pos = xOffset + u8g2.getStrWidth("Connecting") + (i * 5);
        if (x_pos + u8g2.getStrWidth(".") <= xOffset + ScreenWidth) {
          u8g2.drawStr(x_pos, yOffset + 10, ".");
        }
    }
    u8g2.sendBuffer();
  }

  u8g2.clearBuffer();

  if (WiFi.status() == WL_CONNECTED) {
    u8g2.drawStr(xOffset, yOffset + 10, "Connected!");
    u8g2.drawStr(xOffset, yOffset + 20, "IP Address:");
    u8g2.drawStr(xOffset, yOffset + 30, WiFi.localIP().toString().c_str());
    u8g2.sendBuffer();
  } else {
    u8g2.drawStr(xOffset, yOffset + 10, "Failed to");
    u8g2.drawStr(xOffset, yOffset + 20, "connect.");
    u8g2.sendBuffer();
  }
}

void loop() {
}