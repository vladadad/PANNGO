/**
  ******************************************************************************
  * @file    random_number_generator.h
  * @brief   Header file for generating random numbers using the RNG peripheral.
  * @author  Vlad Kulikov
  * @date    23.12.2023
  ******************************************************************************
  */

#ifndef INC_RANDOM_NUMBER_GENERATOR_H_
#define INC_RANDOM_NUMBER_GENERATOR_H_

/* Includes ------------------------------------------------------------------ */
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "client_data.h"

/* Handler types ------------------------------------------------------------*/
extern RNG_HandleTypeDef 	hrng;		/**< External RNG handler for generating random numbers */

/* Define types ------------------------------------------------------------ */
#define RNG_PANGO 			&hrng		/**< Macro for generating random numbers in the get_coordinates function*/

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
void get_coordinates(struct pango_client_data *client_data);

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
void rand_uint128(uint32_t *ins_num);

#endif /* INC_RANDOM_NUMBER_GENERATOR_H_ */
