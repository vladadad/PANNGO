/**
 * @file    new_client.c
 * @author  Vlad Kulikov
 * @brief   Implementation of functions related to processing new clients.
 * @date    2024-01-06
 */
#include "new_client.h"


/**
 * @brief Process client data including initialization, database operations, and location sending.
 *
 * This function performs the following steps:
 * - Initializes and retrieves the start time for the client.
 * - Acquires a mutex to protect shared resources.
 * - Executes database operations, including retrieving parking prices and inserting client data.
 * - Sends the client's location.
 *
 * If any critical error occurs during database operations, the function sets the status
 * to an error code and returns, otherwise, it continues with sending the location.
 *
 * @param client_data_struct A pointer to the structure holding client data.
 * @param stmt A pointer to the MySQL statement for database operations.
 * @param status Pointer to the status variable to be updated based on the success of the operation.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t process_client_data(void *client_data_struct, sqlite3_stmt **stmt, uint8_t *status)
{
    puts("New client");
    initialize_and_get_start_time(client_data_struct);

    pthread_mutex_lock(&mutex);
    if
    (
        retrieve_parking_price_per_city_from_database(client_data_struct, *stmt) == QUIT ||
        insert_client_data_into_database(client_data_struct) == QUIT
    )
    {
        *status = ERROR_STATUS_IN_NEW_CLIENT_SUBFUNCTIONS;
        printf("error in process_client_data\n");
        pthread_mutex_unlock(&mutex);
        return QUIT;
    }
    else
    {
        pthread_mutex_unlock(&mutex);
        /* Sending the city name of the client to the client.  */
        send_client_location(client_data_struct);
        return STAY;
    }
}

/**
 * @brief Initialize and get the start time for the client.
 *
 * This function initializes the start time for the client using the current system time
 * and retrieves the client's location using the location_func.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 */
void initialize_and_get_start_time(void *client_data_struct)
{
    struct pango_data *client = (struct pango_data *)(client_data_struct);
    struct timeval time;

    /*Initializing and geting the value of the time the client started using the app */
    gettimeofday(&time, NULL);
    client->time_start_parking = time.tv_sec;

    /*Returns the location*/
    location_func(client->x_axis, client->y_axis, client->location);
}

/**
 * @brief Retrieve the price from the database based on the client's location.
 *
 * This function constructs and executes an SQL statement to retrieve the price
 * associated with the specified city from the 'city_parking' table in the database.
 * The retrieved price is then stored in the client structure.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @param stmt_arg Pointer to the SQLite statement structure.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t retrieve_parking_price_per_city_from_database(void *client_data_struct, sqlite3_stmt *stmt_arg)
{
    struct pango_data *client = (struct pango_data *)(client_data_struct);
    sqlite3_stmt *stmt = (sqlite3_stmt *)(stmt_arg);
   
    char price_data[150];
    int return_value = 0;
    if (sprintf(price_data, "SELECT * FROM city_parking WHERE CITY = '%s';",
                client->location) < 0)
    {
        perror("retrieve_price_from_database: sprintf");
       return QUIT;
    }
    return_value = sqlite3_prepare_v2(db_prices, price_data, -1, &stmt, 0);
    if (return_value != SQLITE_OK)
    {
        perror("retrieve_price_from_database: sqlite3_prepare_v2");
        return QUIT;
    }
    else
    {
        return_value = sqlite3_step(stmt);
        if (return_value == SQLITE_ROW)
        {
            client->price = sqlite3_column_double(stmt, 1);
            printf("Retrieved value: %.3f\n", client->price);
        }
    }
    sqlite3_finalize(stmt);
    return STAY;
}

/**
 * @brief Insert client data into the database.
 *
 * This function inserts received client data into the 'your_table' table in the client database.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @return QUIT if there is an error, STAY otherwise.
 */
uint8_t insert_client_data_into_database(void *client_data_struct)
{
    struct pango_data *client = (struct pango_data *)(client_data_struct);
    char insert_query[150];
    int val = 0;

    /*Inserting the received data from the client in to the client data base*/
    if (sprintf(insert_query, "INSERT INTO your_table (MAC_ADR , TIME_USED , LOCATION ) VALUES ('%s', %d,'%s');",
                client->mac_address, 0, client->location) < 0)
    {
        perror("insert_client_data_into_database: sprintf");
        return QUIT;
    }
    val = sqlite3_exec(db_client, insert_query, 0, 0, 0);
    if (val != SQLITE_OK)
    {
        perror("insert_client_data_into_database: sqlite3_exec");
        return QUIT;
    }
    return STAY;
}

/**
 * @brief Send the client's current location to the client.
 *
 * This function sends the name of the city the client is currently at to the client using the send function.
 *
 * @param client_data_struct Pointer to the structure containing client data.
 * @return QUIT if there is an error during the send operation, otherwise, returns void.
 */
uint8_t send_client_location(void *client_data_struct)
{
    struct pango_data *client = (struct pango_data *)(client_data_struct);
   
    /* Send the name of the city the client curently at.  */
    if (send(client->client_fd, client->location, sizeof(client->location), 0) == -1)
    {
        perror("Error send_client_location: send");
        return QUIT;
    }
}

/**
 * @brief Determine location based on coordinates.
 *
 * This function determines the location based on the given x and y coordinates.
 * If the coordinates are out of range, an error message is stored in the provided buffer.
 *
 * @param x X-coordinate (0-127).
 * @param y Y-coordinate (0-127).
 * @param buff Buffer to store the location or error message.
 */
void location_func(uint8_t x, uint8_t y, char *buff)
{
    if (x < 0 || x > 127 || y < 0 || y > 127)
    {
        sprintf(buff, "%s", "ERROR"); // If a vlue is an error
    }
    else if (x <= 70 && y <= 70)
    {
        sprintf(buff, "%s", "Ashkelon");
    }
    else if (x <= 70 && y > 70)
    {
        sprintf(buff, "%s", "Jerusalem");
    }
    else if (x > 70 && y <= 70)
    {
        sprintf(buff, "%s", "Petah-Tikva");
    }
    else if (x > 70 && y > 70)
    {
        sprintf(buff, "%s", "Herzliya");
    }
}