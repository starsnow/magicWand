// ----------------------------------------
// BreathingMode.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-19
// 功  能：呼吸模式
// ----------------------------------------

#ifndef _BREATHING_MODE_H_
#define _BREATHING_MODE_H_

#include "BaseRenderMode.h"

class BreathingMode : public BaseRenderMode
{
private:
    unsigned int    ledNum;
    CRGB            *leds;
    CLEDController  *controller;
    uint8_t         maxBrightness;
    uint8_t         originalBrightness;
    CRGBPalette16   breathingModeColorPallette;

public:
    BreathingMode(CLEDController *controller, uint8_t maxBrightness = 100);
    uint8_t getID();
    CRGB calcColor(int index);
    void render();
    unsigned int getRenderInterval();
    void init();
    void enter();
    void exit();
};

#endif
