/**
 * @file 	server_connection_check_functions.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Header file for server connection check functions.
 */
#ifndef SERVER_CONNECTION_CHECK_H
#define SERVER_CONNECTION_CHECK_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "./../../../png_enums.h"

/**
 * @brief Checks if the device is already connected to the server.
 *
 * @param status Status indicator (1 for success, 0 for failure).
 * @param connected Connection status indicator (1 for connected, 0 for not connected).
 * @return CONNECTED if already connected, NOT_CONNECTED if not connected.
 */
int already_connected_to_server_check(uint8_t status, uint8_t connected);

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
void crc8_server_side_value(uint8_t *status, char *location, uint8_t location_size, uint8_t *connected);

/**
 * @brief Checks if the device is ready to quit the connection.
 *
 * @param status Status indicator (1 for success, 0 for failure).
 * @param connected Connection status indicator (1 for connected, 0 for not connected).
 * @return QUIT if ready to quit, DONT_QUIT if not ready to quit.
 */
int ready_to_quit(uint8_t status, uint8_t connected);

#endif /*SERVER_CONNECTION_CHECK_H*/