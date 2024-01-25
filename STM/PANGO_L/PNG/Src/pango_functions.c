/**
  ******************************************************************************
  * @file    pango_functions.c
  * @brief   Source file for custom functions
  * 		 that are not using any peripheral of the STM controller.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "PNG.h"

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

void pack_pango_buffer(struct pango_client_data *client_data, uint8_t buffer_to_send[AMOUNT_OF_DATA_FOR_CRC8_CHECKSUM_VALUE])
{
	/* ^CRC - is a saved place in the buffer for the CRC8 checksum value*/
	/* buffer_to_send = [0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,^CRC ] */
	for(int i = 0; i < AMOUNT_OF_DATA_MINUS_ONE_ITERATION ; ++i)
	{
		/* When i equals 0 */
		if(i == 0)
		{
			/* buffer_to_send = [status ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,^CRC ] */
			*(buffer_to_send + i) = client_data->status;
		}
		/* When i runs from 1 to 6 */
		else if(i > 0 && i < 7)
		{
			/* buffer_to_send = [status ,MAC[0] ,MAC[1] ,MAC[2] ,MAC[3] ,MAC[4] ,MAC[5] ,0 ,0 ,^CRC ] */
			*(buffer_to_send + i) = client_data->mac_address[i - 1];
		}
		/* When i equals 7 */
		else
		{
			/* buffer_to_send = [status ,MAC[0] ,MAC[1] ,MAC[2] ,MAC[3] ,MAC[4] ,MAC[5] ,X ,0, ^CRC ] */
			*(buffer_to_send + i) = client_data->x_coor;
			/* buffer_to_send = [status ,MAC[0] ,MAC[1] ,MAC[2] ,MAC[3] ,MAC[4] ,MAC[5] ,X ,Y, ^CRC ] */
			*(buffer_to_send + (i + 1)) = client_data->y_coor;
		}
	}
}

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check function.
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculate_crc8(uint8_t* data, uint32_t length)
{
    uint8_t crc = 0xFF; // Initial value, typically 0xFF for CRC-8

    for (uint32_t i = 0; i < length; i++)
    {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}
//
//void uart_transmit_bbb(UART_HandleTypeDef *huart, const uint8_t *send_buff, uint16_t send_buff_size, uin32_t timeout, uint8_t transmition_time)
//{
//	HAL_GetTick();
//	while()
//	{
//		HAL_UART_Transmit(huart, send_buff, send_buff_size, *timeout);
//	}
//
//}
