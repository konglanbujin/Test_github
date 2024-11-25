#include <Arduino.h>
#include <ESP32-TWAI-CAN.hpp> // CAN通信库
#include <SPI.h>
#include <Ethernet.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <main_config.h>
#include <PRT.h>
#include <RESP.h>
#include <can.hpp>
#include <screen.hpp>
#include "driver/uart.h"
#include "val_id.h"

#define TRUNK 0
#define HEAD 1

int16_t CMD;      // 命令
int16_t ParamOne; // 参数1
int16_t ParamTwo; // 参数2

uint8_t laserStatus = 0; // 激光器状态
uint8_t laserQdly = 0;   // Qdelay
uint8_t laserErr = 0;
uint8_t laserEnergy = 0; // 激光能量

/************************* FreeRTOS task ******************************/
void Serial_task(void *pvParameters); // 串口任务函数
void CAN_task(void *pvParameters);    // CAN通信任务函数
void Screen_task(void *pvParameters); // 触控屏幕任务函数
// void TEST_task(void *pvParameters);   // 测试任务函数
void Check_connect(void *pvParameters); // 检查连接任务函数
/*********************************************************************/

static TaskHandle_t Check_connect_handle;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(0);
  /************************************ */

  Screen_Init(); // 与串口屏连接的串口初始化
  /************************************ */
  CAN_AGENT_Init();
  xTaskCreate(Serial_task, "Serial", 1024 * 10, NULL, 4, NULL);
  xTaskCreate(CAN_task, "CAN", 1024 * 10, NULL, 4, NULL);
  xTaskCreate(Screen_task, "Screen", 1024 * 10, NULL, 4, NULL);
  // xTaskCreate(TEST_task, "TEST", 1024 * 10, NULL, 4, NULL);
  xTaskCreate(Check_connect, "Check_connect", 1024 * 10, NULL, 4, &Check_connect_handle);
}

void loop() {}

