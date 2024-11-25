#pragma once
#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp> // CAN通信库

/**
 * Agent	0x700
 * Laser	0x701
 * Water	0x702
 * Motor	0x703
 * Servos	0x704
 * Opo	    0x705
 */

#define CAN_TX 39
#define CAN_RX 40
#define CAN_MAX_LEN 255    // 最大数据长度
#define CAN_ID_AGENT 0x700 // 设备ID

extern CanFrame rxFrame_Agent; // 定义接收帧
extern CanFrame txFrame_Agent; // 定义发送帧

extern uint8_t tx_buffer[CAN_MAX_LEN]; // 发送buffer, 长度设置为255
extern uint8_t rx_buffer[CAN_MAX_LEN]; // 接收buffer

extern uint8_t check_water_flag;  // 检测水路板标志
extern uint8_t check_screen_flag; // 检测屏幕标志

struct Two_value
{
    uint16_t value1;
    uint16_t value2;
};

void CAN_AGENT_Init(void);                                          /*CAN总线初始化函数*/
void send_val(uint16_t val_id, uint8_t *data);                      /* 发送变量 */
void Send_Command(int16_t cmd, int16_t paramone, int16_t paramtwo); /* 发送命令 */

void send_val(uint16_t val_id, uint16_t val1);
void send_val(uint16_t val_id, uint16_t val1, uint16_t val2);
void send_val(uint16_t val_id, uint16_t val1, uint16_t val2, uint16_t val3);
void send_val(uint16_t val_id, float f_val1);

uint16_t receive_int_val(uint8_t *data);
float receive_float_val(uint8_t *data);

void float_to_bytes(float f, uint8_t *bytes);
float bytes_to_float(uint8_t *bytes);
Two_value receive_two_int_val(uint8_t *data);