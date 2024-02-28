// Library for rgb state leds on the main board
#include <NeoPixelBus.h>

// Color saturation value, range 1:128
#define max 128

//Color definitions
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

/**
  RGB LEDs class used to drive RGB LED strip
*/
class StateLEDs{
  public: 
    StateLEDs(const uint16_t PixelCount = 2, const uint8_t PixelPin = 42);
    void begin();
    void lightup(RgbColor color0, RgbColor color1);
    void turnoff();
    void visualMotors(short int left, short int right);


  private:
    const uint16_t PixelCount;  // there are two rgb led on the main board
    const uint8_t PixelPin;    // make sure to set this to the correct pin
    NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip; // this initialization works


}
