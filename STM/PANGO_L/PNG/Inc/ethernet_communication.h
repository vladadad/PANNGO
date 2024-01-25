/**
  ******************************************************************************
  * @file    ethernet_communication.h
  * @brief   Header file which using the ETH peripheral
  * 		 to retrieve the MAC address of the controller.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#ifndef INC_ETHERNET_COMMUNICATION_H_
#define INC_ETHERNET_COMMUNICATION_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/* Handler types ------------------------------------------------------------*/
extern ETH_HandleTypeDef 	  heth;			/**< External ETH handler for Ethernet communication */

/* Define types ------------------------------------------------------------ */
#define ETH_HANDLER  		  &heth			/**< Macro for extracting the controllers 'MAC address' */
#define MAC_ADDRESS_LENGTH      6 			/**< Length of a MAC address array. */

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
void get_mac_address(uint8_t *mac_address_array);

#endif /* INC_ETHERNET_COMMUNICATION_H_ */
