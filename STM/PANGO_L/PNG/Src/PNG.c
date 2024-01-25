/**
  ******************************************************************************
  * @file    PNG.c
  * @brief   Source file.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#include "PNG.h"

volatile uint8_t interrupt_flag = OFF;

/* Represents the number of times a received status value was different than
 * the ones the program knows */
volatile uint8_t wrong_status = 1;

/* A struct that will hold the received data from the BBB controller */
struct pango_client_data pango_client_data = {0};

/**
  * @brief Main function for the PNG module.
  * @return Returns 0 on successful completion.
  */
int main_PNG(void)
{
	/* A buffer that will hold the data to be transmitted to the BBB controller */
	uint8_t send_buff[BUFFER_SIZE_TO_SEND] = {0};
	HAL_StatusTypeDef status;
	printf("Start of the program\r\n\n");

	/* Getting the MAC ADDREESS of the STM controller */
	get_mac_address(pango_client_data.mac_address);

	/* Interrupt waits for a status value from the BBB */
	status = HAL_UART_Receive_IT(UART_4, &(pango_client_data.status), sizeof(pango_client_data.status));
	if(status != HAL_OK)
	{
		perror("HAL_UART_Receive_IT");
		exit(1);
	}

	puts("1\r\n");
	while(1)
	{
		/* Loops while a data received interrupt occurs */
		if(interrupt_flag == ON)
		{
			printf("Inside the flag\r\n");

			/* If the CRC-8 check was faulty the nucleos system will restart */
			if(pango_client_data.status == RESTART)
			{
				/* BBB controller send a restarting the system value */
				printf("Restarting because of BBB request\r\n\n");
				NVIC_SystemReset();
			}

			/* Generates random coordinates for the BBB */
			get_coordinates(&pango_client_data);

			/* Data in pango_client_data is stored in send_buff */
			pack_pango_buffer(&pango_client_data, send_buff);

			/* Getting the CRC-8 value */
			send_buff[PLACE_FOR_CRC8_VALUE] = calculate_crc8(send_buff, AMOUNT_OF_DATA_FOR_CRC8_CHECKSUM_VALUE);

			/* Sending all the data to the STM */
			status = HAL_UART_Transmit(UART_4,send_buff, sizeof(send_buff), SMALL_DELAY);
			if(status != HAL_OK)
			{
				perror("HAL_UART_Receive");
				printf("\r\n");
				/* BBB controller send a restarting the system value */
				NVIC_SystemReset();
			}
			/* Here we initialize again the receiving interrupt */
			status = HAL_UART_Receive_IT(UART_4, &(pango_client_data.status), sizeof(pango_client_data.status));
			if(status != HAL_OK)
			{
				perror("HAL_UART_Receive_IT");
				exit(1);
			}
			/* Reseting the flag */
			interrupt_flag = OFF;
		}
	}
	return 0;
}

/* Runs this function when the data received interrupt occurs for HAL_UART_Receive_IT(UART_4,...)*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	HAL_StatusTypeDef status;
	if(wrong_status % 5 == 0)
	{
		uint8_t hardware_error = HARDWARE_ERROR;
		printf("Restart the 'STM Controller' with the restart button\r\n");
		HAL_UART_Transmit(UART_4,(uint8_t*)hardware_error, sizeof(hardware_error), SMALL_DELAY);
		Error_Handler();
	}
	/* When there is hardware error in the STM or the BBB most likely a cable fault */
	switch(pango_client_data.status)
	{
		case ON:
			interrupt_flag = ON;
			break;
		case OFF:
			interrupt_flag = ON;
			break;
		case RESTART:
			interrupt_flag = ON;
			break;
		default:
			++wrong_status;
			interrupt_flag = OFF;
			printf("status  = %d\r\n", pango_client_data.status);
			status = HAL_UART_Receive_IT(UART_4, &(pango_client_data.status), sizeof(pango_client_data.status));
			if(status != HAL_OK)
			{
				perror("HAL_UART_Receive_IT");
				exit(1);
			}
			break;
	}
}
