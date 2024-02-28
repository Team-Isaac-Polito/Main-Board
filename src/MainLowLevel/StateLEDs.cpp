#include "StateLEDs.h"

// NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

StateLEDs::StateLEDs(const uint16_t PixelCount, const uint8_t PixelPin)
    : PixelCount(PixelCount), PixelPin(PixelPin)
{
}

void StateLEDs::begin()
{
    strip(this.PixelCount, this.PixelPin);
    strip.Begin();
    strip.Show(); // this resets all the neopixels to an off state
}

void StateLEDs::lightup(RgbColor color0, RgbColor color1)
{
    strip.SetPixelColor(0, color0);
    strip.SetPixelColor(1, color1);
    strip.Show();
}

void StateLEDs::turnoff()
{
    // turn off the pixels
    strip.SetPixelColor(0, black);
    strip.SetPixelColor(1, black);
    strip.Show();
}

void StateLEDs::visualMotors(short int left, short int right)
{
    left = left/8;
    right = right/8;
    if(left<0){
        RgbColor leftc(left,0,0); //red scale if going backward
    }else{
        RgbColor leftc(0,leftc,0);
    }
    if(right<0){
        RgbColor rightc(right,0,0); //green scale if moving forward
    }else{
        RgbColor rightc(0,right,0)
    }
    lightup(leftc, rightc);
    
}
