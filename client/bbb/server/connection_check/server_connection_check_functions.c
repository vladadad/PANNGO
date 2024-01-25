/**
 * @file 	server_connection_check_functions.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Implementation of server connection check functions.
 *
 * @copyright Copyright (c) 2024
 */
#include "server_connection_check_functions.h"

/**
 * @brief Checks if the device is already connected to the server.
 *
 * @param status Status indicator (1 for success, 0 for failure).
 * @param connected Connection status indicator (1 for connected, 0 for not connected).
 * @return CONNECTED if already connected, NOT_CONNECTED if not connected.
 */
int already_connected_to_server_check(uint8_t status, uint8_t connected)
{
	if ((status == ON) && (connected == NOT_CONNECTED))
	{
		return NOT_CONNECTED;
	}
	return CONNECTED;
}

/**
 * @brief Checks if the device is ready to quit the connection.
 *
 * @param status Status indicator (1 for success, 0 for failure).
 * @param connected Connection status indicator (1 for connected, 0 for not connected).
 * @return QUIT if ready to quit, DONT_QUIT if not ready to quit.
 */
int ready_to_quit(uint8_t status, uint8_t connected)
{
	if ((status == OFF) && (connected == CONNECTED))
	{
		return QUIT;
	}
	return DONT_QUIT;
}

/**
 * @brief Handles the CRC-8 server-side value and updates the status.
 *
 * This function returns a string indicating the value of the CRC-8 check on the server side.
 * In case of different CRC-8 values between the server and the BBB, it returns "ERROR".
 * When the server's CRC-8 value is identical to the BBB's CRC-8 value, it updates the status and
 * returns the current city name where the client parked.
 *
 * @param status Pointer to the status variable.
 * @param location Pointer to the location string.
 * @param location_size Size of the location string.
 * @param connected Pointer to the connected variable.
 */
void crc8_server_side_value(uint8_t *status, char *location_buff, uint8_t location_buff_size, uint8_t *connected)
{
	/* Returns a string indicating the value of
		the CRC-8 check on the server side.  */
	char loacation[location_buff_size];
	memset((char *)loacation, '\0', location_buff_size);
	memcpy(loacation, location_buff, location_buff_size);

	/* In case of a problem on the server side.
	   the server returns the string 'ERROR'.  */
	if (strcmp(loacation, "ERROR") != 0)
	{
		printf("Your location is %s.\nThe parking has started.\n\n\n", loacation);
		/* Avoiding creating another connection on the next button press.  */
		*connected = CONNECTED;
	}
	else
	{
		*status = STAY_ON;
		*connected = NOT_CONNECTED;
		printf("\nA problem occurred on the server side.\n");
		printf("Please try connecting again.\n\n");
	}
}