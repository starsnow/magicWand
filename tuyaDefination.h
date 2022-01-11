// 涂鸦功能相关的头文件

#ifndef _TUYA_DEFINATION_H_
#define _TUYA_DEFINATION_H_

//开关(可下发可上报)
//备注:
#define DPID_SWITCH_LED 20

void tuya_setup(unsigned char *pid, unsigned char *mcu_ver, uint8_t wifi_reconnect_pin, uint8_t wifi_led_pin);
inline void tuya_loop();
// unsigned char dp_process(unsigned char dpid, const unsigned char value[], unsigned short length);
// void dp_update_all(void);
// void init_wifi();
// void check_reconnect_wifi();

#endif