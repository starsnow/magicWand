// ----------------------------------------
// SleepingState.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-13
// 功  能：休眠状态
// ----------------------------------------

#ifndef _SLEEP_STATE_H_
#define _SLEEP_STATE_H_

#include "MagicWand.h"
#include "BaseState.h"
#include "gyro.h"
#include "States.h"
#include "debug.h"

class SleepingState : public BaseState
{
private:
    MagicWand *wand;
    uint8_t threshold;

public:
    SleepingState (MagicWand* wand, uint8_t threshold = DEFAULT_THRESHOLD)
    {
        this->wand = wand;
        this->threshold = threshold;
    }

    uint8_t getID() 
    {
        return 1; 
    }

    void update()
    {
        if (wand->gyro->isMoving(threshold))
        {
            wand->states->switchState((BaseState*) wand->states->waking);
        }
    }

    void enter()
    {
        // 设置灯效
        DEBUG_OUTLN("enter sleeping state");
        wand->strip->setRenderMode(WS2812Strip::BREATHING_MODE);
    }
};

#endif