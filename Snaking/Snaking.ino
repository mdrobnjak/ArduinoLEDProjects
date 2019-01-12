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
static uint8_t hue = 160, hueIncrement = 20;
int iL, iLPrev, iLSkip = 2;

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(30);
}

void fadeall(int iL, int numBehindiL, int inc) {
  for (int i = iL; i > iL - numBehindiL && i >= 0 && i <= NUM_LEDS; i+= inc) {
    leds[i].fadeToBlackBy(255);
  }
}

void checkForInput(int inc)
{
  char z;
  if(loops > 0)
  {
    loops--;
  }
  else if (Serial.available() > 0)
  {
    if(iL + inc < NUM_LEDS && iL + inc > 0)
    {
      iL += inc;
    }
    while(Serial.available() > 0) z = Serial.read();
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
  iLPrev = 9999;
  for (iL = 0; iL < NUM_LEDS; iL++ ) {
    checkForInput(iLSkip);
    
    leds[iL] = CHSV(hue, 255, 255);    
    
    leds[iLPrev].fadeToBlackBy(255);
    iLPrev = iL;
    
    FastLED.show();
    
    delay(delayMs);
  }
  
  for (iL = (NUM_LEDS) - 1; iL >= 0; iL--) {
    checkForInput(-iLSkip);
    
    leds[iL] = CHSV(hue, 255, 255);
    
    leds[iLPrev].fadeToBlackBy(255);
    iLPrev = iL;
    
    FastLED.show();
    
    delay(delayMs);
  }
}
