// ----------------------------------------
// gravityGesture.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-16
// 功  能：分析重力传感器的数据，判断动作
// 传感器的逻辑方向（物理方向有变化，内部通过函数封装调整）：
//         z  ^ y
//          \ |
//            +--> x
// ----------------------------------------

#ifndef _GRAVITY_GESTURE_H_
#define _GRAVITY_GESTURE_H_

#include "gyro.h"

inline int8_t sign(int x)
{
    return x > 0 ? 1 : (x < 0 ? -1 : 0);
}


class GravityGesture
{
public:
    enum AXIS_DIRECTION { AXIS_X, AXIS_MINUS_X, AXIS_Y, AXIS_MINUS_Y, AXIS_Z, AXIS_MINUS_Z };

private:
    const unsigned int CHECK_INTERVAL         = 10;     // 检测间隔
    const unsigned int THRESHOLD_CACHE_TIME   = 100;    // 缓存时间，有数值超过阈值时，在这段时间内不管怎么变化都认为超过阈值了
    const unsigned int TRIGGER_INTERVAL_LIMIT = 500;    // 事件触发间隔限制，一定间隔内不重复触发（也避免同一个摇动触发两个互相反方向的事件）
    
    Gyro *gyro;
    unsigned int threshold;
    AXIS_DIRECTION dirUp, dirForward, dirRight;
    
    void (*cbShakeUp)();
    void (*cbShakeDown)();
    void (*cbShakeLeft)();
    void (*cbShakeRight)();
    void (*cbShakeForward)();
    void (*cbShakeBack)();
    void (*cbCircled)();

private:
    int getGyroAcc(AXIS_DIRECTION dir);
    void checkShakeEvent();
    void checkCircledEvent();
    
public:   
    GravityGesture();
    void init(Gyro *gyro, unsigned int threshold = 15);
    void setDirection(AXIS_DIRECTION up, AXIS_DIRECTION forward, AXIS_DIRECTION right);
    void update();
    void registerShakeUpCallback(void (*cbShakeUp)());
    void registerShakeDownCallback(void (*cbShakeDown)());
    void registerShakeLeftCallback(void (*cbShakeLeft)());
    void registerShakeRightCallback(void (*cbShakeRight)());
    void registerShakeForwardCallback(void (*cbShakeForward)());
    void registerShakeBackCallback(void (*cbShakeBack)());
    void registerCircledCallback(void (*cbCircled)());
    bool isMoving();
    bool isMovingUp();
    bool isMovingDown();
    bool isMovingLeft();
    bool isMovingRight();
    bool isMovingForward();
    bool isMovingBack();
};

#endif