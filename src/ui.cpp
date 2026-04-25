#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_SSD1306.h>
#include "ui.h"

// ================= LED INITIALIZATION =================
void initLED() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  Serial.println("LED initialized");
}

// ================= DISPLAY INITIALIZATION =================
void initDisplay() {
  Wire.begin(21, 32);
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;); // Halt
  }
  
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Initializing...");
  display.display();
  delay(1000);
  
  Serial.println("Display initialized");
}

// ================= LED ANIMATIONS =================
void animateIdleLED() {
  static int pos = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 30) {
    leds[pos] = CRGB::Blue;
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, 50);
    pos = (pos + 1) % NUM_LEDS;
    lastUpdate = millis();
  }
}

void showLEDAnimation(int type) {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 50) {
    if (type == 1) {
      // Green pulse
      fill_solid(leds, NUM_LEDS, CRGB::Green);
    } else if (type == 2) {
      // Red pulse
      fill_solid(leds, NUM_LEDS, CRGB::Red);
    } else if (type == 3) {
      // Rainbow
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV((i * 255 / NUM_LEDS), 255, 255);
      }
    }
    
    FastLED.show();
    lastUpdate = millis();
  }
}

void fadeAllLED() {
  fadeToBlackBy(leds, NUM_LEDS, 100);
  FastLED.show();
}

// ================= DISPLAY UPDATE =================
void updateDisplay() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate > 100) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    
    // Status line
    display.print("Status: ");
    display.println(isPlaying ? "PLAY" : "IDLE");
    
    // Song index
    display.print("Song: ");
    display.println(songIndex);
    
    // Tempo
    display.print("Tempo: ");
    display.print(tempo);
    display.println(" BPM");
    
    // Volume
    display.print("Volume: ");
    display.print(volume);
    display.println("%");
    
    // Mode indicator
    display.print("Mode: ");
    display.println(volumeMode ? "VOL" : "TEMPO");
    
    display.display();
    lastUpdate = millis();
  }
}

void displayMessage(const char* msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
}
