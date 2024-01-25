/**
 * @file 	uart.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Implementation of UART communication functions.
 *
 * @copyright Copyright (c) 2024
 */

#include "tcp.h"

/**
 * @brief Connects to a Pango server using TCP/IP.
 *
 * This function creates a socket, sets up the server address,
 * and connects to the Pango server.
 *
 * @param   client_socket Pointer to store the client socket file descriptor.
 * @param   server_arg Pointer to a sockaddr_in structure containing server details.
 * @return  0 on successful connection, -1 on failure.
 */
uint8_t connect_to_pango_server(int *client_socket, struct sockaddr_in *server_arg)
{
	struct sockaddr_in *server_addr = (struct sockaddr_in *)(server_arg);
	socklen_t server_addr_len = sizeof(*server_addr);

	if ((*client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/*Defining the server address*/
	memset(server_addr, 0, sizeof(*server_addr));
	server_addr->sin_family = AF_INET;
	server_addr->sin_port = htons(55152);
	server_addr->sin_addr.s_addr = inet_addr("10.0.2.15");

	/*Trying to connect to the server*/
	if (connect(*client_socket, (struct sockaddr *)server_addr, server_addr_len) == -1)
	{
		perror("connect");
		/* Wasnt able to connect to the pango server */
		return -1;
	}
	return 0;
}