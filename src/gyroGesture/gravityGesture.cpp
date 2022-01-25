// ----------------------------------------
// gravityGesture.cpp
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-16
// 功  能：分析重力传感器的数据，判断动作
// ----------------------------------------

#include "gravityGesture.h"
#include "arduino.h"
#include "../debug.h"
#include "Smoothing.h"

GravityGesture::GravityGesture()
{

}

void GravityGesture::init(Gyro *gyro, unsigned int threshold)
{
    this->gyro = gyro;
    this->threshold = threshold;
    
    dirUp      = AXIS_Z;
    dirForward = AXIS_Y;
    dirRight   = AXIS_X;

    cbShakeUp      = nullptr;
    cbShakeDown    = nullptr;
    cbShakeLeft    = nullptr;
    cbShakeRight   = nullptr;
    cbShakeForward = nullptr;
    cbShakeBack    = nullptr;
    cbCircled      = nullptr;
}

void GravityGesture::setDirection(AXIS_DIRECTION up, AXIS_DIRECTION forward, AXIS_DIRECTION right)
{
    dirUp      = up;
    dirForward = forward;
    dirRight   = right;
}

int GravityGesture::getGyroAcc(AXIS_DIRECTION dir)
{
    switch (dir)
    {
        case AXIS_X      :  return  gyro->getXAcc();
        case AXIS_MINUS_X:  return -gyro->getXAcc();
        case AXIS_Y      :  return  gyro->getYAcc();
        case AXIS_MINUS_Y:  return -gyro->getYAcc();
        case AXIS_Z      :  return  gyro->getZAcc();
        case AXIS_MINUS_Z:  return -gyro->getZAcc();        
    }

    return 0;
}

void GravityGesture::checkShakeEvent()
{
    // 数组元素按下标 0 1 2，分别是 x y z 的数据

    static unsigned long lastTick = 0;
    static int lastThresholdAcc[3]            = {0, 0, 0}; // x y z
    static unsigned long lastThresholdTime[3] = {0, 0, 0}; // x y z
    static unsigned long lastTriggerTime[3]   = {0, 0, 0}; // x y z 上一次触发事件的时间
    void (*callBack[2][3])() =                             // 事件回调函数指针
    {
        { cbShakeRight, cbShakeForward, cbShakeUp   },     //  x  y  z
        { cbShakeLeft,  cbShakeBack,    cbShakeDown }      // -x -y -z
    };

    unsigned long curTick = millis();
    int acc[3]; // x y z

    if (curTick - lastTick < CHECK_INTERVAL)
        return;

    lastTick = curTick;
    acc[0] = getGyroAcc(dirRight);
    acc[1] = getGyroAcc(dirForward);
    acc[2] = getGyroAcc(dirUp);

    for (uint8_t i = 0; i < 3; ++i)
    {
        if (abs(acc[i]) < threshold)
            continue;

        if (curTick - lastThresholdTime[i] > THRESHOLD_CACHE_TIME || sign(acc[i]) == sign(lastThresholdAcc[i]))
        {
            // 不是同一次的摇动，或是同向的摆动，则更新“上次的摆动信息”
            lastThresholdTime[i] = curTick;
            lastThresholdAcc[i] = acc[i];
        }
        else
        {
            // 触发间隔太短，不触发事件
            if (curTick - lastTriggerTime[i] < TRIGGER_INTERVAL_LIMIT)
                continue;

            lastTriggerTime[i] = curTick;

            // 先正后负的是正向摆，反之负向摆动
            if (lastThresholdAcc[i] > 0)
            {
                if (callBack[0][i] == nullptr)
                    continue;

                // DEBUG_OUT("CB shake 0 ");
                // DEBUG_OUTLN(i);
                callBack[0][i]();
            }
            else
            {
                if (callBack[1][i] == nullptr)
                    continue;

                // DEBUG_OUT("CB shake 1");
                // DEBUG_OUTLN(i);
                callBack[1][i]();
            }
        }
    }
}

