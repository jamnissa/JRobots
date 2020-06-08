#include "stm32f10x.h"                 

#define PI       ((float)3.14)
#define RADIUS   ((float)17.0f / 1000.0f)
#define QUANTITY ((float)820.0f)

#define lengthPulse (float)(0.0001302) // R=17mm pulse = 820

void encoderRightBegin(void);
void encoderLeftBegin(void);
float encoderRead(void);
	
void pwmBegin(void);
void driverBegin(void);
void motorBegin(void);

void motorPwm(uint16_t left, uint16_t right);
void motorLeft(uint16_t left, uint16_t right);
void motorRight(uint16_t left, uint16_t right);
void motorBack(uint16_t left, uint16_t right);
void motorForward(uint16_t left, uint16_t right);
void motorStop(void);
