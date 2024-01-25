/**
 * @file    db_update_thread.h
 * @author 	Vlad Kulikov
 * @date 	2024-01-05
 * @brief   Header file containing declarations for database update thread functionality.
 */
#include "db_update_thread.h"

/**
 * @brief Set the start time for database backup.
 *
 * Initializes the time when the backup of the database starts.
 *
 * @param start_database_backup Pointer to the variable that will store the start time.
 */
void set_start_time(int *start_database_backup)
{
    struct timeval time;
    /*Initializes the time the backup started*/
    gettimeofday(&time, NULL);
    *start_database_backup = time.tv_sec;
}


/**
 * @brief Check if it is time to backup the database.
 *
 * Checks if it is time to backup the database based on the predefined interval.
 *
 * @param database_backup_start_time Start time of the database backup.
 * @param back_up_flag Pointer to the flag indicating if it is time to backup.
 */
void time_to_backup(int database_backup_start_time, uint8_t *back_up_flag)
{
    struct timeval time;
    int current_time = 0;

    gettimeofday(&time, NULL);
    current_time = time.tv_sec;
    if (((current_time - database_backup_start_time) % NUMBER_OF_SECONDS_BETWEEN_BACKUPS) == 0)
    {
        *back_up_flag = TRUE;
    }
    else
    {
        *back_up_flag = FALSE;
    }
}

/**
 * @brief Update clients' data in the database.
 *
 * Updates clients' data in the database, specifically the TIME_USED value, for clients currently using the application.
 *
 * @param address_buffer_of_clients_data_structs Pointer to the buffer containing clients' data structures.
 * @param is_backup_time Flag indicating if it is time to perform the backup.
 */
void update_clients(void *address_buffer_of_clinets_data_structs, uint8_t is_backup_time)
{
    /* Checking if it is time to backup the data*/
    if (is_backup_time != TRUE)
        return;

    struct pango_data *client = (struct pango_data *)address_buffer_of_clinets_data_structs;
    struct timeval time;
    char update_clinet_data[MAX_BUFF_SIZE];
    int current_time = 0;

    for (int i = 0; i < UPDATE_THREAD_MAX_NUM_CLIENTS; ++i)
    {
        /*Cheks if a client is currently using the app*/
        if (client[i].connected == TRUE)
        {
            /* Getting the current time for updating the the app usage of all the currently using clients.  */
            gettimeofday(&time, NULL);
            current_time = time.tv_sec;
            /* Preparing the sqlite3 command to updating the TIME_USED value for all the connected clients.  */
            if (sprintf(update_clinet_data, "UPDATE your_table SET TIME_USED = %d WHERE MAC_ADR = '%s';",
                        (int)(current_time - client[i].time_start_parking), client[i].mac_address) < 0)
            {
                perror("return_database_update_thread: sprintf");
            }
            /* Executing the command.  */
            int return_value = sqlite3_exec(db_client, update_clinet_data, 0, 0, 0);
            if (return_value != SQLITE_OK)
            {
                perror("return_database_update_thread: sqlite3_exec");
            }
        }
    }
}

/**
 * @brief Perform actions before returning from the database update thread.
 *
 * This function is called before returning from the database update thread.
 * It sets flags and performs necessary actions before exiting the thread.
 *
 * @param return_flag Flag indicating if the function should perform its actions.
 * @param address_buffer_of_clients_data_structs Pointer to the buffer containing clients' data structures.
 * @param is_backup_time Pointer to the flag indicating if it is time to perform the backup.
 * @param quit_loop_flag Pointer to the flag indicating if the thread should quit the main loop.
 */
void return_database_update_thread(uint8_t return_flag, void *address_buffer_of_clinets_data_structs, uint8_t *is_backup_time, uint8_t *quit_loop_flag)
{
    if (return_flag != TRUE)
        return;
    /*Changes the value of flag. After updating all the data the thread will exit the while loop and return to the main thread.  */
    *quit_loop_flag = TRUE;
    /* In order to inable the update_clients function.  */
    *is_backup_time = TRUE;
    update_clients(address_buffer_of_clinets_data_structs, *is_backup_time);
    /* In order to disable the update_clients function that will come after return_database_update_thread at the client thread.  */
    *is_backup_time = FALSE;
}