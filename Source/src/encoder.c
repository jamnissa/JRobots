#include "stm32f10x.h"                  // Device header


void tim1EncoderBegin(void) 
{
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;       
   RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;       

   GPIOA->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0);   
   GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1;

   GPIOA->BSRR = GPIO_BSRR_BR8;
   GPIOA->BSRR = GPIO_BSRR_BR9; 

   TIM1->CCMR1 |=  TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;  
   TIM1->CCER  &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);    
   TIM1->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;    

   TIM1->ARR = 65535;           
   TIM1->CR1 |= TIM_CR1_CEN;         
   TIM1->CNT = 0;
}

int main() {
   tim1EncoderBegin();
   
   while(1) {
      uint16_t volatile encoderRight = (TIM1->CNT);  
      delay(100);		 
   }
}