// ----------------------------------------
// ShootingMode.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-20
// 功  能：发射模式
// ----------------------------------------

#ifndef _SHOOTING_MODE_H_
#define _SHOOTING_MODE_H_

#include <arduino.h>
#include "FastLED.h"
#include "BaseRenderMode.h"

class ShootingMode : public BaseRenderMode
{
private:
    unsigned int    ledNum;
    CRGB            *leds;
    CLEDController  *controller;
    uint8_t         maxBrightness;
    CRGB            color;
    uint8_t         speed;
    uint8_t         bulletPos;

public:
    ShootingMode(CLEDController *controller, const CRGB &color, uint8_t speed, uint8_t maxBrightness = 100)
    {
        this->controller = controller;
        leds = controller->leds();
        ledNum = controller->size();
        this->maxBrightness = maxBrightness;
        this->color = color;
        this->speed = speed;
        bulletPos = 0;
    }

    uint8_t getID()  
    {
        return 2;
    }

    void render() 
    {
        fadeToBlackBy(leds, ledNum, 50);
        if (bulletPos > ledNum)
            return;

        leds[bulletPos] = color;
        bulletPos += speed;
    }

    unsigned int getRenderInterval()
    {
        return 5;
    }

    void init()
    {
        enter();
    }

    void enter()
    {
        bulletPos = 0;
    }

    void exit()
    {
    }

};

#endif
