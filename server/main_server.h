#ifndef MAIN_SERVER_H
#define MAIN_SERVER_H

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <signal.h>
#include <stdint.h>
#include "client/client_thread.h"
#include "database/parking_time_db/db_update_thread.h"

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

#define RETURN_THE_DATABASE_UPDATE_THREAD 1

extern pthread_mutex_t mutex;
extern int server_sockfd;
/*D.B where all clients data is stored*/
extern sqlite3 *db_client;
/*D.B where the prices per city are stored*/
extern sqlite3 *db_prices;
extern volatile uint8_t return_thread;

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


/**
 * @brief Pango client thread function.
 *
 * This function handles communication with the Pango client, processes received data,
 * updates the database, and performs various actions based on the received information.
 *
 * @param arg A pointer to the pango_data structure.
 * @return None.
 */
void *pango_client(void *arg);

/**
 * @brief Update the database with client information.
 *
 * This function runs as a thread, periodically updating the database with client information.
 *
 * @param arg A pointer to the pango_data structure.
 * @return None.
 */
void *db_update(void *arg);

#endif /*MAIN_SERVER_H*/