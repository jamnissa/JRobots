#include "stm32f10x.h"                  // Device header
#define PI       ((float)3.14)
#define RADIUS   ((float)17 / 1000)
#define QUANTITY ((float) 820.0f)


TIM_TypeDef * tim;

void tim1EncoderBegin(void) 
{
   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;       
   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;       

   GPIOA->CRL &= ~(GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);   
   GPIOA->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1;

   GPIOA->BSRR = GPIO_BSRR_BR6;
   GPIOA->BSRR = GPIO_BSRR_BR7; 

   TIM3->CCMR1 |=  TIM_CCMR1_CC1S_0;  
	 TIM3->CCMR1 |=  TIM_CCMR1_CC2S_0;
   
	 TIM3->CCER  &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);
	
   TIM3->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;    

   TIM3->ARR = 0xFFFF;           
   TIM3->CR1 |= TIM_CR1_CEN;         
   TIM3->CNT = 0;
}

int main() {
	 
	 tim = TIM1;
	
   tim1EncoderBegin();
	
	 int16_t volatile CNTz = 0;
	 int16_t volatile CNTy = 0;	
	 int16_t volatile CNTx = 0;	
	
	 float volatile dist = 0;	
   
	
	while(1) {
      
		  CNTx = TIM3->CNT;
			
			if ((CNTx >= 1000) | (CNTx <= -1000)) {
				TIM1->CNT = 0;
				CNTy = CNTz;
			}
				
			CNTz = CNTx + CNTy;
			
			dist = ((float)CNTz / 820) * (2 * PI * RADIUS);
			
			
	    for(uint32_t i=0; i < 2400; i++);
   }
}


	
// count_1 = 817
// count_2 = 827 r = 0.034 
// l = 2*pi*r * (ticks / 810)
// e = 1
// c = 2

// encoder = TIM1->CNT;  

// if ((encoder > 400) & (encoder < -400)) {
	//TIM1->CNT = 0;
	//dist_2 = dist_1;
//}

//k = (encoder / QUANTITY);

//dist_1 = k * (2 * PI * RADIUS) + dist_2;

//for(uint32_t i=0; i < 240; i++);

//void tim1EncoderBegin(void) 
//{
//   RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;       
//   RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;       

//   GPIOA->CRH &= ~(GPIO_CRH_CNF8_0 | GPIO_CRH_CNF9_0);   
//   GPIOA->CRH |= GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1;

//   GPIOA->BSRR = GPIO_BSRR_BR8;
//   GPIOA->BSRR = GPIO_BSRR_BR9; 

//   TIM1->CCMR1 |=  TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;  
//   TIM1->CCER  &= ~(TIM_CCER_CC1P | TIM_CCER_CC2P);    
//   TIM1->SMCR  |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;    

//   TIM1->ARR = 0xFFFF;           
//   TIM1->CR1 |= TIM_CR1_CEN;         
//   TIM1->CNT = 0;
//}
