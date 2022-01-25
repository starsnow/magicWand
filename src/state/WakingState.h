// ----------------------------------------
// WakingState.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-14
// 功  能：唤醒状态
// ----------------------------------------

#ifndef _WAKING_STATE_H_
#define _WAKING_STATE_H_

#include "../MagicWand.h"
#include "BaseState.h"
#include "../GyroGesture/gyro.h"
#include "../GyroGesture/gravityGesture.h"
#include "../debug.h"
#include <arduino.h>

void cbCircled();

class WakingState : public BaseState
{
private:
    MagicWand *wand;
    uint8_t threshold;
    unsigned long activeTime;
    unsigned long lastCheckTime;
    const unsigned int TIMEOUT = 3000;          // 超时时间
    const unsigned int CHECK_INTERVAL = 100;    // 检测动作的间隔时间

public:
    WakingState (MagicWand* wand, uint8_t threshold = DEFAULT_THRESHOLD)
    {
        this->wand = wand;
        this->threshold = threshold;
        activeTime = 0;
        lastCheckTime = 0;
    }

    uint8_t getID() 
    {
        return 2;
    }

    void update()
    {
        // 超时回到睡眠状态
        if (millis() - activeTime >= TIMEOUT)
        {
            wand->states->switchState(wand->states->sleeping);
        }
    
        // 检测是否活跃
        if (! wand->gyro->isMoving(threshold))
        {
            return;
        }
        
        // 更新活跃时间
        activeTime = millis();

        // // 在一定间隔的时间做方向判断
        // if (millis() - lastCheckTime < CHECK_INTERVAL)
        //     return;

        // lastCheckTime = miliis();

        // // 判断是否是画圈动作
        // // MPU6050 安装方向如下：
        // // x <---+       ^ 前
        // //       |       |   
        // //       v y     |       
        // // 判断从上开始向右向下向左向上的画圈动作
        // // y 先减小，再增大，再减小
        // // x 先
    }

    void enter()
    {
        // 设置灯效
        DEBUG_OUTLN("enter waking state");
        activeTime = millis();

        wand->gesture->registerCircledCallback(cbCircled);
        wand->strip->setRenderMode(WS2812Strip::SHINING_MODE);
    }

    void exit()
    {
        wand->gesture->registerCircledCallback(nullptr);
    }
};

#endif