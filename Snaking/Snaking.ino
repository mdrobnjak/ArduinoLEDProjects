#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 144

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];
int delayMs = 1000, loops = 0;
static uint8_t hue, hueIncrement = 20;
int iL, iLSkip = 2;

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(255);
  }
}

void checkForInput(int inc)
{
  char z;
  if(loops > 0)
  {
    loops--;
    while(Serial.available() > 0) z = Serial.read();
  }
  else if (Serial.available() > 0)
  {
    iL += inc;
    z = Serial.read();
    hue += hueIncrement;
    delayMs = 5;
    loops = 1;
  }
  else
  {
    delayMs +=25;
  }
}

void loop() {
  hue = 160;
  // First slide the led in one direction
  for (iL = 0; iL < NUM_LEDS && iL >= 0; iL++ ) {
    checkForInput(iLSkip);
    // Set the i'th led to red
    leds[iL] = CHSV(hue, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(delayMs);
  }
  Serial.print("x");

   //Now go in the other direction.
    for (iL = (NUM_LEDS) - 1; iL >= 0 &&  iL < NUM_LEDS ; iL--) {
      checkForInput(-iLSkip);
      // Set the i'th led to red
      leds[iL] = CHSV(hue, 255, 255);
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      delay(delayMs);
    }
}
