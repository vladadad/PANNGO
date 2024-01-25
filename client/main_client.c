/**
 * @file 	main_client.c
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief 	This file contains the main program for the BBB application.
 *
 * @copyright Copyright (c) 2024
 ******************************************************************************
 * The application communicates with an STM controller over UART1 and UART4.
 * It establishes a connection with a server over TCP protocol.
 * It sends and receives data, and manages parking information.
 ******************************************************************************
 * Beagle Bone Black pins in use:
 *	 _________ _________________
 *	|		  |___TX___|___RX___|
 *	|  UART1  | P9_24  | P9_26  |
 *	|_________|________|________|
 *	|		  |___TX___|___RX___|
 *	|  UART4  | P9_13  | P9_11  |
 *	|_________|________|________|
 */

#include "client.h"

/* Flags the thread responsable for
   the data transmited when the button pressed.  */
volatile int exit_app = 0;

int main(int argc, char *argv[])
{
	uint8_t data_buff[DATA_BUFF_SIZE], status = ON, stm_data_receive_error = 0, connected = 0, loop = TRUE;
	int uart1_fd, uart4_fd, client_socket = 0, 
	/* A status that is sent to the STM if there was a problem with the data transfer.  */
	restart = RESTART;
	/* A buffer where the finle values, amount to pay and time parked, are stored.  */
	double payment[PAYMENT_SIZE];
	/* A buff that holds the clients location name*/
	char location[LOCATION_NAME_MAX_LEN];

	/* Creating a struct for the server information regarding network.  */
	struct sockaddr_in server_addr;

	/* Used for setting the uart options.  */
	struct termios options, options2;

	/* Initializing two uart with polling later.  */
	struct pollfd fds[NUM_OF_UARTS_USED];

	/* The thread that is responsable for the button pressing uart transmition.  */
	pthread_t sig_thread;

	/* Initializing and setting up the UART1 and UART4 pins.  */
	init_uart1(&uart1_fd, &options);
	init_uart4(&uart4_fd, &options2);

	/* Assigning the polling event trigger to each file.  */
	init_poll_event(&fds[UART1], &uart1_fd);
	init_poll_event(&fds[UART4], &uart4_fd);

	/* Creating the thread from which pressing the phisical button will start/end the conncetion to the server.  */
	pthread_create(&sig_thread, NULL, &start_end_func, &uart4_fd);

	/* The main program.  */
	while (loop != QUIT)
	{
		/* Flushing the buff of fd2 which is connected to the poll function bellow.  */
		tcflush(uart4_fd, TCIOFLUSH);

		/* Waiting for the button pressing event.  */
		wait_poll_event(&fds[UART4], MAX_DELAY);

		/* Flushing agian in case.  */
		tcflush(uart4_fd, TCIOFLUSH);

		/* sending the status to the STM controller, start/finish.  */
		write(uart1_fd, &status, sizeof(status));

		/* Flushing the buff of fd so the data read is not garbage.  */
		tcflush(uart1_fd, TCIOFLUSH);

		/* Cheking if the STM cintroller sent data back,
		   within 1 second from the first sent status vlue.  */
		if (wait_poll_event(&fds[UART1], SMALL_DELAY) == ERROR)
		{
			/* Enters the statment if the BBB tryied
			   to get data more than 3 times.  */
			++stm_data_receive_error;
			if (stm_data_receive_error == RESTART)
			{
				write(uart1_fd, &restart, sizeof(restart));
				printf("restarting STM controller\n");
				loop = QUIT;
				break;
			}
			continue;
		}
		if (received_data_from_stm(status) == TRUE)
		{
			/* Reading the data from the STM to the buff.  */
			read(uart1_fd, data_buff, sizeof(data_buff));

			/* Gets the value of status that is stored.  */
			get_status(data_buff, sizeof(data_buff), &status);

			/* The value of status determins if
			   the data passed the CRC-8 check or not.  */
			CRC_8_check(data_buff, PANGO_DATA_SIZE, &status);

			/* When the client is not connected to the server yet.  */
			if (already_connected_to_server_check(status, connected) == NOT_CONNECTED)
			{
				uint8_t return_value = 0;

				/* Creating a TCP connection as a client
				   and trying to connect to the server.  */
				if (connect_to_pango_server(&client_socket, &server_addr) == ERROR)
				{
					/* The clinet cant connect to the server, then closing
					   resources and closing the button thread.  */
					exit_app = TRUE;
					pthread_join(sig_thread, NULL);
					loop = QUIT;
					break;
				}
				return_value = send_data_and_receive_location(&client_socket, data_buff, sizeof(data_buff),
															  location, sizeof(location));
				if (return_value == ERROR)
				{
					loop = QUIT;
					break;
				}

				/* Checking if the servers crc8 value is identical to
				   the BBB crc8 value.  */
				crc8_server_side_value(&status, location, sizeof(location), &connected);
				sleep(1);
			}
			else if ((status != OFF) && (status != ON))
			{
				/* Avoiding creating another connection on the next button press,
				   since pressing a button is approximately less than 1 second.  */
				puts("Here");
				sleep(1);
			}

			/* When the cliennt wants to close the app.  */
			if (ready_to_quit(status, connected) == QUIT)
			{
				/* Sending the server a quiting request.  */
				if (send(client_socket, data_buff, sizeof(data_buff), 0) == ERROR)
				{
					loop = QUIT;
					break;
				}

				/* Recives the time used data and the amount of payment. */
				if (recv(client_socket, payment, sizeof(payment), 0) == ERROR)
				{
					loop = QUIT;
					break;
				}

				/*Prints the time parked and the price to pay.  */
				final_price_for_costumer(payment, sizeof(payment));

				close(client_socket);

				/*Restarting the value in case the client wants to connect again*/
				connected = NOT_CONNECTED;
			}
		}
		/*Updating the value of the clients status*/
		updating_status_value(&status);
	}
	/*Closing resources*/
	write(uart1_fd, &restart, sizeof(restart));
	printf("The end of the program\n");
	close(client_socket);
	close(uart1_fd);
	close(uart4_fd);
	return 0;
}
