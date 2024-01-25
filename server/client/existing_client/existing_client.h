/**
 * @file existing_client.h
 * @author  Vladislav Kulikov
 * @date    2024-01-14
 * @brief Header file containing declarations for functions related to existing clients.
 */
#ifndef EXISTING_CLIENT_H
#define EXISTING_CLIENT_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "./../new_client/new_client.h" 

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
	volatile uint8_t running; /*Indecates if the client is currently connecnted to the server and counting time*/
};
#pragma pack(pop)
#endif /*STRUCT_PANGO_DATA*/

extern sqlite3 *db_client;
extern sqlite3 *db_prices;
extern pthread_mutex_t mutex;

/**
 * @brief Retrieve client data from the database.
 *
 * This function is called when the client already exists in the database. It continues counting
 * the parking time from the last value stored in the database and updates relevant information.
 * If any critical error occurs during database operations, the function sets the status
 * to an error code and returns QUIT; otherwise, it sends the client's location and returns STAY.
 *
 * @param client_data_struct Pointer to the client data structure.
 * @param stmt Pointer to the SQLite3 statement for database operations.
 * @param status Pointer to the status variable. It is updated with ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS if an error occurs.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t retriev_client_data(void *client_data_struct, sqlite3_stmt **stmt, uint8_t *status);

/**
 * @brief Retrieve the time_start_parking value from the database.
 *
 * This function extracts the time_start_parking value from the 'your_table' table in the database.
 *
 * @param client_struct Pointer to the client data structure.
 * @param stmt_arg Pointer to the SQLite3 statement structure.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t retrieve_time_start_parking_value_from_database(void *client_struct, sqlite3_stmt *stmt_arg);

/**
 * @brief Update client information and continue counting time.
 *
 * This function updates the time, running status, and location for a client.
 * It retrieves the client's location from the 'your_table' table in the client database
 * and updates the client structure accordingly.
 *
 * @param client_struct Pointer to the structure containing client data.
 * @param stmt_arg Pointer to the SQLite statement structure.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t update_client_and_continue_time(void *client_struct, sqlite3_stmt *stmt_arg);

#endif /*#define EXISTING_CLIENT_H*/