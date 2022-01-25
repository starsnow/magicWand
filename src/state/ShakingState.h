// ----------------------------------------
// shakingState.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-18
// 功  能：摇动的状态
// ----------------------------------------

#ifndef _SHAKING_STATE_H_
#define _SHAKING_STATE_H_

#include "../MagicWand.h"
#include "BaseState.h"
#include "../GyroGesture/gyro.h"
#include "../debug.h"
#include <arduino.h>

class ShakingState : public BaseState
{
public: 
    enum DIRECTION { UP, DOWN, LEFT, RIGHT };

private:
    MagicWand *wand;
    unsigned long enterTime;
    const unsigned int TIMEOUT = 1000;
    DIRECTION dir;

public:
    ShakingState(MagicWand* wand, DIRECTION dir)
    {
        this->wand = wand;
        this->dir  = dir;
        enterTime  = 0;
    }

    void update()
    {
        if (millis() - enterTime >= TIMEOUT)
        {
            wand->states->switchState(wand->states->waking);           
        }
    }

    void enter()
    {
        // 设置灯效
        DEBUG_OUTLN("enter shaking state");
        enterTime = millis();
    }

    void exit()
    {
        
    }

};

#endif