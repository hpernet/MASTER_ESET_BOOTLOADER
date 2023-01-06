// Include
#include <stdint.h>
#include <stm32f446xx.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"

// Define
#define FLASH_SECTOR_2 (uint32_t*) 0x08008000

// Private functions
int __io_putchar(int ch)
{
	while(((USART2->SR)&= (1<<7))==0);
	USART2->DR = ch;
}

// Main
int main(void)
{
	// Variable declaration
	void (*p_application_function) (void);

	// Initialize Application function address
	p_application_function = *(FLASH_SECTOR_2 + 1);

	// Initialize peripherals
	GPIO_Init();
	USART2_Init();

	// Check pushbutton
	if((GPIOC->IDR & (1<<13)) == (1<<13))
	{
		// Lunch application
		printf("Launch Application\r\n");
		p_application_function();
	}

	// Infinite loop
	while(1)
	{
		// Bootloader code
		for(uint32_t i = 0 ; i<100000; i++);
		GPIOA->ODR ^= 1<<5;
		printf("This is BootLoader0\r\n");
	}
}

// End of file
