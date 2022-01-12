// ----------------------------------------
// WS2812.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：2812 灯带效果实现
// ----------------------------------------

#ifndef _WS2812_H_
#define _WS2812_H_

#include <arduino.h>
#include <fastLED.h>

// FastLED 相关参数
#define INIT_BRIGHTNESS     50
#define CHIPSET             WS2812B
#define COLOR_ORDER         GRB
#define LED_NUM             10

// 输入
enum Input 
{
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

// 点阵显示模式
enum RENDER_MODE 
{
    EMPTY,              // 什么也没有
    RAINBOW,            // 彩虹
    COLOURFUL_DREAM,    // 五彩梦幻
    STAR_SKY,           // 星空
    HACKER_MATRIX,      // 黑客帝国
    RAIN,               // 雨
    BUBBLE,             // 气泡
    SNOW,               // 雪
    ENERGY_CUBE,        // 能量魔方
    SNAKE_GAME          // 贪吃蛇游戏
};

// 坐标点
class Point
{
public:
    int8_t x, y;

    Point(int8_t _x = -1, int8_t _y = -1)
    {
        x = _x;
        y = _y;
    };

    bool operator== (const Point &p)
    {
        return (p.x == x) && (p.y == y);
    }
};

void turnOnWS2812();
void turnOffWS2812();
/*
void initWS2812();
void updateWS2812();
void turnOnWS2812();
void turnOffWS2812();
void randomDot(CRGB pLeds[], const CRGB color);
void setRenderMode(enum RENDER_MODE mode);
void inputDir(uint8_t dir);
*/

#endif
