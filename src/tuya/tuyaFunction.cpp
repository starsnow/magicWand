// ----------------------------------------
// tuyaFunction.cpp
// 创建者：seesea <seesea2517#gmail#com>
// 时  间：2022-01-12
// 功  能：涂鸦功能相关的功能实现
// ----------------------------------------

#include "tuyaDefination.h"
#include <TuyaWifi.h>
#include "../debug.h"

// wifi 相关功能变量    

#if (defined ARDUINO_AVR_MICRO) || (defined ARDUINO_AVR_LEONARDO)
    TuyaWifi tuya_device(&Serial1);
#else
    TuyaWifi tuya_device;
#endif

uint8_t reconnect_button, wifi_led;


//* Current device DP values */
unsigned char dp_bool_value = 0;
long dp_value_value = 0;
unsigned char dp_enum_value = 0;
unsigned char dp_string_value[21] = {"0"};

//unsigned char dp_raw_value[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 *                                     dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
*/
unsigned char dp_array[][2] = {
    {DPID_GESTURE, DP_TYPE_ENUM},
};

void init_wifi()
{
    //Initialize led port, turn off led.
    //Initialize networking keys.
    //incoming all DPs and their types array, DP numbers
    //Enter the PID and MCU software version
    pinMode(reconnect_button, INPUT_PULLUP);
    pinMode(wifi_led, OUTPUT);
    digitalWrite(wifi_led, LOW);
}

void check_reconnect_wifi()
{
    static unsigned long last_time = 0;
    static unsigned char wifi_led_state = 0;

    //Enter the connection network mode when Pin7 is pressed.
    if (digitalRead(reconnect_button) == LOW)
    {
        delay(80);
        if (digitalRead(reconnect_button) == LOW)
        {
            tuya_device.mcu_set_wifi_mode(SMART_CONFIG);
        }
    }

    /* LED blinks when network is being connected */
    if ((tuya_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (tuya_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (tuya_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW))
    {
        if (millis() - last_time >= 500)
        {
            last_time = millis();

            if (wifi_led_state == LOW)
            {
                wifi_led_state = HIGH;
            }
            else
            {
                wifi_led_state = LOW;
            }
            digitalWrite(wifi_led, wifi_led_state);
        }
    }
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char dp_process(unsigned char dpid, const unsigned char value[], unsigned short length)
{
    DEBUG_OUT(F("dpid: "));
    DEBUG_OUTLN(dpid);

    switch (dpid)
    {
    case DPID_GESTURE:
        //Status changes should be reported.
        tuya_device.mcu_dp_update(dpid, value, length);
        break;

    default:
        break;
    }
    return SUCCESS;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
    tuya_device.mcu_dp_update(DPID_GESTURE, 4, DP_TYPE_ENUM);
}

void tuya_setup(unsigned char *pid, unsigned char *mcu_ver, uint8_t wifi_reconnect_pin, uint8_t wifi_led_pin)
{
    DEBUG_OUT(F("WIFI_UART_RECV_BUF_LMT: "));
    DEBUG_OUTLN(WIFI_UART_RECV_BUF_LMT);
    DEBUG_OUT(F("WIFI_DATA_PROCESS_LMT: "));
    DEBUG_OUTLN(WIFI_DATA_PROCESS_LMT);
    
    reconnect_button = wifi_reconnect_pin;
    wifi_led = wifi_led_pin;

    init_wifi();

    tuya_device.init(pid, mcu_ver);
    tuya_device.set_dp_cmd_total(dp_array, sizeof(dp_array) / sizeof(dp_array[0]));
    tuya_device.dp_process_func_register(dp_process);         // register DP download processing callback function
    tuya_device.dp_update_all_func_register(dp_update_all);   // register upload all DP callback function
}

void tuya_loop()
{
    check_reconnect_wifi();
    tuya_device.uart_service();
}

// gesture: 
// 上 下 左 右 无
//  0  1  2  3  4 
void upload_gesture_value(uint8_t gesture)
{
    tuya_device.mcu_dp_update(DPID_GESTURE, gesture, DP_TYPE_ENUM);
}