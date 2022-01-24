// ----------------------------------------
// magicWand.ino
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：涂鸦魔法棒
// ----------------------------------------

#include "debug.h"
#include <FastLED.h>
#include "tuyaDefination.h"
#include "MagicWand.h"
#include "States.h"
#include "WS2812Strip.h"

MagicWand *wand;

const uint8_t WIFI_RECONNECT_BUTTON_PIN = 3;
const uint8_t WIFI_LED = LED_BUILTIN;

unsigned char pid[] = {"pzyjsm1fjgcctupg"}; //*********处替换成涂鸦IoT平台自己创建的产品的PID
unsigned char mcu_ver[] = {"1.0.0"};

void setup()
{
    DEBUG_INIT(9600);

#ifdef ARDUINO_AVR_MICRO
    Serial1.begin(9600);
#else
    Serial.begin(9600);
#endif

    tuya_setup(pid, mcu_ver, WIFI_RECONNECT_BUTTON_PIN, WIFI_LED);

    wand = new MagicWand();
    wand->init();
}

void loop()
{
    tuya_loop();
    wand->update();
}

// 一些回调函数，随便放这里

void cbShakeUp()
{
    DEBUG_OUTLN("shake up!");
    wand->states->switchState((BaseState *) wand->states->shakingUp);
    wand->strip->setRenderMode(WS2812Strip::SHOOTING_MODE_UP);
    upload_gesture_value(0);
}

void cbShakeDown()
{
    DEBUG_OUTLN("shake down!");
    wand->states->switchState((BaseState *) wand->states->shakingDown);
    wand->strip->setRenderMode(WS2812Strip::SHOOTING_MODE_DOWN);
    upload_gesture_value(1);
}

void cbShakeLeft()
{
    DEBUG_OUTLN("shake left!");
    wand->states->switchState((BaseState *) wand->states->shakingLeft);
    wand->strip->setRenderMode(WS2812Strip::SHOOTING_MODE_LEFT);
    upload_gesture_value(2);
}

void cbShakeRight()
{
    DEBUG_OUTLN("shake right!");
    wand->states->switchState((BaseState *) wand->states->shakingRight);
    wand->strip->setRenderMode(WS2812Strip::SHOOTING_MODE_RIGHT);
    upload_gesture_value(3);
}

void cbCircled()
{
    DEBUG_OUTLN("cb circled!");
    wand->states->switchState((BaseState *) wand->states->charged);
    wand->strip->setRenderMode(WS2812Strip::ENERGIC_MODE);
    upload_gesture_value(4);
}