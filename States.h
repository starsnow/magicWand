// ----------------------------------------
// States.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-13
// 功  能：状态集合
// ----------------------------------------

#ifndef _STATES_H_
#define _STATES_H_

#include "BaseState.h"
// #include "gyro.h"
#include "MagicWand.h"

class SleepingState;
class WakingState;
class ChargedState;
class ShakingState;

class States
{
private:
    MagicWand *wand;
    BaseState *state;

public:
    SleepingState   *sleeping;
    WakingState     *waking;
    ChargedState    *charged;
    ShakingState    *shakingUp;
    ShakingState    *shakingDown;
    ShakingState    *shakingLeft;
    ShakingState    *shakingRight;

    States(MagicWand *wand);    
    ~States();
    void init();
    void update();
    void switchState(BaseState *newState);
};

#endif