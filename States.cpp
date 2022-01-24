// ----------------------------------------
// States.cpp
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-14
// 功  能：状态集合类的实现
// ----------------------------------------

#include "States.h"
#include "SleepingState.h"
#include "WakingState.h"
#include "ChargedState.h"
#include "ShakingState.h"

States::States(MagicWand *wand)
{
    this->wand = wand;
    state = nullptr;
}

void States::init()
{
    sleeping     = new SleepingState(wand);
    waking       = new WakingState(wand);
    charged      = new ChargedState(wand);
    shakingUp    = new ShakingState(wand, ShakingState::UP);
    shakingDown  = new ShakingState(wand, ShakingState::DOWN);
    shakingLeft  = new ShakingState(wand, ShakingState::LEFT);
    shakingRight = new ShakingState(wand, ShakingState::RIGHT);
}

States::~States()
{
    delete sleeping;
    delete waking;
    delete charged;
    delete shakingUp;
    delete shakingDown;
    delete shakingLeft;
    delete shakingRight;
}

void States::update()
{
    // DEBUG_OUT("update: ");
    if (state != nullptr)
    {
        // DEBUG_OUTLN("state id :" + (int) state->getID());
        // Serial.println(state->getID(), HEX);
        state->update();
    }
}

void States::switchState(BaseState *newState)
{
    if (state != nullptr)
    {
        state->exit();
    }
    
    state = newState;
    if (state == nullptr)
    {
        return;
    }

    state->enter();
}