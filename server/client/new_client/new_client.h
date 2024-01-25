/**
 * @file new_client.h
 * @author  Vlad Kulikov
 * @brief   Implementation of functions related to processing new clients.
 * @brief Header file containing declarations for functions related to new clients.
 */
#ifndef NEW_CLIENT_H
#define NEW_CLIENT_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifndef LOOP_STATUS
#define LOOP_STATUS
enum loop_status
{
	QUIT = 0,
	STAY = !QUIT
};
#endif /*LOOP_STATUS*/

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

#ifndef STATEMENT_STATUS
#define STATEMENT_STATUS
enum statement_status
{
	FALSE = 0,
	TRUE = !FALSE
};
#endif /*STATEMENT_STATUS*/

#ifndef STRUCT_PANGO_DATA
#define STRUCT_PANGO_DATA
#pragma pack(push,1)
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
 * @brief Process client data including initialization, database operations, and location sending.
 *
 * This function performs the following steps:
 * - Initializes and retrieves the start time for the client.
 * - Acquires a mutex to protect shared resources.
 * - Executes database operations, including retrieving parking prices and inserting client data.
 * - Sends the client's location.
 *
 * If any critical error occurs during database operations, the function sets the status
 * to an error code and returns, otherwise, it continues with sending the location.
 *
 * @param client_data_struct A pointer to the structure holding client data.
 * @param stmt A pointer to the MySQL statement for database operations.
 * @param status Pointer to the status variable to be updated based on the success of the operation.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t process_client_data(void *client_data_struct, sqlite3_stmt **stmt, uint8_t *status);

/**
 * @brief Initialize and get the start time for the client.
 *
 * This function initializes the start time for the client using the current system time
 * and retrieves the client's location using the location_func.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 */
void initialize_and_get_start_time(void *client_data_struct);

/**
 * @brief Retrieve the price from the database based on the client's location.
 *
 * This function constructs and executes an SQL statement to retrieve the price
 * associated with the specified city from the 'city_parking' table in the database.
 * The retrieved price is then stored in the client structure.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @param stmt_arg Pointer to the SQLite statement structure.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t retrieve_parking_price_per_city_from_database(void *client_data_struct, sqlite3_stmt *stmt_arg);

/**
 * @brief Insert client data into the database.
 *
 * This function inserts received client data into the 'your_table' table in the client database.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t insert_client_data_into_database(void *client_data_struct);

/**
 * @brief Send the client's current location to the client.
 *
 * This function sends the name of the city the client is currently at to the client using the send function.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @return QUIT if there is an error during the send operation, otherwise, returns void.
 */
uint8_t send_client_location(void *client_data_struct);

/**
 * @brief Determine location based on coordinates.
 *
 * This function determines the location based on the given x and y coordinates.
 * If the coordinates are out of range, an error message is stored in the provided buffer.
 *
 * @param x X-coordinate (0-127).
 * @param y Y-coordinate (0-127).
 * @param buff Buffer to store the location or error message.
 */
void location_func(uint8_t x, uint8_t y, char *buff);

#endif /*NEW_CLIENT_H*/