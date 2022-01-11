#ifndef _DEBUG_H_
#define _DEBUG_H_

// #define DEBUG

#ifdef DEBUG
    #include <arduino.h>
    #include <SoftwareSerial.h>
    SoftwareSerial debugSerial(12, 13);
    #define DEBUG_INIT(x)   debugSerial.begin(x);
    #define DEBUG_OUT(x)    debugSerial.print(x);
    #define DEBUG_OUT_LN(x) debugSerial.println(x);
#else
    #define DEBUG_OUT(x)
    #define DEBUG_OUT_LN(x)
#endif

#endif