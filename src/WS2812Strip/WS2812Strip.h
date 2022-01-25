// ----------------------------------------
// WS2812Strip.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：2812 灯带效果实现
// ----------------------------------------

#ifndef _WS2812_STRIP_H_
#define _WS2812_STRIP_H_

#include "../debug.h"
#include <arduino.h>
#include <FastLED.h>
#include "BaseRenderMode.h"
#include "BreathingMode.h"
#include "ShiningMode.h"
#include "EnergicMode.h"
#include "ShootingMode.h"

// FastLED 相关参数
#define INIT_BRIGHTNESS     50
#define CHIPSET             WS2812B
#define COLOR_ORDER         GRB
#define DATA_PIN            9

class WS2812Strip
{
public:
    enum RENDER_MODE { 
        BREATHING_MODE = 0,
        SHINING_MODE,
        ENERGIC_MODE,
        SHOOTING_MODE_LEFT,
        SHOOTING_MODE_RIGHT,
        SHOOTING_MODE_UP,
        SHOOTING_MODE_DOWN 
    };

private:
    unsigned int    ledNum;
    CRGB            *leds;
    CLEDController  *controller;
    unsigned int    renderInterval;
    BaseRenderMode  *renderModePtr;
    uint8_t         pin;
    uint8_t         brightness;

    BreathingMode   *breathingMode;
    ShiningMode     *shiningMode;
    EnergicMode     *energicMode;
    ShootingMode    *shootingModeLeft;
    ShootingMode    *shootingModeRight;
    ShootingMode    *shootingModeUp;
    ShootingMode    *shootingModeDown;

public:
    WS2812Strip()
    {
        leds = nullptr;
        controller = nullptr;
        renderModePtr = nullptr;
        renderInterval = 999;
        brightness = INIT_BRIGHTNESS;
        pin = 0;
    }

    // 这个数据信号引脚 FastLED 要求必须是常量，所以就不能用参数了
    WS2812Strip(/* uint8_t controlPin,*/uint16_t ledNum, uint8_t brightness = INIT_BRIGHTNESS)
    {
        init(/*controlPin, */ledNum, brightness);
    }

    ~WS2812Strip()
    {
        delete breathingMode;
        delete shiningMode;

        delete [] leds;
    }

    void init(/* uint8_t controlPin,*/uint16_t ledNum, uint8_t brightness = INIT_BRIGHTNESS)
    {
        // pin = controlPin;
        this->ledNum = ledNum;
        this->brightness = brightness;

        leds = new CRGB [ledNum];
        controller = &FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, ledNum);
        renderModePtr = nullptr;
        controller->setDither(DISABLE_DITHER);

        breathingMode = new BreathingMode(controller, 80);
        breathingMode->init();

        shiningMode = new ShiningMode(controller);
        shiningMode->init();

        energicMode = new EnergicMode(controller);
        energicMode->init();

        shootingModeUp = new ShootingMode(controller, CRGB::Green, 1);
        shootingModeUp->init();

        shootingModeDown = new ShootingMode(controller, CRGB::Blue, 1);
        shootingModeDown->init();

        shootingModeLeft = new ShootingMode(controller, CRGB::Orange, 1);
        shootingModeLeft->init();

        shootingModeRight = new ShootingMode(controller, CRGB::Red, 1);
        shootingModeRight->init();
    }

    void setRenderMode(RENDER_MODE mode)
    {
        DEBUG_OUTLN((int) mode);
        BaseRenderMode *newModePtr = nullptr;
        switch (mode)
        {
            case BREATHING_MODE:        newModePtr = breathingMode;      break;
            case SHINING_MODE:          newModePtr = shiningMode;        break;    
            case ENERGIC_MODE:          newModePtr = energicMode;        break;
            case SHOOTING_MODE_UP:      newModePtr = shootingModeUp;     break;
            case SHOOTING_MODE_DOWN:    newModePtr = shootingModeDown;   break;
            case SHOOTING_MODE_LEFT:    newModePtr = shootingModeLeft;   break;
            case SHOOTING_MODE_RIGHT:   newModePtr = shootingModeRight;  break;
        };

        if (renderModePtr != nullptr)
        {
            renderModePtr->exit();
        }

        renderModePtr = newModePtr;
        renderInterval = newModePtr->getRenderInterval();
        renderModePtr->enter();
    }

    void update()
    {
        static unsigned long lastTick = 0;
        if (millis() - lastTick < renderInterval)
            return;
        
        lastTick = millis();

        if (renderModePtr == nullptr)
            return;

        renderModePtr->render();
        FastLED.show();
    }
};

#endif
