// Library for rgb state leds on the main board
#include <NeoPixelBus.h>

const uint16_t PixelCount = 2;  // there are two rgb led on the main board
const uint8_t PixelPin = 42;    // make sure to set this to the correct pin

// Color saturation value, range 1:128
#define max 128

// this initialization works
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);


RgbColor red(max, 0, 0);
RgbColor green(0, max, 0);
RgbColor blue(0, 0, max);

RgbColor cyan(0, max, max);
RgbColor magenta(max, 0, max);
RgbColor yellow(max, 200, 0);

RgbColor orange(max, 30, 0);
RgbColor purple(45, 0, max);
RgbColor springreen(0, max, max / 2);

RgbColor white(max);
RgbColor black(0);


void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;  // wait for serial attach

  Serial.println();
  Serial.println("Initializing...");
  Serial.flush();

  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();


  Serial.println();
  Serial.println("Running...");
}

void loop() {
  colortest();
}

void lightup(RgbColor color0, RgbColor color1) {
  strip.SetPixelColor(0, color0);
  strip.SetPixelColor(1, color1);
  strip.Show();
}

void turnoff() {
  // turn off the pixels
  strip.SetPixelColor(0, black);
  strip.SetPixelColor(1, black);
  strip.Show();
}

void colortest(){
  Serial.println("Starting...");
  lightup(red, red);
  delay(2000);
  lightup(green, green);
  delay(2000);
  lightup(blue, blue);
  delay(2000);
  lightup(cyan, cyan);
  delay(2000);
  lightup(magenta, magenta);
  delay(2000);
  lightup(yellow, yellow);
  delay(2000);
  lightup(purple, purple);
  delay(2000);
  lightup(orange, orange);
  delay(2000);

  Serial.println("Off ...");
  turnoff();
  delay(2000);
}
