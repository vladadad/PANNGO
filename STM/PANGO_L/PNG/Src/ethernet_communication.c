/**
  ******************************************************************************
  * @file    ethernet_communication.c
  * @brief   Header file which using the ETH peripheral
  * 		 to retrieve the MAC address of the controller.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#include "ethernet_communication.h"

/**
 * @brief Retrieves the MAC address from the Ethernet controller and stores it in an array.
 *
 * This function initializes the Ethernet controller using HAL_ETH_Init and retrieves
 * the MAC address from the initialized controller, storing it in the specified array.
 *
 * @param mac_address_array Pointer to an array to store the retrieved MAC address.
 *
 * The function handles errors during the Ethernet controller initialization by printing
 * an error message and entering an infinite loop if the HAL_ETH_Init operation fails.
 * The MAC address is then obtained from the initialized controller and stored in the array.
 */
void get_mac_address(uint8_t *mac_address_array)
{
	HAL_StatusTypeDef status;

	status = HAL_ETH_Init(ETH_HANDLER);
	if(status != HAL_OK)
	{
		perror("HAL_ETH_Init");
		printf("\r\n");
		/* Entering an infinite loop */
		Error_Handler();
	}
	/* Getting the MAC address and storing it inside the array */
	for(int i = 0; i < MAC_ADDRESS_LENGTH; ++i)
	{
		mac_address_array[i] = *(ETH_HANDLER.Init.MACAddr[i]);
	}
}
