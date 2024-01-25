/**
 * @file    client_thread_func.c
 * @author  Vlad Kulikov
 * @brief   Implementation of functions related to client-thread functionality.
 * @date    2024-01-06
 */
#include "client_thread.h"

/**
 * @brief Wait for data from the client and handle disconnection.
 *
 * This function waits for data from the client specified by the provided
 * structure pointer. It also handles the case where the client disconnects
 * unexpectedly.
 *
 * @param client_arg Pointer to the structure containing client data.
 * @param status Pointer to the status variable to be updated based on connection status.
 * @param client_buff Pointer to the buffer to store received client data.
 * @param client_buff_size Size of the client data buffer.
 * @return CONNECTION_LOST if the client disconnects unexpectedly, 0 otherwise.
 */
uint8_t wait_for_data_from_client(void *client_arg, uint8_t *status, uint8_t *client_buff, uint8_t client_buff_size)
{
    struct pango_data *client = (struct pango_data *)client_arg;
    /* Waiting for the client data from the BBB.  */
    if (recv(client->client_fd, client_buff, client_buff_size, 0) == 0)
    {
        /* The thread enteres if the client suddenly disscinnected.  */
        *status = CONNECTION_LOST;
        printf("The client disconnected suddenly\n");

        /* The thread changes the running status,
           so the updating data base thread wont change the time value,
           while the client is disconnected.  */
        pthread_mutex_lock(&mutex);

        client->connected = FALSE;

        pthread_mutex_unlock(&mutex);

        return CONNECTION_LOST;
    }
    return 0;
}

/**
 * @brief Calculate CRC-8 checksum for the given data and used in the CRC_8_check function.
 *
 * This function calculates the CRC-8 checksum for the given data using a predefined polynomial.
 *
 * @param data Pointer to the data array.
 * @param length Length of the data array.
 * @return CRC-8 checksum.
 */
uint8_t calculateCRC8(uint8_t* data, uint32_t length) {
    uint8_t crc = 0xFF;

    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i];

        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc;
}

/**
 * @brief Check CRC-8 validity for a data buffer.
 *
 * This function checks the CRC-8 validity of the received data buffer.
 * If the CRC check fails, an error status is set.
 *
 * @param buff Data buffer to check.
 * @param buff_size Size of the data buffer.
 * @param status Pointer to the status variable. It is updated with CRC8_TEST_FAILED if the CRC check fails.
 *
 * @return TRUE if the CRC-8 check is successful, FALSE otherwise.
 */
uint8_t CRC_8_check(uint8_t buff[10], uint8_t buff_size, uint8_t *status )
{
	if(buff[9] == calculateCRC8(buff, buff_size))
    {
        printf("The CRC-8 check is successful\n");
        return TRUE;
	}
	else
    {
		printf("The CRC-8 check has faild.\nThere is data coraption in the recevied data.\n");
		*status = CRC8_TEST_FAILED; //Error value 
        return FALSE;
	}
}

/**
 * @brief Store client data in the struct: client_data_struct.
 *
 * This function extracts relevant information from the client data buffer and
 * stores it in the struct: client_data_struct.
 *
 * @param status Pointer to the status variable indicating data integrity.
 * @param client_buff Pointer to the buffer containing client data.
 * @param client_buff_size Size of the client data buffer.
 * @param client_arg Pointer to the structure for storing client data.
 */
void store_client_data_in_struct(uint8_t *status, uint8_t *client_buff,
                               uint8_t client_buff_size, void *client_arg)
{
    struct pango_data *client = (struct pango_data *)client_arg;
    if (*status != CRC8_TEST_FAILED)
    {
        int offset = 0;
        for (size_t i = 0; i < 8; ++i)
        {
            if (i == 0)
            {
                *status = client_buff[0];
            }
            if (i > 0 && i < 7)
            {
                if (i == 6)
                {
                    offset += sprintf(client->mac_address + offset, "%x", client_buff[i]);
                }
                else
                {
                    offset += sprintf(client->mac_address + offset, "%x%c", client_buff[i], ':');
                }
            }
            else
            {
                client->x_axis = client_buff[i];
                client->y_axis = client_buff[i + 1];
            }
        }
    }
}

/**
 * @brief Check the existence of a client in the database based on MAC address.
 *
 * This function prepares an SQL statement to check if a client with the specified
 * MAC address exists in the database.
 *
 * @param mac_address_buff Pointer to the buffer containing the MAC address.
 * @param mac_addres_size Size of the MAC address buffer.
 * @param stmt_arg Pointer to the SQLite3 statement object (output parameter).
 */
void clinet_exist_in_database_check_preparation(uint8_t *mac_address_buff,
                                                uint8_t mac_addres_size, 
                                                sqlite3_stmt **stmt_arg)
{
    uint8_t mac_address[mac_addres_size];
    char check_if_open[150];

    /* Copy the MAC address from the buffer to a local variable.  */
    memcpy(mac_address, mac_address_buff, mac_addres_size);

    /* Creating the sqlite3 command.  */
    if (sprintf(check_if_open, "SELECT * FROM your_table WHERE MAC_ADR = '%s';",
                mac_address) < 0)
    {
        puts("sprintf err");
    }

    /* Preparing the sql command.
       Which searches in the Data Base of clients,
       checking if the mac address appears there.  */
    if (sqlite3_prepare_v2(db_client, check_if_open, -1, stmt_arg, 0) != SQLITE_OK)
    {
        perror("sqlite3_prepare_v2:check_if_open");
    }
}

