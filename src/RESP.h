#pragma once
#include <Arduino.h>
#include <stdio.h>

#define PACT_SN "SN:SBH-PACT-512-003"

/* response type */
#define RESP_WL 1
#define RESP_WV 2
#define RESP_WO 3
#define RESP_WD 4
#define RESP_MT 5
#define RESP_ST 6
#define RESP_SW 7
#define RESP_LA 8
#define RESP_LS 9
#define RESP_SN 0
#define RESP_ERR 254
#define RESP_DBG 255
#define RESP_SV 10    // servos
#define RESP_MS 11    //拉膜状态
#define RESP_MP 12    //拉膜百分比
#define RESP_DS 13    //设备状态
#define RESP_CS 14    //活塞校准状态

void respToPC(uint8_t type, void *data);    
void returnSN(void);