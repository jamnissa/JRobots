#include "stm32f10x.h" 
#include "string.h"


/* lidar begin */
//void usart2Begin(uint32_t baudrate) {
//	
//	uint32_t _BRR = 72000000 / (baudrate * 2);
//	
//	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
//	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
//	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
//	
//	GPIOA->CRL &=~ (GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
//	GPIOA->CRL |=  (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_1); //TX
//	
//	GPIOA->CRL &=~ (GPIO_CRL_CNF3 | GPIO_CRL_MODE3); 
//	GPIOA->CRL |= GPIO_CRL_CNF3_0; // RX
//	
//	USART2->BRR = _BRR;
//	USART2->CR1 = 0x0;
//	USART2->CR2 = 0x0;
//	
//	USART2->CR1 |= USART_CR1_TE;
//	USART2->CR1 |= USART_CR1_RE;
//	USART2->CR1 |= USART_CR1_UE;
//	
//	USART2->CR1 |= USART_CR1_RXNEIE;
//	NVIC_EnableIRQ(USART2_IRQn);
//}

//char USART2_BUFFER[8];
//uint16_t i = 0;

//void USART2_IRQHandler(void) {
//	
//	if ((USART2->SR & USART_SR_RXNE) != 0) { 
//	  
//		char tmp = USART2->DR;
//		
//		if(tmp == 0x59) {
//			  i = 0;	
//			  memset(USART2_BUFFER, 0, 8);
//				USART2_BUFFER[i++] = tmp;
//		} else {
//				USART2_BUFFER[i++] = tmp;
//		}			
//	}
//}

//void usart2Transmission(char* str) {
//	
//	for(uint16_t i=0; i < strlen(str); i++) {
//		while(!(USART2->SR & USART_SR_TC));
//		USART2->DR = str[i];
//	}
//	
//	while(!(USART2->SR & USART_SR_TC));
//	USART2->DR = '\n';
//}

//void usart2TransmissionByte(uint8_t byte) {
//		while(!(USART2->SR & USART_SR_TC));
//		USART2->DR = byte;
//}

/* lidar end */


/* driver begin */

void pwmBegin(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOA->CRL |= (GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0);
	
	GPIOA->CRL &=~ (GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOA->CRL |=  (GPIO_CRL_CNF1_1 | GPIO_CRL_MODE1);
		
	TIM2->PSC = 32; // 72000000 / 72 / 100 10k
	TIM2->ARR = 100;
	
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

void motorLeft(uint16_t pwm) {
	 motorPwm(pwm, pwm*0.3);
	 
	 GPIOA->BSRR = GPIO_BSRR_BR5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BS4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BS0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BR1; // BIN2
}

void motorRight(uint16_t pwm) {
	 motorPwm(pwm, pwm);
	 
	 GPIOA->BSRR = GPIO_BSRR_BS5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BR4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BR0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BS1; // BIN2
}

void motorForward(uint16_t pwm) {
	 motorPwm(pwm, pwm);
	 
	 GPIOA->BSRR = GPIO_BSRR_BS5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BR4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BS0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BR1; // BIN2
}

void motorBack(uint16_t pwm) {
	 motorPwm(pwm, pwm);
	 
	 GPIOA->BSRR = GPIO_BSRR_BR5; // AIN1 
	 GPIOA->BSRR = GPIO_BSRR_BS4; // AIN2
	 
	 GPIOB->BSRR = GPIO_BSRR_BR0; // BIN1
	 GPIOB->BSRR = GPIO_BSRR_BS1; // BIN2
}

/* driver end */

TIM_TypeDef * tim;


/* bluetooth begin */
void usart2Begin(uint32_t baudrate) {
	
	uint32_t _BRR = 72000000 / (baudrate * 2);
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &=~ (GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
	GPIOA->CRL |=  (GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_1); //TX
	
	GPIOA->CRL &=~ (GPIO_CRL_CNF3 | GPIO_CRL_MODE3); 
	GPIOA->CRL |= GPIO_CRL_CNF3_0; // RX
	
	USART2->BRR = _BRR;
	USART2->CR1 = 0x0;
	USART2->CR2 = 0x0;
	
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_UE;
	
	USART2->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART2_IRQn);
}

char USART2_BUFFER[10];
uint16_t i = 0;
char string [10];
void usart2Receive(void) {
	if (!strcmp(USART2_BUFFER,"left50\r")) {
      motorLeft(50);
	}		
	if (!strcmp(USART2_BUFFER,"right50\r")) {
      motorRight(50);
	}	
	if (!strcmp(USART2_BUFFER,"right20\r")) {
      motorRight(20);
	}
}

void USART2_IRQHandler(void) {
	
	if ((USART2->SR & USART_SR_RXNE) != 0) { 
	  
		char tmp = USART2->DR;
		
		if(tmp == 0x0A) {
		   i = 0;	
			 usart2Receive();
			 memset(USART2_BUFFER, 0, 10);
			
		} else {
				USART2_BUFFER[i] = tmp;
			  string[i] = USART2_BUFFER[i];
			  i++;
		}			
	}
}



void usart2Transmission(char* str) {
	
	for(uint16_t i=0; i < strlen(str); i++) {
		while(!(USART2->SR & USART_SR_TC));
		USART2->DR = str[i];
	}
	
	while(!(USART2->SR & USART_SR_TC));
	USART2->DR = '\n';
}

void usart2TransmissionByte(uint8_t byte) {
		while(!(USART2->SR & USART_SR_TC));
		USART2->DR = byte;
}

/* bluetooth end */


int main() {
	tim = TIM2;

	
	motorBegin();
	volatile uint16_t speed = 30;
	
	usart2Begin(115200);
	usart2Transmission("hi"); 
	
	while(1) { 
//		motorLeft(speed);
//		for(uint32_t i=0; i<24000000; i++);
//		
//		motorRight(speed);
//		for(uint32_t i=0; i<24000000; i++);
//		motorForward(speed);
//		for(uint32_t i=0; i<24000000; i++);
//		
//    motorBack(speed);
//    for(uint32_t i=0; i<24000000; i++);
	}
}	
