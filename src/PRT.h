#pragma once 
#include "Arduino.h"
#include "main_config.h"

#ifdef RELEASE
#define PRT1(x)           \
  {                       \
    Serial.println(F(x)); \
  }
#define PRT2(x, y)        \
  {                       \
    Serial.print(F(x));   \
    Serial.print(F(":")); \
    Serial.println(y);    \
  }
#define PRT2N(x, y)       \
  {                       \
    Serial.print(x);      \
    Serial.print(F(":")); \
    Serial.println(F(y)); \
  }
#else
#define PRT1(x)
#define PRT2(x, y)
#define PRT2N(x, y)
#endif

#ifdef DEBUG
#define LOG(x) Serial.print(x);
#define LOG_LN(x) Serial.println(x);
#define LOG1(x)           \
  {                       \
    Serial.print("-> ");  \
    Serial.println(F(x)); \
  }
#define LOG2(x, y)         \
  {                        \
    Serial.print("-> ");   \
    Serial.print(F(x));    \
    Serial.print(F(": ")); \
    Serial.println(y);     \
  }
#define LOG3(x, y, z)      \
  {                        \
    Serial.print("-> ");   \
    Serial.print(F(x));    \
    Serial.print(y);       \
    Serial.print(F(": ")); \
    Serial.println(z);     \
  }
#else
#define LOG(x)
#define LOG_LN(x)
#define LOG1(x)
#define LOG2(x, y)
#define LOG3(x, y, z)
#endif

