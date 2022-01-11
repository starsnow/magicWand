// ----------------------------------------
// gyro.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-11
// 功  能：九轴传感器封装
// ----------------------------------------

#ifndef _GYRO_H_
#define _GYRO_H_

#include <MPU6050.h>

class gyro
{
private:
    int xSpeed;
    int ySpeed;
    int scale;
    MPU6050 accelgyro;
    int16_t ax, ay, az;
    int16_t offsetX, offsetY, offsetZ;
    bool status;

public:
    WindSystem()
    {
        status = false;
    }

    void init(int scale = 1000)
    {
        Wire.begin();
        accelgyro.initialize();
        status = accelgyro.testConnection() ? true : false;
        accelgyro.getAcceleration(&offsetX, &offsetY, &offsetZ);
        this->scale = scale;
    }

    void update()
    {
        accelgyro.getAcceleration(&ax, &ay, &az);
        xSpeed = (ax - offsetX) / scale;
        ySpeed = (az - offsetZ) / scale;
    }

    int getXSpeed()
    {
        return xSpeed;
    }

    int getYSpeed()
    {
        return ySpeed;
    }

};

#endif
