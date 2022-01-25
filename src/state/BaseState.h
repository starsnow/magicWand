// ----------------------------------------
// BaseState.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-13
// 功  能：状态的基类
// 补充说明：状态的更新、进入和退出等时机可以增加回调函数，这样更灵活一些，以后有用到了再弄吧，现在就将就用着
// ----------------------------------------

#ifndef _STATE_BASE_H_
#define _STATE_BASE_H_

#include "Arduino.h"

#define DEFAULT_THRESHOLD 10

class BaseState
{
public:
    virtual uint8_t getID() { return 0; }
    virtual void update()   {}
    virtual void enter()    {}
    virtual void exit()     {}
    virtual ~BaseState()    {}
};

#endif
