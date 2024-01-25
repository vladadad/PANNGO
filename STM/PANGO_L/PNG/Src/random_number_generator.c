/**
  ******************************************************************************
  * @file    random_number_generator.c
  * @brief   Source file for generating random numbers using the RNG peripheral.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "random_number_generator.h"

/**
 * @brief Generates random coordinates and assigns them to a pango_client_data structure.
 *
 * This function generates random x and y coordinates using the rand_uint128 function
 * and assigns them to the x_coor and y_coor fields, respectively, in the pango_client_data structure.
 *
 * @param client_data Pointer to the pango_client_data structure to store the generated coordinates.
 *
 * The function uses the rand_uint128 function to generate random 32-bit unsigned integers
 * and assigns them to the x_coor and y_coor fields in the client_data structure.
 */
void get_coordinates(struct pango_client_data *client_data)
{
	uint32_t temp = 0;

	/* Generating a random number for x_coor */
	rand_uint128(&temp);
	client_data->x_coor = temp;

	/* Generating a random number for y_coor */
	rand_uint128(&temp);
	client_data->y_coor = temp;
}

/**
 * @brief Generates a random 32-bit unsigned integer and restricts it to the range [0, 127].
 *
 * This function utilizes the Hardware Abstraction Layer (HAL) to generate a random number
 * using the hardware Random Number Generator (RNG). The generated number is then
 * reduced to fit within the range [0, 127].
 *
 * @param insert_num Pointer to a variable to store the generated random number.
 *
 * The function handles errors in the RNG generation by printing an error message
 * and entering an infinite loop if the HAL_RNG_GenerateRandomNumber operation fails.
 * The final random number is stored at the address pointed to by insert_num.
 */
void rand_uint128(uint32_t *insert_num)
{
	HAL_StatusTypeDef status = 0;

	status = HAL_RNG_GenerateRandomNumber(RNG_PANGO, insert_num);
	if(status != HAL_OK)
	{
		perror("HAL_RNG_GenerateRandomNumber");
		printf("\r\n");

		/* Entering an infinite loop */
		Error_Handler();
	}
	/* inserting the random number inside the address and restricts it to the range [0, 127]*/
	*insert_num =(uint8_t)(*insert_num % 128);
}
