// ----------------------------------------
// gyro.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-11
// 功  能：九轴传感器封装
// ----------------------------------------

#ifndef _GYRO_H_
#define _GYRO_H_

#include <MPU6050.h>

class Gyro
{
private:
    int xAcc;
    int yAcc;
    int zAcc;
    int scale;
    MPU6050 accelgyro;
    int16_t ax, ay, az;
    int16_t offsetX, offsetY, offsetZ;
    bool status;

public:
    Gyro()
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
        // Serial.print("offset x: ");
        // Serial.println(offsetX);
        // Serial.print("offset y: ");
        // Serial.println(offsetY);
        // Serial.print("offset z: ");
        // Serial.println(offsetZ);
    }

    void update()
    {
        accelgyro.getAcceleration(&ax, &ay, &az);
        xAcc = (ax - offsetX) / scale;
        yAcc = (ay - offsetY) / scale;
        zAcc = (az - offsetZ) / scale;
    }

    int getXAcc()
    {
        return xAcc;
    }

    int getYAcc()
    {
        return yAcc;
    }

    int getZAcc()
    {
        return zAcc;
    }

    bool isMoving(unsigned int threshold)
    {
        if (abs(xAcc) >= threshold || abs(yAcc) >= threshold || abs(zAcc) >= threshold)
        {
            return true;
        }
        
        return false;
    }
};

#endif
