/**
 * @file 	poll_functions.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	Implementation for poll event functions.
 *
 * @copyright Copyright (c) 2024
 */

#include "poll_functions.h"

/**
 * @brief Initializes a poll event structure for a specified file descriptor.
 *
 * @param fds Pointer to a pollfd structure to be initialized.
 * @param fd File descriptor for which the poll event is initialized.
 */
void init_poll_event(struct pollfd *fds, int *fd)
{
	struct pollfd *func_fds = (struct pollfd *)(fds);
	func_fds->fd = *fd;
	func_fds->events = POLLIN;
}

/**
 * @brief Waits for a poll event on a specified file descriptor.
 *
 * This function blocks until a poll event occurs on the specified file descriptor or a timeout is reached.
 *
 * @param fds Pointer to a pollfd structure with initialized file descriptor and events.
 * @param time_out Timeout value in milliseconds.
 * @return 0 if a poll event occurs, -1 if no data is received within the timeout.
 */
int wait_poll_event(struct pollfd *fds, int time_out)
{
	struct pollfd *func_fds = (struct pollfd *)(fds);
	while (1)
	{
		/* When poll event happend during the time out.  */
		if (poll(func_fds, 1, time_out) > 0)
		{
			return 0;
		}
		else
		{
			printf("No data was received by the BBB in the time limit\n");
			return -1;
		}
	}
}