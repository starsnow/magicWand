// ----------------------------------------
// BreathingMode.cpp
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-24
// 功  能：呼吸模式
// ----------------------------------------

#include <arduino.h>
#include "FastLED.h"
#include "BreathingMode.h"

DEFINE_GRADIENT_PALETTE(breathingMode_gp) {
    0,   0x00, 0x00, 0x00,      // 黑
    2,   0x88, 0x88, 0x88,      // 想要的颜色写这里 RGB
    220, 0x00, 0x00, 0x00,      // 黑
    255, 0x00, 0x00, 0x00       // 黑
};

BreathingMode::BreathingMode(CLEDController *controller, uint8_t maxBrightness = 100)
{
    this->controller = controller;
    leds = controller->leds();
    ledNum = controller->size();
    this->maxBrightness = maxBrightness;
    breathingModeColorPallette = breathingMode_gp;
}

uint8_t BreathingMode::getID()  
{
    return 1;
}

CRGB BreathingMode::calcColor(int index)
{
    int colorIndex = map(abs(index - ((int)ledNum + 1) / 2), 0, ((int)ledNum + 1) / 2, 15, 240);
    return ColorFromPalette(breathingModeColorPallette, colorIndex);
}

void BreathingMode::render() 
{
    FastLED.setBrightness(scale8(maxBrightness, beatsin8(10, 0, 150)));
}

unsigned int BreathingMode::getRenderInterval()
{
    return 10;
}

void BreathingMode::init()
{
    enter();
}

void BreathingMode::enter()
{
    originalBrightness = FastLED.getBrightness();
    
    for (uint16_t i = 0; i < ledNum; ++i)
    {
        leds[i] = calcColor(i);
    }
}

void BreathingMode::exit()
{
    FastLED.setBrightness(originalBrightness);
}