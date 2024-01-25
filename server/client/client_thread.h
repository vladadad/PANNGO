/**
 * @file 	client_thread.h
 * @brief 	Header file containing declarations for client-thread functionality.
 * @author  Vlad Kulikov
 * @date    2024-01-06
 */
#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sqlite3.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "./new_client/new_client.h"
#include "./existing_client/existing_client.h"

#ifndef COMMON_DEFINES
#define COMMON_DEFINES

#define SERVER_MAX_NUM_CLIENTS 10
#define CRC8_TEST_FAILED 255
#define ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS 254
#define ERROR_STATUS_IN_NEW_CLIENT_SUBFUNCTIONS 253
#define CLOSE_APP_ERROR 252
#define ERROR 251
#define MAC_ADDRESS_SIZE 18
#define ERROR_MESSAGE_SIZE 5
#define MAX_BUFF_SIZE 150

#endif /*COMMON_DEFINES*/

#define CRC_POLYNOMIAL 0x8D
#define CLIENT_DATA_BUFFER_SIZE 10
#define STATUS_INITIAL_VALUE 2
#define PANGO_DATA_SIZE (CLIENT_DATA_BUFFER_SIZE - 1)

#ifndef FLAG_STATE
#define FLAG_STATE
enum flag_state
{
	ON = 1,
	OFF = 0,
};
#endif /*FLAG_STATE*/

#ifndef APP_STATUS
#define APP_STATUS
enum app_status
{
	START_APP = 1,
	CLOSE_APP = 2,
	CONNECTION_LOST = 3,
};
#endif /*APP_STATUS*/

#ifndef STATEMENT_STATUS
#define STATEMENT_STATUS
enum statement_status
{
	FALSE = 0,
	TRUE = !FALSE
};
#endif /*STATEMENT_STATUS*/

#ifndef LOOP_STATUS
#define LOOP_STATUS
enum loop_status
{
	QUIT = 0,
	STAY = !QUIT
};
#endif /*LOOP_STATUS*/

#ifndef STRUCT_PANGO_DATA
#define STRUCT_PANGO_DATA
#pragma pack(push, 1)
struct pango_data
{
	uint8_t status; /*A flag that indicates if the application has started or ended*/
	char mac_address[MAC_ADDRESS_SIZE];
	uint8_t x_axis;
	uint8_t y_axis;
	uint8_t client_fd;
	char location[12]; /*Where the clients location name will be stored*/
	double price;
	int time_start_parking;	  /*The time the client started to use the application*/
	volatile uint8_t connected; /*Indecates if the client is currently connecnted to the server and counting time*/
};
#pragma pack(pop)
#endif /*STRUCT_PANGO_DATA*/

extern sqlite3 *db_client;
extern sqlite3 *db_prices;
extern pthread_mutex_t mutex;

/**
 * @brief Wait for data from the client and handle disconnection.
 *
 * This function waits for data from the client specified by the provided
 * structure pointer. It also handles the case where the client disconnects
 * unexpectedly.
 *
 * @param client_arg Pointer to the structure containing client data.
 * @param status Pointer to the status variable to be updated based on connection status.
 * @param client_buff Pointer to the buffer to store received client data.
 * @param client_buff_size Size of the client data buffer.
 * @return CONNECTION_LOST if the client disconnects unexpectedly, 0 otherwise.
 */
uint8_t wait_for_data_from_client(void *client_arg, uint8_t *status, uint8_t *client_buff, uint8_t client_buff_size);

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check function.
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculateCRC8(uint8_t *data, uint32_t length);

/**
 * @brief Check CRC-8 validity for a data buffer.
 *
 * This function checks the CRC-8 validity of the received data buffer.
 * If the CRC check fails, an error status is set.
 *
 * @param buff Data buffer to check.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the status variable. It is updated with CRC8_TEST_FAILED if the CRC check fails.
 *
 * @return TRUE if the CRC-8 check is successful, FALSE otherwise.
 */
uint8_t CRC_8_check(uint8_t buff[10], uint8_t buff_size, uint8_t *status);

/**
 * @brief Store client data in the struct: client_data_struct.
 *
 * This function extracts relevant information from the client data buffer and
 * stores it in the struct: client_data_struct.
 *
 * @param status Pointer to the status variable indicating data integrity.
 * @param client_buff Pointer to the buffer containing client data.
 * @param client_buff_size Size of the client data buffer.
 * @param client_arg Pointer to the structure for storing client data.
 */
void store_client_data_in_struct(uint8_t *status, uint8_t *client_buff,
								 uint8_t client_buff_size, void *client_arg);

/**
 * @brief Check the existence of a client in the database based on MAC address.
 *
 * This function prepares an SQL statement to check if a client with the specified
 * MAC address exists in the database.
 *
 * @param mac_address_buff Pointer to the buffer containing the MAC address.
 * @param mac_addres_size Size of the MAC address buffer.
 * @param stmt_arg Pointer to the SQLite3 statement object (output parameter).
 */
void clinet_exist_in_database_check_preparation(uint8_t *mac_address_buff,
												uint8_t mac_addres_size,
												sqlite3_stmt **stmt_arg);

/**
 * @brief Check the result of a client existence check in the database.
 *
 * This function checks the result of a SQLite statement execution to determine
 * if a client exists in the database.
 *
 * @param stmt_arg Pointer to the SQLite3 statement object.
 * @param allrdy_chckd Pointer to a flag indicating if the client has already been checked.
 * @return TRUE if the client exists and has not been checked already, FALSE otherwise.
 */
uint8_t clinet_exist_in_database_check(uint8_t *mac_address_buff,
									   uint8_t mac_addres_size,
									   uint8_t *allrdy_chckd,
									   sqlite3_stmt **stmt_arg);

/**
 * @brief Check if a new client based on the checked database value.
 *
 * This function determines whether a new client is present based on the checked database value.
 * If the checked database value is FALSE, it returns TRUE, indicating a new client.
 * Otherwise, it returns FALSE, indicating an existing client.
 *
 * @param checked_database_value The value indicating whether the client exists in the database (TRUE or FALSE).
 *
 * @return TRUE if the client is new, FALSE if the client already exists in the database.
 */
uint8_t new_client(uint8_t checked_database_value);

/**
 * @brief Update client data in the database based on the parking duration.
 *
 * This function calculates the parking duration, updates the database with the calculated
 * time used, and sets the provided 'end' parameter with the current time.
 *
 * @param end Pointer to the variable that will be updated with the current time.
 * @param client_struct Pointer to the client data structure.
 */
uint8_t update_client_data(uint32_t *end, void *client_data_struct);

/**
 * @brief Calculate and send payment data to the client.
 *
 * This function calculates the parking duration and payment amount based on the start and end times
 * and sends the payment data to the client.
 *
 * @param start_time Start time of parking.
 * @param end_time End time of parking.
 * @param parking_price_per_second Price per second of parking.
 * @param client_fd File descriptor of the client.
 */
void calculate_and_send_payment_data(int start_time, int end_time, double parking_price_per_second, uint8_t client_fd);

/**
 * @brief Remove client data from the database based on MAC address.
 *
 * This function deletes all data related to a client from the database using its MAC address.
 *
 * @param mac_address_arg Pointer to the MAC address.
 * @param mac_address_size Size of the MAC address.
 */
void remove_client_data(uint8_t *mac_address_arg, uint8_t mac_address_size);

#endif /*CLIENT_THREAD_H*/