// Include
#include <stdint.h>
#include <stm32f446xx.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "stm32f4xx_hal.h"

// Define
#define ADDR_FLASH_SECTOR_2 (volatile uint32_t*) 0x08008000

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
	void (*p_application_function) (void);   // pointer function on user application
	uint32_t index    = 0U;
	uint8_t  rx_value = 0U;
	FLASH_EraseInitTypeDef flash_erase;
	uint32_t flash_sector_error;

	// Initialize Application function address
	p_application_function = *(ADDR_FLASH_SECTOR_2 + 1);

	// Initialize peripherals
	GPIO_Init();
	USART2_Init();

	// Check pushbutton
	if((GPIOC->IDR & (1<<13)) == (1<<13))
	{
		// Set the Vector Table offset to the Application
		SCB->VTOR = ADDR_FLASH_SECTOR_2;

		// Set the stack pointer
		__set_MSP(*(ADDR_FLASH_SECTOR_2));

		// Go to the application
		p_application_function();
	}
	else
	{
		// Unlock flash
		if (HAL_OK == HAL_FLASH_Unlock())
		{
			// Initialize erasing parameters to erase flash sector 2
			flash_erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
			flash_erase.Banks        = FLASH_BANK_1;
			flash_erase.Sector       = FLASH_SECTOR_2;
			flash_erase.NbSectors    = 1;
			flash_erase.VoltageRange = FLASH_VOLTAGE_RANGE_1;

			// Try to erase flash sector 2
			if (HAL_OK == HAL_FLASHEx_Erase(&flash_erase, &flash_sector_error))
			{
				for (index = 0U; index < 5156; index++)
				{
					// receive from USART
					rx_value = USART2_ReceiveChar();

					// Print echo on USART
					USART2_TransmitChar(rx_value);

					// Print the received paquet on the flash
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,
							          0x08008000 + index,
									  rx_value);
				}
			}
		}
	}

	// Infinite loop
	while(1)
	{
		// Bootloader code
		for(uint32_t i = 0 ; i<100000; i++);
		GPIOA->ODR ^= 1<<5;
//		printf("This is BootLoader0\r\n");
	}
}

// End of file
