#include <can.hpp>
#include <PRT.h>

CanFrame rxFrame_Agent; // 定义接收帧
CanFrame txFrame_Agent; // 定义发送帧

uint8_t tx_buffer[CAN_MAX_LEN] = {}; // 发送buffer, 长度设置为255
uint8_t rx_buffer[CAN_MAX_LEN] = {}; // 接收buffer

uint8_t check_water_flag = 0;  // 检测水路板标志
uint8_t check_screen_flag = 0; // 检测屏幕标志

void CAN_AGENT_Init(void)
{
  if (ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 10, 10))
  {
    Serial.println("CAN AGENT Init success!");
  }
  else
  {
    Serial.println("CAN AGENT Init fail!");
  }
}

void Send_Command(int16_t cmd, int16_t paramone, int16_t paramtwo)
{
  CanFrame cmdFrame = {0};
  cmdFrame.identifier = CAN_ID_AGENT;
  cmdFrame.extd = 0;
  cmdFrame.data_length_code = 8;
  cmdFrame.data[0] = 0x00; // CMD 标志
  cmdFrame.data[1] = 0x01;
  cmdFrame.data[2] = cmd & 0x00FF;
  cmdFrame.data[3] = cmd >> 8;
  cmdFrame.data[4] = paramone & 0x00FF;
  cmdFrame.data[5] = paramone >> 8;
  cmdFrame.data[6] = paramtwo & 0x00FF;
  cmdFrame.data[7] = paramtwo >> 8;
  ESP32Can.writeFrame(cmdFrame);
}

void send_val(uint16_t val_id, uint8_t *data)
{
  CanFrame valFrame = {0};
  valFrame.identifier = CAN_ID_AGENT;
  valFrame.extd = 0;
  // uint8_t data_len = sizeof(data) / sizeof(uint8_t);
  valFrame.data_length_code = 8;
  valFrame.data[0] = val_id >> 8;
  valFrame.data[1] = val_id & 0x00FF;

  for (uint8_t i = 0; i < 6; i++)
  {
    valFrame.data[i + 2] = *((uint8_t *)data + i);
  }
  ESP32Can.writeFrame(valFrame);
}

void send_val(uint16_t val_id, uint16_t val1)
{
  CanFrame valFrame = {0};
  valFrame.identifier = CAN_ID_AGENT;
  valFrame.extd = 0;
  valFrame.data_length_code = 8;
  valFrame.data[0] = val_id >> 8;
  valFrame.data[1] = val_id & 0x00FF;
  valFrame.data[6] = val1 >> 8;
  valFrame.data[7] = val1 & 0x00FF;
  ESP32Can.writeFrame(valFrame);
}

void send_val(uint16_t val_id, uint16_t val1, uint16_t val2)
{
  CanFrame valFrame = {0};
  valFrame.identifier = CAN_ID_AGENT;
  valFrame.extd = 0;
  valFrame.data_length_code = 8;
  valFrame.data[0] = val_id >> 8;
  valFrame.data[1] = val_id & 0x00FF;
  valFrame.data[6] = val1 >> 8;
  valFrame.data[7] = val1 & 0x00FF;
  valFrame.data[4] = val2 >> 8;
  valFrame.data[5] = val2 & 0x00FF;
  ESP32Can.writeFrame(valFrame);
}

void send_val(uint16_t val_id, uint16_t val1, uint16_t val2, uint16_t val3)
{
  CanFrame valFrame = {0};
  valFrame.identifier = CAN_ID_AGENT;
  valFrame.extd = 0;
  valFrame.data_length_code = 8;
  valFrame.data[0] = val_id >> 8;
  valFrame.data[1] = val_id & 0x00FF;
  valFrame.data[6] = val1 >> 8;
  valFrame.data[7] = val1 & 0x00FF;
  valFrame.data[4] = val2 >> 8;
  valFrame.data[5] = val2 & 0x00FF;
  valFrame.data[2] = val3 >> 8;
  valFrame.data[3] = val3 & 0x00FF;
  ESP32Can.writeFrame(valFrame);
}

void send_val(uint16_t val_id, float f_val1)
{
  uint8_t buffer[4] = {};
  float_to_bytes(f_val1, buffer);
  // send_val(val_id, *((uint16_t *)buffer), *((uint16_t *)(buffer + 2)));
  CanFrame valFrame = {0};
  valFrame.identifier = CAN_ID_AGENT;
  valFrame.extd = 0;
  valFrame.data_length_code = 8;
  valFrame.data[0] = val_id >> 8;
  valFrame.data[1] = val_id & 0x00FF;
  valFrame.data[4] = buffer[0];
  valFrame.data[5] = buffer[1];
  valFrame.data[6] = buffer[2];
  valFrame.data[7] = buffer[3];
  ESP32Can.writeFrame(valFrame);
}

// 将浮点型数据转换为字节型数据
void float_to_bytes(float f, uint8_t *bytes)
{
  union
  {
    float f;
    uint8_t bytes[4];
  } data;
  data.f = f;
  for (int i = 0; i < 4; i++)
  {
    bytes[i] = data.bytes[i];
  }
}

// 将字节型数据转换为浮点型数据
float bytes_to_float(uint8_t *bytes)
{
  union
  {
    float f;
    uint8_t bytes[4];
  } data;
  for (int i = 0; i < 4; i++)
  {
    data.bytes[i] = bytes[i];
  }
  return data.f;
}

uint16_t receive_int_val(uint8_t *data)
{
  return (data[6] << 8) + data[7];
}

float receive_float_val(uint8_t *data)
{
  uint8_t buffer[4] = {data[4], data[5], data[6], data[7]};
  return bytes_to_float(buffer);
}

Two_value receive_two_int_val(uint8_t *data)
{
  Two_value two_val;
  two_val.value1 = receive_int_val(data);
  two_val.value2 = (data[4] << 8) + data[5];
  return two_val;
}
