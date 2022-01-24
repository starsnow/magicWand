// ----------------------------------------
// MagicWand.cpp
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-14
// 功  能：魔法棒
// ----------------------------------------

#include "BaseState.h"
#include "States.h"
#include "gyro.h"
#include "gravityGesture.h"
#include "MagicWand.h"
#include "debug.h"
#include "WS2812Strip.h"

MagicWand::MagicWand()
{
    gyro = new Gyro();
    states = new States(this);
    gesture = new GravityGesture();
    strip = new WS2812Strip();
}

MagicWand::~MagicWand()
{
    delete states;
    delete gyro;
    delete gesture;
    delete strip;
}

void MagicWand::init()
{
    gyro->init();
    gesture->init(gyro);
    gesture->setDirection(GravityGesture::AXIS_Z, GravityGesture::AXIS_MINUS_Y, GravityGesture::AXIS_MINUS_X);
    states->init();
    states->switchState((BaseState *) states->sleeping);
    strip->init(48, 50);
    strip->setRenderMode(WS2812Strip::BREATHING_MODE);
}

void MagicWand::update()
{
    // DEBUG_OUTLN("wand update");
    gyro->update();
    gesture->update();
    states->update();
    strip->update();
}