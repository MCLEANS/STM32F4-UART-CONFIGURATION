/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "clockconfig.h"
#include <string.h>

custom_libraries::clock_config system_clock;

#define BUFFER_SIZE 512
char receive_buffer[BUFFER_SIZE];
int buffer_position = 0;
int data = 0;

char receive_char(){
	char byte;
	byte = USART3->DR;
	return byte;
}

void receive_string(){
	for(int i = 0; i < BUFFER_SIZE; i++,buffer_position++){
		receive_buffer[buffer_position] = receive_char();
		if(buffer_position >= BUFFER_SIZE) buffer_position = 0;
	}
}

void flush_buffer(){
	for(int i = 0; i < BUFFER_SIZE; i++) {
		receive_buffer[i] = 0x00;
	}
	buffer_position = 0;
}

void send_char(char byte){
	while(!(USART3->SR & USART_SR_TXE)){}
	USART3->DR = byte;
}

void send_string(char *byte){
	for(;*byte;byte++){
		send_char(*byte);
	}
}

extern "C" void USART3_IRQHandler(){
	data++;
}

int main(void)
{

	system_clock.initialize();
	//Enable USART RCC (USART3)
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//Set USART3 pins to Alternate Function Mode
	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR10_0;

	//Enable the specific alternate function
	GPIOB->AFR[1] |= (1<<8);
	GPIOB->AFR[1] |= (1<<9);
	GPIOB->AFR[1] |= (1<<10);


	uint16_t baud = (42000000/(9600));

	//Enable RX, TX and UART
    USART3->CR1 |= USART_CR1_TE;
	USART3->CR1 |= USART_CR1_RE;
	USART3->CR1 |= USART_CR1_UE;
	USART3->CR2 |= USART_CR1_RXNEIE;

	//Set the UART baudrate(9600)
	USART3->BRR = ( ( ( baud / 16 ) << 4) |
            ( ( (baud % 16 )<< 0) ) );

	NVIC_SetPriority(USART3_IRQn,0x00);
	NVIC_EnableIRQ(USART3_IRQn);


	char name[] = "JACK MCLEANS";
	send_string(name);


	while(1){
		/*
		if(strncmp(receive_buffer,name,sizeof(name)/sizeof(char) -1) == 0){
			flush_buffer();
			send_string("GOT IT");
			send_char('\n');
		}*/
	}
}
