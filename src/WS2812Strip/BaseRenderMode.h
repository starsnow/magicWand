// ----------------------------------------
// BaseRenderMode.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-19
// 功  能：2812 渲染模式基类
// ----------------------------------------

#ifndef _BASE_RENDER_MODE_H_
#define _BASE_RENDER_MODE_H_

class BaseRenderMode
{
public:
    virtual uint8_t      getID()  { return 0;}              // 模式ID
    virtual void         render() {}                        // 渲染函数
    virtual unsigned int getRenderInterval() {return 10;}   // 渲染时间间隔（单位毫秒）
    virtual void         init()   {}                        // 初始化
    virtual void         enter()  {Serial.println("enter base");}                        // 进入模式
    virtual void         exit()   {}                        // 退出模式
};

#endif
