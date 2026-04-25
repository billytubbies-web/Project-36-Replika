#include <Arduino.h>
#include "audio.h"
#include "ui.h"

// ================= GLOBAL VARIABLES =================
int songIndex = 0;
int tempo = 120;
int volume = 100;
bool isPlaying = false;
bool volumeMode = false;

CRGB leds[NUM_LEDS];
Adafruit_SSD1306 display(128, 64, &Wire, -1);
File midiFile;

TaskHandle_t AudioTaskHandle;

// ================= BUTTON HANDLING (Core 1) =================
void handleButtons() {
  // BTN_PLAY: Play/Pause
  if (!digitalRead(BTN_PLAY)) {
    isPlaying = !isPlaying;
    if (isPlaying) {
      playMidiFile("/0.mid");
    } else {
      stopMidiFile();
    }
    delay(300);
  }
  
  // BTN_NEXT: Next Song
  if (!digitalRead(BTN_NEXT)) {
    songIndex++;
    delay(300);
  }
  
  // BTN_PREV: Previous Song
  if (!digitalRead(BTN_PREV)) {
    if (songIndex > 0) songIndex--;
    delay(300);
  }
  
  // BTN_UP: Increase Tempo/Volume
  if (!digitalRead(BTN_UP)) {
    if (volumeMode) {
      setVolume(getVolume() + 5);
    } else {
      tempo += 10;
    }
    delay(300);
  }
  
  // BTN_DOWN: Decrease Tempo/Volume
  if (!digitalRead(BTN_DOWN)) {
    if (volumeMode) {
      setVolume(getVolume() - 5);
    } else {
      if (tempo > 0) tempo -= 10;
    }
    delay(300);
  }
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  
  // Pin Configuration
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  // Hardware Initialization
  initLED();
  initDisplay();
  SD.begin(5);

  // Start Audio Task on Core 0
  xTaskCreatePinnedToCore(
    audioTask,      // Task function
    "AudioTask",    // Task name
    10000,          // Stack size
    NULL,           // Parameters
    1,              // Priority
    &AudioTaskHandle, // Task handle
    0               // Core 0
  );

  Serial.println("Setup Complete!");
}

// ================= MAIN LOOP (Core 1) =================
void loop() {
  // Handle button input
  handleButtons();
  
  // Display animation based on status
  if (!isPlaying) {
    animateIdleLED();
  } else {
    showLEDAnimation(2); // Red pulse when playing
  }
  
  // Update OLED display
  updateDisplay();
  
  delay(10); // Small delay to prevent overwhelming Core 1
}
