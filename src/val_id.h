#pragma once

/**
 * 定义CAN通信中各变量的ID
 *
 */

/**
 * Agent	0x700
 * Laser & 	0x701
 * Water & Piston & Scan_Motor	0x702
 */

/* Agent	0x700*/
#define ID_AGENT_CMD 1
#define ID_SCREEN_POS 2 // 屏幕_电机位置

#define ID_PUMP_FILLDRAIN 5
#define ID_PUMP_GETPOS 33
#define ID_PUMP_CALIBRATION 39
#define ID_PUMP_CHECKING 100

/* Laser & 	0x701*/
#define ID_LASER_CMD 1
#define ID_LASER_STATE 2
#define ID_LASER_QDELAY 3

/* Water & Piston & Scan_Motor	0x702*/
#define ID_WATER_CMD 1
#define ID_WATER_TEMP 2
#define ID_WATER_PRESSURE 3

#define ID_SCAN_CURRENT_POS 4
#define ID_SCAN_TARGET_POS 5
#define ID_SCAN_POS_PERCENT 6
#define ID_SCAN_STATE 7

/******************************** */
#define ID_PUMP_GETPOS 33
#define ID_PUMP_CHECKING 100
/******************************** */
