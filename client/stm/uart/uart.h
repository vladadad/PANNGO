/**
 * @file 	uart.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Header file for UART communication functions.
 *
 * @copyright Copyright (c) 2024
 */
#ifndef UART_PNG_H
#define UART_PNG_H

#include <termios.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "./../../png_enums.h"

#define NUM_OF_UARTS_USED   2
#define UART1               0
#define UART4               1

extern struct termios options;
extern struct termios options2;

/**
 * @brief Initializes UART communication on ttyO1.
 *
 * This function opens the specified UART device file (/dev/ttyO1) and configures the communication settings.
 *
 * @param fd Pointer to store the file descriptor for the opened UART device.
 * @param opt Pointer to a termios structure for configuring UART communication settings.
 */
void init_uart1(int *fd, void *opt);

/**
 * @brief Initializes UART communication on ttyO4.
 *
 * This function opens the specified UART device file (/dev/ttyO4) and configures the communication settings.
 *
 * @param fd Pointer to store the file descriptor for the opened UART device.
 * @param opt Pointer to a termios structure for configuring UART communication settings.
 */
void init_uart4(int *fd, void *opt);

#endif /*UART_PNG_H*/