// ----------------------------------------
// debug.h
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：DEBUG 的功能，在包含此头文件前定义 DEBUG 即可开启 DEBUG 输出
// ----------------------------------------

#ifndef _DEBUG_H_
#define _DEBUG_H_

#define DEBUG

#ifdef DEBUG
    #include <arduino.h>
    #include <SoftwareSerial.h>
    // SoftwareSerial debugSerial(12, 13);
    #define debugSerial Serial
    #define DEBUG_INIT(x)   debugSerial.begin(x);
    #define DEBUG_OUT(x)    debugSerial.print(x);
    #define DEBUG_OUTLN(x)  debugSerial.println(x);
#else
    #define DEBUG_INIT(x)
    #define DEBUG_OUT(x)
    #define DEBUG_OUTLN(x)
#endif

#endif
