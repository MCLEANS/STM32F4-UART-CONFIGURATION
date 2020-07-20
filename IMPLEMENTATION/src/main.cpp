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
#include "USART.h"

custom_libraries::clock_config system_clock;
custom_libraries::USART serial(USART1,GPIOA,10,9,9600);

extern "C" void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){
		USART1->SR &= ~USART_SR_RXNE;
		serial.read_string();
	}
}


int main(void)
{

	system_clock.initialize();
	serial.initialize();

	NVIC_SetPriority(USART1_IRQn,0x03);
	NVIC_EnableIRQ(USART1_IRQn);

	char test_response[] = "RESPONSE";
	char test_querry[] = "QUERRY";

	while(1){
		if(strncmp(serial.receive_buffer,test_querry,(sizeof(test_querry)/sizeof(char))-1) == 0){
			serial.println(test_response);
			serial.flush_buffer();
		}

	}
}
