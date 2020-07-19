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

int data = 0;

#define BUFFER_SIZE 512
char receive_buffer[BUFFER_SIZE];
int buffer_position;



void print_char(char byte){
	while(!(USART3->SR & USART_SR_TXE)){}
	USART3->DR = byte;
}

void print(char *byte){
	for(;*byte;byte++) print_char(*byte);
}

void println(char *byte){
	for(;*byte;byte++) print_char(*byte);
	print_char('\n');
}

char read_char(){
	char byte;
	byte = USART3->DR;
	return byte;
}

void read_string(){
	receive_buffer[buffer_position] = USART3->DR;
	buffer_position++;

	if(buffer_position >= BUFFER_SIZE) buffer_position = 0;

}

void flush_buffer(){
	for(int i = 0; i < BUFFER_SIZE; i++) receive_buffer[i] = 0x00;
	buffer_position = 0;
}

extern "C" void USART3_IRQHandler(void){
	if(USART3->SR & USART_SR_RXNE){
		USART3->SR &= ~USART_SR_RXNE;
		read_string();
	}

}


int main(void)
{

	system_clock.initialize();

	//Enable USART RCC (USART3)
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//Set USART3 pins to Alternate Function Mode
	//TX pin setup
	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR10_0;

	//Enable the specific alternate function
	GPIOB->AFR[1] |= (1<<8);
	GPIOB->AFR[1] |= (1<<9);
	GPIOB->AFR[1] |= (1<<10);

	//RX pin setup
	GPIOB->MODER |= GPIO_MODER_MODER11_1;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR11_0;

	GPIOB->AFR[1] |= (1<<12);
	GPIOB->AFR[1] |= (1<<13);
	GPIOB->AFR[1] |= (1<<14);



	uint16_t baud = (42000000/(9600));

	//Enable RX, TX and UART
    USART3->CR1 |= USART_CR1_TE;
	USART3->CR1 |= USART_CR1_RE;
	USART3->CR1 |= USART_CR1_UE;
	USART3->CR1 |= USART_CR1_RXNEIE;


	//Set the UART baudrate(9600)
	USART3->BRR = ( ( ( baud / 16 ) << 4) |
            ( ( (baud % 16 )<< 0) ) );

	NVIC_SetPriority(USART3_IRQn,0x03);
	NVIC_EnableIRQ(USART3_IRQn);

	char name[] = "MCLEANS";


	while(1){

		if(strncmp(receive_buffer,name,(sizeof(name)/sizeof(char))-1) == 0){
			println("JACK");
			flush_buffer();
		}

	}
}
