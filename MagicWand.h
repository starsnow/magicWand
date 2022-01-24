// ----------------------------------------
// MagicWand.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-13
// 功  能：魔法棒对象
// ----------------------------------------

#ifndef _MAGIC_WAND_H_
#define _MAGIC_WAND_H_

#include "gravityGesture.h"
#include "WS2812Strip.h"

class Gyro;
class States;

class MagicWand
{
private:

public:
    States *states;
    Gyro *gyro;
    GravityGesture *gesture;
    WS2812Strip *strip;

    MagicWand();
    ~MagicWand();
    void init();
    void update();
};

#endif