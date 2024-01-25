#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <signal.h>
#include "main_server.h"

pthread_mutex_t mutex; 			
int server_sockfd;	
/* D.B where all clients data is stored.  */ 			
sqlite3 *db_client;	
/* D.B where the prices per city are stored.  */	
sqlite3 *db_prices;	
/* A flag that when turnd on calls the 'update database thread' to return to the main thread.  */				
volatile uint8_t return_thread;	

int main(void){	
	/*Initalizing data for the TCP server*/
	int client_sockfd;	
	struct sockaddr_in server_addr, client_addr;	
	/*Intializing the data reserved for each client*/
	struct pango_data client[SERVER_MAX_NUM_CLIENTS];
	memset(client, 0, sizeof(client));		
	/*The thread reserved for the clients*/
	pthread_t thread[SERVER_MAX_NUM_CLIENTS], db_upd_thr;
	socklen_t 	server_addr_len = sizeof(server_addr),	
				client_addr_len = sizeof(client_addr);
	uint8_t return_value = 0;

	
	return_value = sqlite3_open("pango_client_database.db", &db_client);
	if (return_value != SQLITE_OK) {
		perror("main_server:main:sqlite3_open:pango_client_database.db");
		exit(EXIT_FAILURE);
	}

	const char *create_table_query = "CREATE TABLE IF NOT EXISTS your_table (MAC_ADR TEXT, TIME_USED INT, LOCATION TEXT);";
	return_value = sqlite3_exec(db_client, create_table_query, 0, 0, 0);
	if(return_value != SQLITE_OK){
		perror("main_server:main:sqlite3_exec:create_table_query");
		exit(EXIT_FAILURE);
	}

	return_value = sqlite3_open("parking_prices_per_city.db", &db_prices);
	if (return_value != SQLITE_OK) {
		perror("main_server:main:sqlite3_open:parking_prices_per_city.db");
		exit(EXIT_FAILURE);
	}
	
	puts("SERVER: Starting");
	
	/* Creating the server socket */
	if((server_sockfd = socket( AF_INET, SOCK_STREAM, 0)) == -1){ 
		perror("socket");
		exit(EXIT_FAILURE);
	}

	/* Seting the servers atributes like: address, port number and who he will ne listening to. */
	memset(&server_addr, 0, sizeof(server_addr));
	/*Using IPv4*/
	server_addr.sin_family = AF_INET; 			
	/*Port number 55152*/
	server_addr.sin_port =  htons(55152);
	/*Listens to conect to any available network*/		
	server_addr.sin_addr.s_addr = INADDR_ANY; 	
	
	/* Binding the socket of the server and its network values. */
	if(bind(server_sockfd, (struct sockaddr*) &server_addr, server_addr_len) == -1){
		perror("bind");
		close(server_sockfd);
		exit(EXIT_FAILURE);
	}

	/* Listening for a data from a client.  */
	if(listen(server_sockfd, 5) == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	/* Creatig a thread that updates the database.  */
	if(pthread_create(&db_upd_thr, NULL, db_update,(void *)client) == -1){
		perror("pthread_create db_thread");
	}
	
	/*The mutex is created here so if there is an error in the code above I wouldnt need to close it every time*/
	pthread_mutex_init(&mutex, NULL);

	for(int i = 0; i <= SERVER_MAX_NUM_CLIENTS; ++i){

		/*Waiting for a request from a client to connect to the server*/
		if((client[i].client_fd = accept(server_sockfd, (struct sockaddr*)&client_addr, &client_addr_len)) == -1){
			perror("accept");
		}

		/*Allocating a thread to every new client*/
		if(pthread_create(&thread[i], NULL, pango_client, (void *)&client[i]) == -1){
			perror("pthread_create");
			close(client_sockfd);
		}
	}
	
	//////////////////// Releasing the resources before the end of the program ///////////////////////

	if(pthread_join(thread[1], NULL) == -1){
		perror("pthread_join:");
		printf("thread[%d] is the problem\n", 3);
	}
	
	/*Changing the value so the db_upd_thr thread updates and stores the clients data in the Data Base and exits it thread*/
	return_thread = RETURN_THE_DATABASE_UPDATE_THREAD;

	if(pthread_join(db_upd_thr, NULL) == -1){
		perror("pthread_join:");
	}

	if(close(server_sockfd) == -1){ 
		perror("close server_sockfd");
	}

	return_value = sqlite3_close(db_prices);
	if (return_value != SQLITE_OK) {
        perror("main_server:main:sqlite3_close(db_prices)");
    }

	return_value = sqlite3_close(db_client);
	if (return_value != SQLITE_OK) {
        perror("main_server:main:sqlite3_close(db_client)");
    }

	pthread_mutex_destroy(&mutex);
	puts("Server quits");

	return 0;
}
