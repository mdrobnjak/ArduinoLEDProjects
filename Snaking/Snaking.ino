#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 150

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 5
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];

int delayMs = 1000;
int loops = 0;
static uint8_t hue = 160;
static uint8_t hueIncrement = 20;
int iL, iLPrev = 9999, iLSkip = 1;
int brightness = 30;
char z;

void setup() {
  Serial.begin(57600);
  Serial.println("resetting");
  LEDS.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(brightness);
}

void Dim(int dimBy)
{
  if (brightness > dimBy)
  {
    brightness -= dimBy;
    LEDS.setBrightness(brightness);
  }
}

void Brighten(int newBrightness)
{
  brightness = newBrightness;
  LEDS.setBrightness(brightness);
}

void ExecuteSkip()
{
  if (iL + iLSkip < NUM_LEDS && iL + iLSkip > 0)
  {
    iL += iLSkip;
  }
}

void ClearSerialBuffer()
{
  while (Serial.available() > 0) z = Serial.read();
}

void CheckForInput()
{
  z = '0';
  if (Serial.available() > 0)
  {
    z = Serial.read();
  }
}

void ProcessInput()
{
  switch (z)
  {
    case '0':
      if (loops > 0) loops--;
      else
      {
        delayMs += 25;
        Dim(10);
      }
      break;
    case 'b':
      ExecuteSkip(); //Skip ahead.

      Brighten(180);

      hue += hueIncrement;
      delayMs = 5;
      loops = 1;
      break;
    case 'm':
      Brighten(240);
      while (true) //Stop moving.
      {
        FastLED.show();
        
        Dim(3);

        if (Serial.available() > 0)z = Serial.read();

        if (z == 'b')
        {
          ProcessInput();
          return;
        }

        if (z == 'm')Brighten(180);

        z = '0';
      }
      break;
    default:
      break;
  }
  ClearSerialBuffer();
}

void loop() {

  iLPrev = 9999;

  for (iL = 0; iL < NUM_LEDS; iL++ ) {
    CheckForInput();
    ProcessInput();

    leds[iL] = CHSV(hue, 255, 255);

    leds[iLPrev].fadeToBlackBy(255);
    iLPrev = iL;

    FastLED.show();

    FastLED.delay(delayMs);
  }

  for (iL = (NUM_LEDS) - 1; iL >= 0; iL--) {
    CheckForInput();
    ProcessInput();

    leds[iL] = CHSV(hue, 255, 255);

    leds[iLPrev].fadeToBlackBy(255);
    iLPrev = iL;

    FastLED.show();

    FastLED.delay(delayMs);
  }
}
