// ----------------------------------------------------------------------------
// Smoothing.h
//
// Created 2015-08-31
// By seesea <seesea2517#gmail#com>
//
// 滑动平均滤波，用以避免小振动
// 参考自带的示例并做了封装
// ----------------------------------------------------------------------------

#ifndef _SMOOTHING_H_
#define _SMOOTHING_H_

class Smoothing
{
private:
    int  *buf;                   // 处理数据所需要的内存指针
    long total;                  // 所有元素的总和
    int  readIndex;              // 当前处理数据的数组元素索引
    int  size;                   // 处理内存的元素个数

public:
    // 构造函数
    // 根据传入的参数申请相应的内存
    Smoothing(int bufferSize)
    {
        // 如果分配失败怎样抛出异常？待学习
        buf = new int [bufferSize];
        memset(buf, 0, sizeof(int) * bufferSize);

        readIndex = 0;
        size      = bufferSize;
        total     = 0;
    }
    
    // 析构函数
    // 释放申请的内存
    ~Smoothing()
    {
        delete [] buf;
    }

    // 加入一个新数据
    void addData(int data)
    {
        total -= buf[readIndex];
        total += data;
        buf[readIndex] = data;

        if (++readIndex >= size)
            readIndex = 0;
    }

    // 获取平均值
    long getAverage()
    {
        // 构造函数保证 size 不为 0
        return total / size;
    }
};

#endif
