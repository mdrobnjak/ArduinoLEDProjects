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
int iloops = 0, jloops = 0;
int delayMs = 100;
int j = 0;
int iSpeed = 1, jSpeed = 1;

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

void checkForInput()
{
  if (Serial.available() > 0)
  {
    char z = Serial.read();
    if(z == 'b')
    {
      iSpeed = 3;
      iloops = 1;
    }
    else
    {
      jSpeed = 3;
      jloops = 1;
    }
  }
  if (iloops > 0)
  {
    iloops--;
  }
  else
  {
    //delayMs++;
    iSpeed = 1;
  }
  if(jloops > 0)
  {
    jloops--;
  }
  else
  {
    jSpeed = 1;
  }
}

void loop() {
  static uint8_t hue = 160;
  Serial.print("x");
  // First slide the led in one direction
  for (int i = 0 ; ; ) {
    checkForInput();
    // Set the i'th led to red
    leds[i] = CHSV(hue, 255, 255);
    leds[j] = CHSV(hue-60, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();

    i = i >= NUM_LEDS - 1 ? 0 : i + iSpeed;
    j = j >= NUM_LEDS - 1 ? 0 : j + jSpeed;

    // Wait a little bit before we loop around and do it again
    delay(delayMs);
  }
  Serial.print("x");

  // Now go in the other direction.
  //  for (int i = (NUM_LEDS) - 1; i >= 0; i--) {
  //    checkForInput();
  //    // Set the i'th led to red
  //    leds[i] = CHSV(hue, 255, 255);
  //    // Show the leds
  //    FastLED.show();
  //    // now that we've shown the leds, reset the i'th led to black
  //    // leds[i] = CRGB::Black;
  //    fadeall();
  //    // Wait a little bit before we loop around and do it again
  //    delay(delayMs);
  //  }
}
