/**
 * @file 	poll_functions.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Header file for poll event functions.
 *
 * @copyright Copyright (c) 2024
 */
#ifndef POLL_FUNCTIONS_H
#define POLL_FUNCTIONS_H

#include <poll.h>
#include <stdio.h>
#include "./../../png_enums.h"

enum delay
{
	MAX_DELAY = -1,
	SMALL_DELAY = 1000
};

/**
 * @brief Initializes a poll event structure for a specified file descriptor.
 *
 * @param fds Pointer to a pollfd structure to be initialized.
 * @param fd File descriptor for which the poll event is initialized.
 */
void init_poll_event(struct pollfd *fds, int *fd);

/**
 * @brief Waits for a poll event on a specified file descriptor.
 *
 * This function blocks until a poll event occurs on the specified file descriptor or a timeout is reached.
 *
 * @param fds Pointer to a pollfd structure with initialized file descriptor and events.
 * @param time_out Timeout value in milliseconds.
 * @return 0 if a poll event occurs, -1 if no data is received within the timeout.
 */
int wait_poll_event(struct pollfd *fds, int time_out);

#endif /*POLL_FUNCTIONS_H*/