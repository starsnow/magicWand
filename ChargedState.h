// ----------------------------------------
// ChargedState.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-16
// 功  能：聚能状态
// ----------------------------------------

#ifndef _CHARGED_STATE_H_
#define _CHARGED_STATE_H_

#include "MagicWand.h"
#include "BaseState.h"
#include "gyro.h"
#include "debug.h"
#include <arduino.h>

void cbShakeUp();
void cbShakeDown();
void cbShakeLeft();
void cbShakeRight();

class ChargedState : public BaseState
{
private:
    MagicWand *wand;
    uint8_t threshold;
    unsigned long enterTime;
    const unsigned int TIMEOUT = 5000;

public:
    ChargedState(MagicWand* wand, uint8_t threshold = DEFAULT_THRESHOLD)
    {
        this->wand = wand;
        this->threshold = threshold;
        enterTime = 0;
    }

    uint8_t getID()
    {
        return 3;
    }

    void update()
    {
        if (millis() - enterTime >= TIMEOUT)
        {
            wand->states->switchState(wand->states->sleeping);           
        }
    }

    void enter()
    {
        // 设置灯效
        DEBUG_OUTLN("enter charged state");
        enterTime = millis();

        wand->gesture->registerShakeUpCallback(cbShakeUp);
        wand->gesture->registerShakeDownCallback(cbShakeDown);
        wand->gesture->registerShakeLeftCallback(cbShakeLeft);
        wand->gesture->registerShakeRightCallback(cbShakeRight);

        wand->strip->setRenderMode(WS2812Strip::ENERGIC_MODE);
    }

    void exit()
    {
        DEBUG_OUTLN("exit charged state");
        wand->gesture->registerShakeUpCallback(nullptr);
        wand->gesture->registerShakeDownCallback(nullptr);
        wand->gesture->registerShakeLeftCallback(nullptr);
        wand->gesture->registerShakeRightCallback(nullptr);
    }

};

#endif