void Serial_task(void *pvParameters)
{
  while (1)
  {
    // 检查是否有数据可读
    if (Serial.available() > 0)
    {
      CMD = Serial.parseInt();
      ParamOne = Serial.parseInt();
      ParamTwo = Serial.parseInt();
      LOG2("CMD", CMD);
      LOG2("ParamOne", ParamOne);
      LOG2("ParamTwo", ParamTwo);
      switch (CMD)
      {
      case ID_PUMP_CHECKING:
      {
        check_screen_flag = 0;
        check_water_flag = 0;
        Send_Command(CMD, ParamOne, ParamTwo);
        // 串口屏检测
        Serial0.write(ask, 7);
        xTaskNotifyGive(Check_connect_handle); /* 通知检查连接任务执行 */
      }
      break;

      default:
      {
        Send_Command(CMD, ParamOne, ParamTwo);
      }
      break;
      }
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

/* 串口触摸屏任务函数 */
void Screen_task(void *pvParameters)
{
  while (1)
  {
    memset(screen_data, 0, 9);

    if (Serial0.available() > 0)
    {
      Serial0.readBytes(screen_data, 9); // 从串口屏读取数据

      if (screen_data[4] == answer[4])
      {
        check_screen_flag = 1;
      }
      else
      {
        check_screen_flag = 0;
      }

      value = judge(screen_data);
      if (value < MOTOR_ON) // 如果是收到滑块的信息
      {
        Serial.print("Target:");
        Serial.println(value);
        send_val(ID_SCREEN_POS, (uint16_t)(value)); // Target position
      }
      switch (value) // 按键状态判断
      {

      case MOTOR_ON:
        Serial.println("MOTOR_ON");
        break;
      case MOTOR_OFF:
        Serial.println("MOTOR_OFF");
        break;
      case FILL_ON: // 进水
      {
        state = 1;
        Send_Command(5, state * speed, 0);
        break;
      }
      case DRAIN_ON: // 排水
      {
        state = -1;
        Send_Command(5, state * speed, 0);
        break;
      }
      case FILLDRAIN_OFF: // 关闭进排水
      {
        state = 0;
        Send_Command(5, state * speed, 0);
        Serial.println("FILLDRAIN_OFF");
        break;
      }
      case LOW_ON: // 低速
      {
        speed = 85;
        Send_Command(5, state * speed, 0);
        Serial.println("LOW_ON");
        break;
      }
      case MIDDLE_ON: // 中速
      {
        speed = 170;
        Send_Command(5, state * speed, 0);
        Serial.println("MIDDLE_ON");
        break;
      }
      case HIGH_ON: // 高速
      {
        speed = 255;
        Send_Command(5, state * speed, 0);
        Serial.println("HIGH_ON");
        break;
      }
      case TURN_ON:
      {
        Serial.println("TURN_ON");
        break;
      }
      case TURN_OFF:
      {
        Serial.println("TURN_OFF");
        break;
      }
      default:
        break;
      }
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void CAN_task(void *pvParameters)
{
  while (1)
  {
    if (ESP32Can.readFrame(rxFrame_Agent, 1000))
    {
      uint16_t id = (rxFrame_Agent.data[0] << 8) + (rxFrame_Agent.data[1]);
      // PRT2("id: ", id);
      switch (rxFrame_Agent.identifier)
      {
      case 0x701: // 激光器
      {
        // Serial.print("Laser01: ");
        // int i = 0;
        // for (i = 0; i < 8; i++)
        // {
        //   Serial.println(rxFrame_Agent.data[i], HEX);
        // }
        // Serial.println();

        switch (id)
        {
        case ID_LASER_CMD: // CMD
        {
          /* 解析CMD */
        }
        break;
        case ID_LASER_STATE: // 激光器状态
        {
          laserStatus = rxFrame_Agent.data[1];
          Serial.print("laserStatus: ");
          Serial.println(laserStatus);
        }
        break;
        case ID_LASER_QDELAY: // Qdelay
        {
          laserQdly = rxFrame_Agent.data[1] + (rxFrame_Agent.data[2] << 8);
          laserEnergy = (laserQdly <= 209 && laserQdly >= 9) ? (209 - laserQdly) / 2 : -1; // WJD 20230620 fix energy issue
          Serial.print("laserQdly: ");
          Serial.println(laserQdly);
          Serial.print("LS:");
          Serial.print(laserStatus);
          Serial.print(",");
          Serial.println(laserEnergy);
        }
        break;

        default:
          break;
        }
        break;
      }
      case 0x702: // 水路
      {
        switch (id)
        {
        case ID_WATER_CMD: // CMD
        {
        }
        break;
        case ID_WATER_PRESSURE: // 压力
        {
          int16_t pressure = receive_int_val(rxFrame_Agent.data);
          PRT2("pressure ", pressure);
        }
        break;
        case ID_WATER_TEMP: // 温度
        {
          float temperature = receive_float_val(rxFrame_Agent.data);
          PRT2("temperature ", temperature);
        }
        break;
        case ID_SCAN_POS_PERCENT:
        {
          int16_t pos = receive_int_val(rxFrame_Agent.data);
          // PRT2("pos: ", pos);
          current[7] = uint8_t(pos);
          Serial0.write(current, 8);
          PRT2("pos", pos);
        }
        break;
        case ID_PUMP_GETPOS: // 获取当前电机位置
        {
          Two_value current_pos = receive_two_int_val(rxFrame_Agent.data);
          int16_t Current_POS_Scan = current_pos.value1;
          int16_t Percent_POS_Scan = current_pos.value2;
          LOG2("Current_POS_Scan", Current_POS_Scan);
          LOG2("Percent_POS_Scan", Percent_POS_Scan);
        }
        break;
        case ID_PUMP_CHECKING: // 通信检测
        {
          // 水路检测
          int16_t Checking_Result = receive_int_val(rxFrame_Agent.data);
          if (Checking_Result == ID_PUMP_CHECKING)
          {
            check_water_flag = 1;
          }
        }
        break;

        default:
          break;
        }
        break;
      }
      default:
        break;
      }
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

/**
 * @brief 检查连接任务函数
 * @details 检查触摸屏和水路的通信是否正常，如果正常则发送OK，否则发送NG
 */
void Check_connect(void *pvParameters)
{
  while (1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 阻塞自身，等待通知执行
    vTaskDelay(5 / portTICK_PERIOD_MS);      // 等待5ms
    if (check_screen_flag == 1)
    {
      PRT1("Checking Screen successfully!");
    }
    else
    {
      PRT1("Checking Screen failed!");
    }
    if (check_water_flag == 1)
    {
      PRT1("Checking Water successfully!");
    }
    else
    {
      PRT1("Checking Water failed!");
    }
  }
}