// 检测前向竖直面的画圈的动作，用于魔法棒
// 时间紧迫，简单起见就只检测前向轴的变小又变大的过程
// （其实就是上下摆动的动作，它是画圈动作的一个分量，做上下摆也可以触发，画圈也可以触发）
// 更精确的检测可以加入俯仰角和偏航角，更高科技的做法可以用深度学习，关键是咱都不会，就放在备注里了
void GravityGesture::checkCircledEvent()
{
    static unsigned long lastTick = 0;
    
    unsigned long curTick = millis();
    if (curTick - lastTick < CHECK_INTERVAL)
        return;

    lastTick = curTick;

    // 检测的阶段，画圈的前向加速度变化趋势接近于一个余弦曲线，但我们只判断大概就好，从正值变为负值再变回正值，先减小，再增大
    // 阶段 1 正值，减小
    // 阶段 2 负值，减小
    // 阶段 3 负值，增大
    // 阶段 4 正值，增大
    static uint8_t period = 0; 
    static int lastAcc = 0;
    static Smoothing s(10);

    // int acc  = getGyroAcc(dirForward);  // 当前加速度（直接用会有跳动，需要滤波）
    s.addData(getGyroAcc(dirForward));     // 进行平滑滤波一下
    int acc = s.getAverage();
    int dAcc = acc - lastAcc;              // 变化量
    lastAcc = acc;

    // 如果是状态 4，可以触发画圈事件了
    if (period == 4)
    {
        period = 0;
        
        if (cbCircled != nullptr)
        {
            cbCircled();
        }

        return;
    }

    // 整个画圈检测过程要求加速度不大于摆手势的阈值，减少复杂度
    if (abs(acc) > threshold)
    {
        period = 0;
        return;
    }

    // 任何阶段都可以进入状态 1，其它情况不匹配的话，会进入状态 0
    if (acc > 0 && dAcc <= 0)
    {
        period = 1;
        return;
    }

    if (acc <= 0 && dAcc <= 0 && (period == 1 || period == 2))
    {
        period = 2;
        return;
    }

    if (acc <= 0 && dAcc >= 0 && (period == 2 || period == 3))
    {
        period = 3;
        return;
    }

    if (acc > 0 && dAcc >= 0 && period == 3)
    {
        period = 4;
        return;
    }

    period = 0;
}

void GravityGesture::update()
{
    checkCircledEvent();
    checkShakeEvent();
}

void GravityGesture::registerShakeUpCallback(void (*cbShakeUp)())
{
    this->cbShakeUp = cbShakeUp;
}

void GravityGesture::registerShakeDownCallback(void (*cbShakeDown)())
{
    this->cbShakeDown = cbShakeDown;
}

void GravityGesture::registerShakeLeftCallback(void (*cbShakeLeft)())
{
    this->cbShakeLeft = cbShakeLeft;
}

void GravityGesture::registerShakeRightCallback(void (*cbShakeRight)())
{
    this->cbShakeRight = cbShakeRight;
}

void GravityGesture::registerShakeForwardCallback(void (*cbShakeForward)())
{
    this->cbShakeForward = cbShakeForward;
}

void GravityGesture::registerShakeBackCallback(void (*cbShakeBack)())
{
    this->cbShakeBack = cbShakeBack;
}

void GravityGesture::registerCircledCallback(void (*cbCircled)())
{
    this->cbCircled = cbCircled;
}

bool GravityGesture::isMoving()
{
    return gyro->isMoving(threshold);
}

bool GravityGesture::isMovingUp()
{   
    return getGyroAcc(dirUp) > 0;
}

bool GravityGesture::isMovingDown()
{
    return getGyroAcc(dirUp) < 0;
}

bool GravityGesture::isMovingLeft()
{
    return getGyroAcc(dirRight) < 0;
}

bool GravityGesture::isMovingRight()
{
    return getGyroAcc(dirRight) > 0;
}

bool GravityGesture::isMovingForward()
{
    return getGyroAcc(dirForward) > 0;
}

bool GravityGesture::isMovingBack()
{
    return getGyroAcc(dirForward) < 0;
}