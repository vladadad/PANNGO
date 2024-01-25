/**
 * @file    existing_client.c
 * @author  Vladislav Kulikov
 * @date    2024-01-14
 * @brief   Implementation for handling existing clients in the system.
 */

#include "existing_client.h"

/**
 * @brief Retrieve client data from the database.
 *
 * This function is called when the client already exists in the database. It continues counting
 * the parking time from the last value stored in the database and updates relevant information.
 * If any critical error occurs during database operations, the function sets the status
 * to an error code and returns QUIT; otherwise, it sends the client's location and returns STAY.
 *
 * @param client_data_struct Pointer to the client data structure.
 * @param stmt Pointer to the SQLite3 statement for database operations.
 * @param status Pointer to the status variable. It is updated with ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS if an error occurs.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t retriev_client_data(void *client_data_struct, sqlite3_stmt **stmt, uint8_t *status)
{
    puts("Client already exsists in the database");
					
    pthread_mutex_lock(&mutex);
    /* Continue counting the parking time,from the last value stored in the database.  */
    if 
    (
        retrieve_time_start_parking_value_from_database(client_data_struct, *stmt) == QUIT ||
        update_client_and_continue_time(client_data_struct, *stmt) == QUIT ||
        retrieve_parking_price_per_city_from_database(client_data_struct, *stmt) == QUIT
    )
    {
        *status = ERROR_STATUS_IN_CLIENT_EXIST_SUBFUNCTIONS;
        pthread_mutex_unlock(&mutex);
        return QUIT;
    }
    else
    {
        send_client_location(client_data_struct);
        pthread_mutex_unlock(&mutex);
        return STAY;
    }
}

/**
 * @brief Retrieve the time_start_parking value from the database.
 *
 * This function extracts the time_start_parking value from the 'your_table' table in the database.
 *
 * @param client_struct Pointer to the client data structure.
 * @param stmt_arg Pointer to the SQLite3 statement structure.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t retrieve_time_start_parking_value_from_database(void *client_struct, sqlite3_stmt *stmt_arg)
{ 
    struct pango_data *client = (struct pango_data *)(client_struct);
    sqlite3_stmt *stmt = (sqlite3_stmt *)(stmt_arg);

    /* A buffer that stores the sqlite3 command.  */
    char time_used_data[MAX_BUFF_SIZE] = {0};
    uint8_t return_value = 0;

    /* Extracting the vlaue of time_start_parking from the data base.
    Which indicates the time value at the beggining of the app.  */
    if (sprintf(time_used_data, "SELECT TIME_USED FROM your_table WHERE MAC_ADR = '%s';",
                client->mac_address) < 0)
    {
        perror("sqlite3_prepare_v2: sprintf");
        sqlite3_finalize(stmt);
        return QUIT;
    }

    return_value = sqlite3_prepare_v2(db_client, time_used_data, -1, &stmt, 0);
    if (return_value != SQLITE_OK)
    {
        perror("sqlite3_prepare_v2: sqlite3_prepare_v2");
        sqlite3_finalize(stmt);
        return QUIT;
    }
    else
    {
        return_value = sqlite3_step(stmt);
        if (return_value == SQLITE_ROW)
        {
            client->time_start_parking = sqlite3_column_int(stmt, 0);
            printf("client->time_start_parking = %d\n", client->time_start_parking);
        }
    }
    sqlite3_finalize(stmt);
    return STAY;
}

/**
 * @brief Update client information and continue counting time.
 *
 * This function updates the time, running status, and location for a client.
 * It retrieves the client's location from the 'your_table' table in the client database
 * and updates the client structure accordingly.
 *
 * @param client_struct Pointer to the structure containing client data.
 * @param stmt_arg Pointer to the SQLite statement structure.
 *
 * @return QUIT if there is an error during database operations, STAY otherwise.
 */
uint8_t update_client_and_continue_time(void *client_struct, sqlite3_stmt *stmt_arg)
{
    struct pango_data *client = (struct pango_data *)(client_struct);
    sqlite3_stmt *stmt = (sqlite3_stmt *)(stmt_arg);
    struct timeval time;
    
    char location_data[150];
    uint8_t return_value = 0;

    /* Continue counting the time from the last time value stored in the data base.  */
    gettimeofday(&time, NULL);
    client->time_start_parking = time.tv_sec - client->time_start_parking;

    /* Updating the thread that the clinet resumes the app usage.  */
    client->connected = TRUE;

    /* Retrieving the location stored in the client database,
       so it can extract the price by the location name.  */
    if (sprintf(location_data, "SELECT LOCATION FROM your_table WHERE MAC_ADR = '%s';",
                client->mac_address) < 0)
    {
        perror("update_client_and_continue_time: sprintf");
        return QUIT;
    }

    return_value = sqlite3_prepare_v2(db_client, location_data, -1, &stmt, 0);
    if (return_value != SQLITE_OK)
    {
        perror("update_client_and_continue_time: sqlite3_prepare_v2");
        return QUIT;
    }
    else
    {
        return_value = sqlite3_step(stmt);
        if (return_value == SQLITE_ROW)
        {
            const unsigned char *location = sqlite3_column_text(stmt, 0);
            strcpy(client->location, (const char *)location);
            printf("Retrieved location: %s\n", client->location);
        }
    }

    sqlite3_finalize(stmt);
    return STAY;
}