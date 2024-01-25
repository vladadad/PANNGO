/**
 * @file 	uart.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Header file for TCP/IP communication functions.
 *
 * @copyright Copyright (c) 2024
 */
#ifndef TCP_PNG_H
#define TCP_PNG_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "./../../../png_enums.h"

/* Creating a struct for the server information regarding network.  */
extern struct sockaddr_in server_addr;

/**
 * @brief Connects to a Pango server using TCP/IP.
 *
 * This function creates a socket, sets up the server address,
 * and connects to the Pango server.
 *
 * @param   client_socket Pointer to store the client socket file descriptor.
 * @param   server_arg Pointer to a sockaddr_in structure containing server details.
 *
 * @return  0 on successful connection, -1 on failure.
 */
uint8_t connect_to_pango_server(int *client_socket, struct sockaddr_in *server_arg);

#endif /*TCP_PNG_H*/