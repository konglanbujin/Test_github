#pragma once

#include <Arduino.h>

#ifdef __cplusplus

#endif

// 定义功能按键
#define MOTOR_ON 101
#define MOTOR_OFF 102
#define FILL_ON 103
#define DRAIN_ON 104
#define DRAIN_OFF 105
#define FILLDRAIN_OFF 106
#define LOW_ON 107
#define MIDDLE_ON 108
#define HIGH_ON 109
#define TURN_ON 110
#define TURN_OFF 111
#define TRUNK_FILL_ON 112
#define HEAD_FILL_ON 113
#define TRUNK_DRAIN_ON 114
#define HEAD_DRAIN_ON 115
#define STOP 116

extern uint8_t screen_data[9]; // 数据缓冲区
extern uint8_t current[];      // 显示数值
extern uint8_t value;          // 函数返回值，0-100为Target pos值，101-115为对应按键值
extern uint8_t ask[7];         // 5A A5 04 83 11 00 01
extern uint8_t answer[9];      // 5A A5 06 83 11 00 01 00 00

extern int16_t state; // 控制进排水状态
extern int16_t speed; // 进排水速度

uint8_t judge(uint8_t *data);
void Screen_Init(void);
