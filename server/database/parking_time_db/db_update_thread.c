#include "db_update_thread.h"

/**
 * @brief Update the database with client information.
 *
 * This function runs as a thread, periodically updating the database with client information.
 *
 * @param arg A pointer to the pango_data structure.
 * @return None.
 */
void *db_update(void *address_buffer_of_clinets_data_structs)
{
	struct pango_data *client = (struct pango_data *)address_buffer_of_clinets_data_structs;
	struct timeval time;
	uint8_t is_backup_time = 0, quit_loop_flag = 0;
	int database_backup_start_time = 0; 
	/*flag is responsibale to quit the while loop. It happens when the server is quiting*/	  
	
	set_start_time(&database_backup_start_time);

	/* Is used in case of start_db_bkup and current_db_backup having the same time value.
	It differentiate the two since current_db_backup gets it value 1 second later.  */
	sleep(1); 

	while (quit_loop_flag != TRUE)
	{
		pthread_mutex_lock(&mutex);

		/* Checks if the difference in time between start_db_bkup and current_db_backup is 5 seconds.  */
		time_to_backup(database_backup_start_time, &is_backup_time);
		/* Exits the update database thread, if there was a flag to quit ,from the main thread.  */
		return_database_update_thread(return_thread, address_buffer_of_clinets_data_structs, &is_backup_time, &quit_loop_flag);
		/* Update clients' data in the database.  */
		update_clients(address_buffer_of_clinets_data_structs, is_backup_time);

		pthread_mutex_unlock(&mutex);
	}
	printf("Out of update db thread\n");
	pthread_exit(NULL);
}