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
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//Set USART1 pins to Alternate Function Mode
	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR10_0;


	//Enable the specific alternate function
	GPIOB->AFR[1] |= (1<<8);
	GPIOB->AFR[1] |= (1<<9);
	GPIOB->AFR[1] |= (1<<10);

	//GPIOA->AFR[1] |= (1<<8);
	//GPIOA->AFR[1] |= (1<<9);
	//GPIOA->AFR[1] |= (1<<10);

	//Set the UART baudrate(9600)
	USART3->BRR = 0x1117;

	//Enable RX, TX and UART
	USART3->CR1 |= USART_CR1_TE;
	USART3->CR1 |= USART_CR1_RE;
	USART3->CR1 |= USART_CR1_UE;

	while(1){

		USART3->DR = 0x4D;
		while(!(USART3->SR & USART_SR_TXE)){}



	}
}
