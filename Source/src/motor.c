#include "motor.h"

void encoderLeftBegin(void) {
	
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	 RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;       
   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;       

   GPIOA->CRL   &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);   
   GPIOA->CRL   |=   GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1;

   GPIOA->BSRR   = GPIO_BSRR_BR6;
   GPIOA->BSRR   = GPIO_BSRR_BR7; 

   TIM3->CCMR1  |=  TIM_CCMR1_CC1S_0;  
	 TIM3->CCMR1  |=  TIM_CCMR1_CC2S_0;
   
	 TIM3->CCER   &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
	
   TIM3->SMCR   |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;    

   TIM3->ARR     = 0xFFFF;           
   TIM3->CR1    |= TIM_CR1_CEN;         
   TIM3->CNT     = 0;
	
}

void encoderRightBegin(void) {
	
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	 RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;       
   RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;       

   GPIOA->CRH   &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0);   
   GPIOA->CRH   |=   GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1;

   GPIOA->BSRR   = GPIO_BSRR_BR8;
   GPIOA->BSRR   = GPIO_BSRR_BR9; 

   TIM1->CCMR1  |=  TIM_CCMR1_CC1S_0;  
	 TIM1->CCMR1  |=  TIM_CCMR1_CC2S_0;
   
	 TIM1->CCER   &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
	
   TIM1->SMCR   |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;    
	
   TIM1->ARR     = 0xFFFF;           
   TIM1->CR1    |= TIM_CR1_CEN;         
   TIM1->CNT     = 0;
	 
}

float encoderRead(void) {
	
	int16_t volatile encoderLeft;
	int16_t volatile encoderRight;
	
	encoderLeft  = ~(TIM3->CNT);
	encoderRight =  (TIM1->CNT);
	
	float volatile encoderPositionsLeft;
	float volatile encoderPositionsRight;
	float volatile encoderPositionsCenter;
	
	encoderPositionsLeft  = encoderLeft  * lengthPulse;
	encoderPositionsRight = encoderRight * lengthPulse;
	
	encoderPositionsCenter = 0.5 * (encoderPositionsLeft + encoderPositionsRight);
	
	return encoderPositionsCenter;
}

/* driver begin */

void pwmBegin(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOA->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0);
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOA->CRL |=  (GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1);
		
	TIM2->PSC = 32; // 72000000 / 72 / 100 10k
	TIM2->ARR = 1000;
	
	TIM2->CCR1 = 0; //CH1
	TIM2->CCR2 = 0; //CH2
	
	TIM2->CCMR1 |=  TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM2->CCER  |=  TIM_CCER_CC1E;
	TIM2->CCER  &=~ TIM_CCER_CC1P;
	
	TIM2->CCMR1 |=  TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
	TIM2->CCER  |=  TIM_CCER_CC2E;
	TIM2->CCER  &=~ TIM_CCER_CC2P;
	
	TIM2->CR1   &=~ TIM_CR1_DIR;
	TIM2->CR1   |=  TIM_CR1_CEN;
}

void driverBegin(void) {
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE4 | GPIO_CRL_CNF4); //PA4 AIN2
	GPIOA->CRL |=   GPIO_CRL_MODE4;
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE5 | GPIO_CRL_CNF5); //PA5 AIN1
	GPIOA->CRL |=   GPIO_CRL_MODE5;
	
	GPIOB->CRL &=~ (GPIO_CRL_MODE0 | GPIO_CRL_CNF0); //PB0 BIN1
	GPIOB->CRL |=   GPIO_CRL_MODE0;
	
	GPIOB->CRL &=~ (GPIO_CRL_MODE1 | GPIO_CRL_CNF1); //PB1 BIN2
	GPIOB->CRL |=   GPIO_CRL_MODE1;
}

void motorBegin(void) {
	pwmBegin();
	driverBegin();
}

void motorPwm(uint16_t left, uint16_t right) {
	TIM2->CCR1 = left;  //CH1
	TIM2->CCR2 = right; //CH2
}

void motorLeft(uint16_t left, uint16_t right) {
	 motorPwm(left, right);
	 
	 GPIOA->BSRR = GPIO_BSRR_BR5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BS4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BS0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BR1; // BIN2
}

void motorRight(uint16_t left, uint16_t right) {
	 motorPwm(left, right);
	 
	 GPIOA->BSRR = GPIO_BSRR_BS5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BR4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BR0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BS1; // BIN2
}

void motorForward(uint16_t left, uint16_t right) {
	 motorPwm(left, right);
	 
	 GPIOA->BSRR = GPIO_BSRR_BS5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BR4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BS0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BR1; // BIN2
}

void motorBack(uint16_t left, uint16_t right) {
	 motorPwm(left, right);
	 
	 GPIOA->BSRR = GPIO_BSRR_BR5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BS4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BR0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BS1; // BIN2
}

void motorStop() {
	 motorPwm(0, 0);
	 
	 GPIOA->BSRR = GPIO_BSRR_BR5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BR4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BR0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BR1; // BIN2
}

