/**
 * @file 	db_update_thread.h
 * @author 	Vlad Kulikov
 * @brief 	Header file containing declarations for the database update thread functionality.
 * @date 2024-01-15
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef DB_UPDATE_THREAD_H
#define DB_UPDATE_THREAD_H

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <unistd.h>

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
	int time_start_parking;		/*The time the client started to use the application*/
	volatile uint8_t connected; /*Indecates if the client is currently connecnted to the server and counting time*/
};
#pragma pack(pop)
#endif /*STRUCT_PANGO_DATA*/

/* In case you want to change the number of clients, 
you need to also change the max number of clients in the main_server.h*/
#define UPDATE_THREAD_MAX_NUM_CLIENTS 10
#define NUMBER_OF_SECONDS_BETWEEN_BACKUPS 5


#ifndef STATEMENT_STATUS
#define STATEMENT_STATUS
enum statement_status
{
	FALSE = 0,
	TRUE = !FALSE
};
#endif /*STATEMENT_STATUS*/

extern sqlite3 *db_client;
extern sqlite3 *db_prices;
extern pthread_mutex_t mutex;
extern volatile uint8_t return_thread;
extern int flag;

/**
 * @brief Set the start time for database backup.
 *
 * Initializes the time when the backup of the database starts.
 *
 * @param start_database_backup Pointer to the variable that will store the start time.
 */
void set_start_time(int *start_database_backup);

/**
 * @brief Check if it is time to backup the database.
 *
 * Checks if it is time to backup the database based on the predefined interval.
 *
 * @param database_backup_start_time Start time of the database backup.
 * @param back_up_flag Pointer to the flag indicating if it is time to backup.
 */
void time_to_backup(int database_backup_start_time, uint8_t *back_up_flag);

/**
 * @brief Update clients' data in the database.
 *
 * Updates clients' data in the database, specifically the TIME_USED value, for clients currently using the application.
 *
 * @param address_buffer_of_clients_data_structs Pointer to the buffer containing clients' data structures.
 * @param is_backup_time Flag indicating if it is time to perform the backup.
 */
void update_clients(void *address_buffer_of_clinets_data_structs, uint8_t is_backup_time);

/**
 * @brief Perform actions before returning from the database update thread.
 *
 * This function is called before returning from the database update thread.
 * It sets flags and performs necessary actions before exiting the thread.
 *
 * @param return_flag Flag indicating if the function should perform its actions.
 * @param address_buffer_of_clients_data_structs Pointer to the buffer containing clients' data structures.
 * @param is_backup_time Pointer to the flag indicating if it is time to perform the backup.
 * @param quit_loop_flag Pointer to the flag indicating if the thread should quit the main loop.
 */
void return_database_update_thread(uint8_t return_flag, void *address_buffer_of_clinets_data_structs, uint8_t *is_backup_time, uint8_t *quit_loop_flag);

#endif /* DB_UPDATE_THREAD_H */