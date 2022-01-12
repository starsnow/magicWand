// ----------------------------------------
// magicWand.ino
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：涂鸦魔法棒
// ----------------------------------------

#include <debug.h>
#include <FastLED.h>
#include "WS2812.h"
#include "tuyaDefination.h"


const uint8_t WIFI_RECONNECT_BUTTON_PIN = 3;
const uint8_t WIFI_LED = LED_BUILTIN;

unsigned char pid[] = {"vhj6p7rzvoemihhx"}; //*********处替换成涂鸦IoT平台自己创建的产品的PID
unsigned char mcu_ver[] = {"1.0.0"};

void setup()
{
    DEBUG_INIT(9600);
    DEBUG_OUT(F("WIFI_UART_RECV_BUF_LMT: "));
    DEBUG_OUT_LN(WIFI_UART_RECV_BUF_LMT);
    DEBUG_OUT(F("WIFI_DATA_PROCESS_LMT: "));
    DEBUG_OUT_LN(WIFI_DATA_PROCESS_LMT);

    Serial.begin(9600);

    tuya_setup(pid, mcu_ver, WIFI_RECONNECT_BUTTON_PIN, WIFI_LED);
    initWS2812();
}

void loop()
{
    tuya_loop();
    updateWS2812();
}