/**
 * @brief Check the result of a client existence check in the database.
 *
 * This function checks the result of a SQLite statement execution to determine
 * if a client exists in the database.
 *
 * @param stmt_arg Pointer to the SQLite3 statement object.
 * @param allrdy_chckd Pointer to a flag indicating if the client has already been checked.
 * @return TRUE if the client exists and has not been checked already, FALSE otherwise.
 */
uint8_t clinet_exist_in_database_check(uint8_t *mac_address_buff,
                                       uint8_t mac_addres_size,
                                       uint8_t *allrdy_chckd,
                                       sqlite3_stmt **stmt_arg)
{
    clinet_exist_in_database_check_preparation(mac_address_buff, mac_addres_size, stmt_arg);

    /* Checking if the client exists.  */
    if ((sqlite3_step(*stmt_arg) == SQLITE_ROW) && (*allrdy_chckd == 0))
    {
        /* Releasing the stmt.  */
        sqlite3_finalize(*stmt_arg);
        return TRUE;
    }
    /* Releasing the stmt.  */
    sqlite3_finalize(*stmt_arg);
    return FALSE;
}

/**
 * @brief Check if a new client based on the checked database value.
 *
 * This function determines whether a new client is present based on the checked database value.
 * If the checked database value is FALSE, it returns TRUE, indicating a new client.
 * Otherwise, it returns FALSE, indicating an existing client.
 *
 * @param checked_database_value The value indicating whether the client exists in the database (TRUE or FALSE).
 *
 * @return TRUE if the client is new, FALSE if the client already exists in the database.
 */
uint8_t new_client(uint8_t checked_database_value)
{
    if(checked_database_value == FALSE) return TRUE; 
    else return FALSE;
}

/**
 * @brief Update client data in the database based on the parking duration.
 *
 * This function calculates the parking duration, updates the database with the calculated
 * time used, and sets the provided 'end' parameter with the current time.
 *
 * @param end Pointer to the variable that will be updated with the current time.
 * @param client_struct Pointer to the client data structure.
 */
uint8_t update_client_data(uint32_t *end, void *client_data_struct)
{
    struct pango_data *client = (struct pango_data *)(client_data_struct);
    struct timeval time;
    char update_clinet_data[100];
    uint8_t return_value = 0;

    /*Getting the final time value for the amount the client has to pay*/
    gettimeofday(&time, NULL);
    *end = time.tv_sec;

    if (sprintf(update_clinet_data, "UPDATE your_table SET TIME_USED = %d WHERE MAC_ADR = '%s';",
                (int)(*end - client->time_start_parking), client->mac_address) < 0)
    {
        perror("update_client_data: sprintf");
        return CLOSE_APP_ERROR;
    }

    return_value = sqlite3_exec(db_client, update_clinet_data, 0, 0, 0);
    if (return_value != SQLITE_OK)
    {
        perror("update_client_data: sqlite3_exec");
        return ERROR;
    }

    return 0;
}

/**
 * @brief Calculate and send payment data to the client.
 *
 * This function calculates the parking duration and payment amount based on the start and end times
 * and sends the payment data to the client.
 *
 * @param start_time Start time of parking.
 * @param end_time End time of parking.
 * @param parking_price_per_second Price per second of parking.
 * @param client_fd File descriptor of the client.
 */
void calculate_and_send_payment_data(int start_time, int end_time,double parking_price_per_second, uint8_t client_fd)
{
    int elapsed_time_seconds = end_time - start_time;
    double pay[2];

    /* Inserting the data of price and time in to a buff.  */
    pay[0] = parking_price_per_second * elapsed_time_seconds;
    pay[1] = elapsed_time_seconds;
    /*Sending the data to the client*/
    sleep(1);
    if (send(client_fd, pay, sizeof(pay), 0) == -1)
    { // Sending thw payment data
        perror("Error send func,in pay");
    }
}

/**
 * @brief Remove client data from the database based on MAC address.
 *
 * This function deletes all data related to a client from the database using its MAC address.
 *
 * @param mac_address_arg Pointer to the MAC address.
 * @param mac_address_size Size of the MAC address.
 */
void remove_client_data(uint8_t *mac_address_arg, uint8_t mac_address_size)
{
    int val = 0;
    char delete_query[150];
    char mac_address[mac_address_size];
    memcpy(mac_address, mac_address_arg, mac_address_size);

    /*Removing all the clinets data that's stored in the data base*/
    if (sprintf(delete_query,
                "DELETE FROM your_table WHERE MAC_ADR = '%s';",
                mac_address) < 0)
    {
        perror("remove_client_data: sprintf");
    }

    val = sqlite3_exec(db_client, delete_query, 0, 0, 0);
    if (val != SQLITE_OK)
    {
        perror("remove_client_data: sqlite3_exec in status");
    }
}