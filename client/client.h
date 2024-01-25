/**
 * @file 	client.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Header file containing declarations for client-related functions.
 */
#ifndef CLIENT_H 
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include "./stm/uart/uart.h"
#include "./bbb/poll_event/poll_functions.h"
#include "./bbb/server/tcp/tcp.h"
#include "./bbb/server/connection_check/server_connection_check_functions.h"
#include "png_enums.h"

#define CRC_POLYNOMIAL 				  0x8D
#define PANGO_DATA_SIZE 	 	  	   9
#define DATA_BUFF_SIZE 			PANGO_DATA_SIZE + 1
#define PAYMENT_SIZE 				   2
#define LOCATION_NAME_MAX_LEN         12
#define ERROR 						  -1

pthread_mutex_t mutex; 
extern volatile int exit_app;

/**
 * @brief Start-End function to control the application.
 *
 * This function continuously writes "Hi" to a file descriptor until the exit_app flag is set to 1.
 * It is intended for controlling the start and end of the application.
 *
 * @param arg A pointer to the file descriptor (int).
 * @return None.
 */
void *start_end_func(void *arg);

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check func. .
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculateCRC8(uint8_t* data, uint32_t length);

/**
 * @brief Check CRC-8 validity for a data buffer.
 *
 * This function checks the CRC-8 validity of the received data buffer.
 * If the CRC check fails, an error status is set.
 *
 * @param buff Data buffer to check.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the status variable.
 */
void CRC_8_check(uint8_t buff[10], uint8_t buff_size, uint8_t *status );

/**
 * @brief Retrieve the status from a data buffer.
 *
 * This function extracts the status information from the given data buffer.
 *
 * @param buff Data buffer containing status information.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the variable to store the retrieved status.
 * @return None.
 */
void get_status(uint8_t buff[10], uint8_t buff_size, uint8_t *status);

/**
 * @brief Convert received status to a boolean value.
 *
 * This function converts the received status to a boolean value.
 *
 * @param status The received status.
 * @return 1 if status is ON or OFF, 0 otherwise.
 */
uint8_t received_data_from_stm(uint8_t status);

/**
 * @brief Send data to the server and receive the client location name.
 *
 * This function sends data to the server and receives the client location name.
 *
 * @param client_socket Pointer to the client socket.
 * @param data_buff The data buffer to send.
 * @param data_buff_size The size of the data buffer.
 * @param location Pointer to store the received location name.
 * @param location_size The size of the location buffer.
 * @return 0 on success, -1 on error.
 */
int send_data_and_receive_location (int *client_socket, uint8_t *data_buff,
	uint8_t data_buff_size, uint8_t *location, uint8_t location_size);

/**
 * @brief Update the status value by toggling between 0 and 1.
 *
 * This function updates the status value by toggling between 0 and 1.
 *
 * @param status Pointer to the status value.
 * @return None.
 */
void updating_status_value(uint8_t *status);

/**
 * @brief Display the final parking information.
 *
 * This function displays the final parking information including duration and payment.
 *
 * @param payment An array containing payment information.
 * @param size The size of the payment array.
 * @return None.
 */
void final_price_for_costumer(double *payment, int size);

#endif /*CLIENT_H*/