// ----------------------------------------
// EnergicMode.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-20
// 功  能：聚能模式光效，当魔棒画了一个圈后聚满了能量，等待发招
// ----------------------------------------

#ifndef _ENERGIC_MODE_H_
#define _ENERGIC_MODE_H_

#include <arduino.h>
#include "FastLED.h"
#include "BaseRenderMode.h"

class EnergyDot
{
public:
    int pos;    // 当前应用 int 就够了
    int speed;
    int middlePos;

    EnergyDot()
    {
        pos = -1;
        speed = 0;
    }

    void move()
    {
        if (pos < 0)
            return;
            
        pos += speed;
    }

    void renew(int pos, int speed)
    {
        this->pos = pos;
        this->speed = pos > middlePos ? -speed : speed;
    }
};

class EnergicMode : public BaseRenderMode
{
private:
    const uint8_t   ENERGY_AREA_LENGTH = 6;
    const uint8_t   GLITTER_BRIGHTNESS_SCALE = 200;
    unsigned int    ledNum;
    CRGB            *leds;
    CLEDController  *controller;
    uint16_t        middlePos;
    uint16_t        energyAreaStart;
    uint16_t        energyAreaEnd;
    uint8_t         initBrightnessScale;
    uint8_t         tempBrightnessScale;
    EnergyDot       *energyDots;
    uint8_t         energyDotNum;
    uint8_t         newDotChance;

public:
    EnergicMode(CLEDController *controller, uint8_t newDotChance = 40, uint8_t initBrightnessScale = 30)
    {
        this->controller = controller;
        this->newDotChance = newDotChance;
        this->initBrightnessScale = initBrightnessScale;

        leds   = controller->leds();
        ledNum = controller->size();
        middlePos = (ledNum + 1) / 2;
        
        energyAreaStart = middlePos - (ENERGY_AREA_LENGTH + 1) / 2;
        energyAreaEnd   = energyAreaStart + ENERGY_AREA_LENGTH;
        
        tempBrightnessScale = 0;

        energyDotNum = 4;
        energyDots = new EnergyDot [energyDotNum];
        for (uint8_t i = 0; i < energyDotNum; ++i)
        {
            energyDots[i].middlePos = middlePos;
        }
    }

    ~EnergicMode()
    {
        delete [] energyDots;
    }

    uint8_t getID()
    {
        return 3;
    }

    void addRandomEnergyDot()
    {
        int newPos, speed;

        if (! (random8(100) < newDotChance))
            return;

        for (uint8_t i = 0; i < energyDotNum; ++i)
        {
            if (energyDots[i].pos >= 0)
                continue;

            energyDots[i].renew(random(ledNum), random8(1, 2));
            break;
        }
    }

    void render() 
    {
        addRandomEnergyDot();

        fadeToBlackBy(leds, energyAreaStart, 80);
        fadeToBlackBy(leds + energyAreaEnd, ledNum - energyAreaEnd, 80);

        for (uint8_t i = 0; i < energyDotNum; ++i)
        {
            if (energyDots[i].pos < 0)
                continue;

            energyDots[i].move();
            if (energyDots[i].pos > energyAreaStart && energyDots[i].pos < energyAreaEnd)
            {
                tempBrightnessScale = GLITTER_BRIGHTNESS_SCALE;
                energyDots[i].pos = -1;
                continue;
            }

            leds[energyDots[i].pos] = CRGB::Gold;
            leds[energyDots[i].pos].nscale8(initBrightnessScale);
        }

        if (tempBrightnessScale > initBrightnessScale)
        {
            for (uint16_t i = energyAreaStart; i < energyAreaEnd; ++i)
            {
                leds[i] = CRGB::Gold;
                leds[i].nscale8(tempBrightnessScale);
            }

            tempBrightnessScale -= 15;
        }
    }

    unsigned int getRenderInterval()
    {
        return 80;
    }

    CRGB calcColor(int index)
    {
        // 中间做为能量区域
        if (index >= energyAreaStart && index < energyAreaEnd)
        {
            return CRGB::Gold;
        }

        return CRGB::Black;
    }

    void init()
    {
        enter();
    }

    void enter()
    {
        for (uint16_t i = 0; i < ledNum; ++i)
        {
            leds[i] = calcColor(i);
            leds[i].nscale8(initBrightnessScale);
        }
    }

    void exit()
    {

    }

};

#endif
