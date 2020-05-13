#include "stm32f10x.h" 
#include "string.h"

char buffer[8];
int16_t volatile dist = 0;
char tmp;
void USART2_IRQHandler(void) {
	
	if ((USART2->SR & USART_SR_RXNE) != 0) {
		tmp = USART2->DR;
		
		if (tmp == 0x59){ // CR			
			memset(buffer, 0, 8);
		}
		
		buffer[strlen(buffer)] = tmp;
	}
	
}

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

int main() {
	usart2Begin(115200);

	//usart2TransmissionByte(0x42);
	//usart2TransmissionByte(0x57);
	//usart2TransmissionByte(0x02);
	//usart2TransmissionByte(0x00);
	//usart2TransmissionByte(0x00);
	//usart2TransmissionByte(0x00);
	//usart2TransmissionByte(0x06);
	//usart2TransmissionByte(0x08);
	
	
	
	while(1) {
		
		dist = buffer[1];// + buffer[2] * 256;
		
	//	if (!strcmp(buffer, "Hi")){
		//	usart2Transmission("Hello");
			//memset(buffer, 0, 255);
	//	}
		
		
		// for(uint32_t i=0; i<240000;i++);
		//		if (tmp == 0x59){ // CR
			//buffer[0]=0x59;
			//if (tmp == 0x59) {
			//	buffer[1]=0x59;
				//for(int i=2;i<9;i++)
				//{ buffer[i]=tmp;
				//}
				//int check=buffer[0]+buffer[1]+buffer[2]+buffer[3]+buffer[4]+buffer[5]+buffer[6]+buffer[7];
				//if(buffer[8]==(check&0xff)){
				//	dist = buffer[1]+ buffer[2] * 256;
				//}
			//}				
			
		}

	}
