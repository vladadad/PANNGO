/**
  ******************************************************************************
  * @file    PNG.h
  * @brief   Header file that contains all the needed includes and
  *  		 defines to run the program.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  *
  */

#ifndef INC_PNG_H_
#define INC_PNG_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "stm32f7xx_hal.h"				/**< Include that enables the use of NVIC_SystemReset function */
#include "random_number_generator.h"	/**< Include that enables the use of RNG peripheral and functions depending on it */
#include "uart_communication.h"			/**< Include that enables the use of UART peripheral and functions depending on it */
#include "ethernet_communication.h"		/**< Include that enables the use of ETH peripheral and functions depending on it */



/**
 * @brief Enumeration representing the restart flag states.
 *
 * This enumeration defines two states: ON and OFF, representing logical states.
 */
typedef enum
{
	ON = 1,								/**< Represents the logical state 'ON' of the client*/
	OFF = 2,							/**< Represents the logical state 'OFF' of the client*/
	RESTART = 3,						/**< Restarting the STM Controller because of BBB problem */
	HARDWARE_ERROR = 255				/**< Problem with the STM hardware */
} restart_flag;

/**
 * @brief Macro definitions for functions.c file.
 */
#define CRC_POLYNOMIAL         0x8D			/**< Polynomial used for CRC-8 calculations */
#define RESTART_NUCLEO_BOARD    4			/**< Value that restarts the controller, is send from the BBB in case of a CRC-8 checksum fail */
#define AMOUNT_OF_DATA_MINUS_ONE_ITERATION                 					 8 					/**< Represents the amount of data minus one in a loop iteration for packing */
#define PLACE_FOR_CRC8_VALUE                				(uint8_t)(BUFFER_SIZE_TO_SEND - 1) 	/**< Index indicating the position in the buffer where the CRC-8 value is stored */
#define AMOUNT_OF_DATA_FOR_CRC8_CHECKSUM_VALUE              (uint8_t)(BUFFER_SIZE_TO_SEND - 1) 	/**< Amount of data used for CRC-8 checksum calculation */
/**
  * @brief Main function for the PNG module.
  * @return Returns 0 on successful completion.
  */
int main_PNG(void);

/**
 * @brief Packs data from a pango_client_data structure into a buffer for transmission.
 *
 * This function prepares a buffer with specific data from the pango_client_data structure,
 * arranging it for transmission with a provision for a CRC8 checksum.
 *
 * @param client_data Pointer to the pango_client_data structure containing data to be packed.
 * @param buffer_to_send Array to store the packed data along with space for CRC8 checksum.
 *
 * The structure of the packed buffer is as follows:
 * - Element 0: Status value from the pango_client_data structure.
 * - Elements 1 to 6: MAC address (MAC[0] to MAC[5]) from the pango_client_data structure.
 * - Element 7: X-coordinate (x_coor) from the pango_client_data structure.
 * - Element 8: Y-coordinate (y_coor) from the pango_client_data structure.
 * - Element 9: Reserved for CRC8 checksum.
 *
 * The function ensures correct positioning of the structure's data within the buffer.
 */
void pack_pango_buffer(struct pango_client_data *client_data, uint8_t buffer_to_send[AMOUNT_OF_DATA_FOR_CRC8_CHECKSUM_VALUE]);

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check function.
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculate_crc8(uint8_t* data, uint32_t length);

#endif /* INC_PNG_H_ */
