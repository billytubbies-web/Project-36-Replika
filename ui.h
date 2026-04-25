#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include <FastLED.h>
#include <Adafruit_SSD1306.h>

// ================= HARDWARE PIN DEFINITIONS =================
#define NUM_LEDS 36
#define LED_PIN 15
#define BTN_PLAY 33
#define BTN_NEXT 14
#define BTN_PREV 27
#define BTN_UP 12
#define BTN_DOWN 13

// ================= UI DECLARATIONS =================
extern CRGB leds[NUM_LEDS];
extern Adafruit_SSD1306 display;
extern int songIndex;
extern bool volumeMode;

// LED Functions
void initLED();
void updateLEDAnimation(bool isPlaying);

// Display Functions
void initDisplay();
void updateDisplay(bool isPlaying, int volume, int songIdx);
void clearDisplay();

#endif
