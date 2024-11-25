#include <screen.hpp>
#include <driver/uart.h>

#define TXD_PIN_232_0 (43) // 232串口0发送管脚
#define RXD_PIN_232_0 (44) // 232串口0接收管脚

uint8_t screen_data[9];                               // 数据缓冲区
uint8_t current[] = {90, 165, 5, 130, 32, 19, 0, 0};  // 显示数值
uint8_t value;                                        // 函数返回值，0-100为Target pos值，101-115为对应按键值
uint8_t ask[] = {90, 165, 4, 131, 17, 0, 1};          // 5A A5 04 83 11 00 01
uint8_t answer[] = {90, 165, 6, 131, 17, 0, 1, 0, 0}; // 5A A5 06 83 11 00 01 00 00

int16_t state = 0;   // 控制进排水状态
int16_t speed = 255; // 进排水速度

/*初始化屏幕串口*/
void Screen_Init(void)
{
  Serial0.begin(115200);
  Serial0.setTimeout(0);
  Serial0.setPins(RXD_PIN_232_0, TXD_PIN_232_0); // 与串口屏连接的串口初始化
}

uint8_t judge(uint8_t *data) // 判断串口屏触控状态
{
  if (data[0] == 90 && data[1] == 165)
  {
    data[0] = 0;
    data[1] = 0;

    if (data[5] == 5)
    {
      if (data[8] == 0)
        return FILL_ON;
      else if (data[8] == 2)
        return DRAIN_ON;
      else
        return FILLDRAIN_OFF;
    }
    else if (data[5] == 21)
    {
      if (data[8] == 3)
        return LOW_ON;
      else if (data[8] == 2)
        return MIDDLE_ON;
      else if (data[8] == 1)
        return HIGH_ON;
    }
    else if (data[5] == 48)
      return MOTOR_ON;
    else if (data[5] == 51)
      return MOTOR_OFF;
    else if (data[5] == 7)
    {
      if (data[8] == 1)
        return TURN_ON;
      else
        return TURN_OFF;
    }
    else if (data[5] == 3)
      return data[8];
  }
  return STOP;
}
