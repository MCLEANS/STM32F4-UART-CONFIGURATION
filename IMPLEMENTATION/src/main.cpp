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

custom_libraries::clock_config system_clock;

int main(void)
{
	/*
	 * Initialize system clock
	 * AHB at 168MHz
	 * APB1 at 42MHz
	 * APB2 at 84MHz
	 */
	system_clock.initialize();

	//Enable USART RCC (USART1)
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	//Set USART1 pins to Alternate Function Mode
	GPIOA->MODER |= GPIO_MODER_MODER9_1;
	GPIOA->MODER |= GPIO_MODER_MODER10_1;

	//Enable the specific alternate function
	GPIOA->AFR[1] |= (1<<4);
	GPIOA->AFR[1] |= (1<<5);
	GPIOA->AFR[1] |= (1<<6);

	GPIOA->AFR[1] |= (1<<8);
	GPIOA->AFR[1] |= (1<<9);
	GPIOA->AFR[1] |= (1<<10);

	//Set the UART baudrate(9600)
	USART1->BRR = 0x222E;

	//Enable RX, TX and UART
	USART1->CR1 |= USART_CR1_TE;
	USART1->CR1 |= USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;


	while(1){

		USART1->DR = 'A';
		while(!(USART1->SR & USART_SR_TXE));

	}
}
