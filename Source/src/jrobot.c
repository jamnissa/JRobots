#include "motor.h"
#include "math.h"

TIM_TypeDef * tim;

void tim4Begin(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	
	TIM4->PSC     = 72000000 / 1000000 - 1;
	TIM4->ARR     = 100;
	
	TIM4->DIER   |= TIM_DIER_UIE;
  TIM4->CR1	   |= TIM_CR1_CEN;
	
	NVIC_EnableIRQ(TIM4_IRQn);
}

int32_t volatile K = 100;
int32_t volatile delta = 0;
float   volatile encoder = 0;
uint32_t volatile left;
uint32_t volatile right;
float volatile diff = 0;


void TIM4_IRQHandler(void) {
	
	TIM4->SR &=~ TIM_SR_UIF;
		
	encoder = encoderRead(); 
	
	delta = K * (encoder + diff);
	
	left = 150 + delta;
	right = 150 - delta;
	
	if (left > 500)  left = 500;
	if (right > 500) right = 500;
	
	motorForward(left, right);
}

int main() {
	
	tim4Begin();
	
	tim = TIM1;
	
	encoderRightBegin();
	encoderLeftBegin();
	
	motorBegin();
		
	while(1) {
		
		
	
	}
	
}