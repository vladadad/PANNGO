/**
 * @file    client_thread.c
 * @author  Vlad Kulikov
 * @date    2024-01-06
 */
#include "client_thread.h"

/**
 * @brief Pango client thread function.
 *
 * This function handles communication with the Pango client, processes received data,
 * updates the database, and performs various actions based on the status value.
 *
 * @param arg A pointer to the pango_data structure.
 */
void *pango_client(void *client_data_struct)
{
	struct pango_data *client = (struct pango_data *)client_data_struct;
	struct timeval time;
	sqlite3_stmt *stmt;

	/* Buffer that will store clients data in to clients_data_struct,
	for an easier access of the data.  */
	uint8_t client_data_buff[CLIENT_DATA_BUFFER_SIZE] = {0};

	/* Value that indicates if a loop function should:
	   keep running (STAY) or QUIT.  */
	uint8_t return_value = STAY;

	/* Represents the clients application status.  */
	uint8_t status = STATUS_INITIAL_VALUE;

	/*  Indicates whether the client's data has been checked in the database.  */
	uint8_t checked_database = FALSE;

	/* Representation of the time vlaue at the end of client_thread.  */
	uint32_t end_time = 0;

	/* A message, that indicates a failure,
	which will be sent to the client.  */
	const char err_msg[ERROR_MESSAGE_SIZE] = "ERROR";

	printf("Client %d connected\n\n", client->client_fd);

	while (return_value != QUIT)
	{
		if (wait_for_data_from_client(client_data_struct, &status, client_data_buff, sizeof(client_data_buff)) == CONNECTION_LOST)
		{
			/* Apon sudden disconnection sets the staus value to CONNECTION_LOST.  */
			return_value = QUIT;
			break;
		}

		/* Checking the received data for 'data corruption'.  */
		switch (CRC_8_check(client_data_buff, PANGO_DATA_SIZE, &status))
		{
		/* Copying the client data from a buffer to a struct.  */
		case TRUE:
			store_client_data_in_struct(&status, client_data_buff, sizeof(client_data_buff), client_data_struct);
			break;
		/* CRC_8_check assigns the variable status the value CRC8_TEST_FAILED .  */
		case FALSE:
		default:
			return_value = QUIT;
			break;
		}

		switch (status)
		{
		/* If the status vlaue, received by the client, says that the client wants to start using the app.  */
		case START_APP:
			/* If the MAC address already appears in the database of clients, the data extracted and updated.  */
			if (clinet_exist_in_database_check(client->mac_address, sizeof(client->mac_address), &checked_database, &stmt) == TRUE)
			{
				return_value = retriev_client_data(client_data_struct, &stmt, &status);
				if (return_value != QUIT)
				{
					/* in the next while loop iteration, after the client is already connected,
					it won't check if the client already exists.  */
					checked_database = TRUE;
				}
				else
					break;
			}

			/* When the mac address doesn't appear in the client data base.  */
			if (new_client(checked_database) == TRUE)
			{
				/* Intitalizing and storing the clent data in the database.  */
				return_value = process_client_data(client_data_struct, &stmt, &status);
				if (return_value != QUIT)
				{
					/* Setting the clients running value of the thread with ON.
					which flags the data base to update the clients data.  */
					client->connected = TRUE;
				}
			}
			break;
		/* If the status vlaue, received by the client, says that the client wants to close the app.  */
		case CLOSE_APP:
			pthread_mutex_lock(&mutex);

			if (update_client_data(&end_time, client_data_struct) == ERROR)
			{
				status = CLOSE_APP_ERROR;
			}

			/* Setting off the running value of the thread.
			which flags the data base to stop updating the clients data.  */
			client->connected = FALSE;

			pthread_mutex_unlock(&mutex);
			return_value = QUIT;
			break;
		default:
			break;
		}
	}

	/* Handling the exit of the client, depanding on the status value */
	switch (status)
	{
	/* Calculating and sanding the amount to pay, to the client.
	And removing the clients data from the database.  */
	case CLOSE_APP:
		calculate_and_send_payment_data(client->time_start_parking, end_time, client->price, client->client_fd);
		pthread_mutex_lock(&mutex);
		remove_client_data(client->mac_address, sizeof(client->mac_address));
		pthread_mutex_unlock(&mutex);
		break;
	/* Updeating the clients data in to the client data base.  */
	case CONNECTION_LOST:
		pthread_mutex_lock(&mutex);
		update_client_data(&end_time, client_data_struct);
		pthread_mutex_unlock(&mutex);
		break;
	case CRC8_TEST_FAILED:
		puts("The CRC-8 value, of the received data, is different compared to the one the client sent");
		if (send(client->client_fd, err_msg, sizeof(err_msg), 0) == -1)
		{
			perror("CRC8_TEST_FAILED: send");
		}
		break;
	/* An error occurred in on of the inner functions of clinet_exist_in_database_check.  */
	case ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS:
		puts("Error in one of clinet_exist_in_database_check inner functions");
		if (send(client->client_fd, err_msg, sizeof(err_msg), 0) == -1)
		{
			perror("ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS: send");
		}
		break;
	case ERROR_STATUS_IN_NEW_CLIENT_SUBFUNCTIONS:
		puts("Error in one of new_client inner functions");
		if (send(client->client_fd, err_msg, sizeof(err_msg), 0) == -1)
		{
			perror("ERROR_STATUS_IN_NEW_CLIENT_SUBFUNCTIONS: send");
		}
		break;
	default:
		break;
	}

	/*closing resources*/
	close(client->client_fd);
	printf("status = %d\n", status);
	printf("Client disconnected.\n");
	pthread_exit(NULL);
}