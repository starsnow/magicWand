// ----------------------------------------
// ShiningMode.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-20
// 功  能：闪烁模式（仙女魔法棒的那种星星点点闪烁的感觉）
// ----------------------------------------

#ifndef _SHINING_MODE_H_
#define _SHINING_MODE_H_

#include <arduino.h>
#include "FastLED.h"
#include "BaseRenderMode.h"

class Glitter
{
public:
    int pos;
    int speed;
    uint8_t life;
    uint8_t fadefactor;

    Glitter()
    {
        pos = -1;
        speed = 0;
        life = 0;
        fadefactor = 0;
    }

    void move()
    {
        if (life <= 0)
            return;
            
        // pos -= speed;
        --life;
    }

    void renew(int pos, int speed, uint8_t life)
    {
        this->pos = pos;
        this->speed = speed;
        this->life = life;
        // fadefactor = 255 - (255 / life);
        fadefactor = 255 / life;
    }
};

class ShiningMode : public BaseRenderMode
{
private:
    unsigned int    ledNum;
    CRGB            *leds;
    CLEDController  *controller;
    uint8_t         chanceOfGlitter;
    unsigned int    headPoints;
    Glitter         *glitters;
    unsigned int    glitterNum;

public:
    ShiningMode(CLEDController *controller, uint8_t chanceOfGlitter = 80)
    {
        this->controller = controller;
        leds = controller->leds();
        ledNum = controller->size();
        this->chanceOfGlitter = chanceOfGlitter;

        headPoints = ledNum > 100 ? 10 : ledNum / 10;   // 以十分之一的 LED 为头部（如果灯带长超过 100 个 LED 则只取十个）
        // glitterNum = ledNum / 2;
        glitterNum = 15;
        glitters   = new Glitter [glitterNum];
    }

    ~ShiningMode()
    {
        delete [] glitters;
    }

    uint8_t getID()  
    {
        return 2;
    }

    void addGlitter()
    {
        if(! (random8(100) < chanceOfGlitter))
            return;

        for (uint16_t i = 0; i < glitterNum; ++i)
        {
            if (glitters[i].life > 0)
            {
                continue;
            }

            glitters[i].renew(random16(ledNum - headPoints), random8(1, 4), random8(10, 30));
            leds[glitters[i].pos] = CRGB::Gold;
            break;
        }
    }

    void render() 
    {
        fadeToBlackBy(leds, ledNum - headPoints, 50);
        for (uint16_t i = 0; i < glitterNum; ++i)
        {
            if (glitters[i].life <= 0)
                continue;
            
            glitters[i].move();
            // leds[glitters[i].pos].fadeToBlackBy(glitters[i].fadefactor);
            // leds[glitters[i].pos] = CRGB::Gold;
        }

        addGlitter();
    }

    unsigned int getRenderInterval()
    {
        return 80;
    }

    CRGB calcColor(int index)
    {
        // 灯带安装是反过来的，所以头部其实在尾部
        if (index >= headPoints)
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
        }
    }

    void exit()
    {

    }

};

#endif
