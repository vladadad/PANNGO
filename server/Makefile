CC = gcc
CSERVER_FLAGS = -lsqlite3 -pthread -I./database -I./client
CSQL_FLAGS = -lsqlite3  -I./database/price_db 

SERVER_TARGET = srvr
SQL_TARGET =  sql_price_db_create

SRC_MAIN = main_server.c
SRC_CLIENT = ./client/client_thread.c
SRC_CLIENT_FUNC = ./client/client_thread_func.c
SRC_DB_UPDATE = ./database/parking_time_db/db_update_thread.c
SRC_DB_UPDATE_FUNC = ./database/parking_time_db/db_update_thread_func.c
SRC_CREATE_DB = ./database/price_db/sql_price_db_create.c
SRC_NEW_CLIENT = ./client/new_client/new_client.c
SRC_EXISTING_CLINET = ./client/existing_client/existing_client.c

HEAD_DB_UPDATE = ./database/parking_time_db/db_update_thread.h
HEAD_SERVER = main_server.h
HEAD_CLIENT = ./client/client_thread.h
HEAD_NEW_CLIENT = ./client/new_client/new_client.h
HEAD_EXISTING_CLINET = ./client/existing_client/existing_client.h

server : $(SERVER_TARGET) $(SQL_TARGET) 
	./$(SQL_TARGET) 
 
$(SERVER_TARGET) 	: 	$(SRC_MAIN) $(SRC_CLIENT) $(SRC_DB_UPDATE) $(SRC_DB_UPDATE_FUNC) $(SRC_CLIENT_FUNC) \
						$(SRC_NEW_CLIENT) $(SRC_EXISTING_CLINET) \
						$(HEAD_SERVER) $(HEAD_CLIENT) $(HEAD_NEW_CLIENT) $(HEAD_EXISTING_CLINET) $(HEAD_DB_UPDATE)
	$(CC) $^ $(CSERVER_FLAGS)  -o $(SERVER_TARGET) 

$(SQL_TARGET) 	: 	$(SRC_CREATE_DB)
	$(CC) $^ $(CSQL_FLAGS) -o $(SQL_TARGET)

clean:
	rm $(SERVER_TARGET) $(SQL_TARGET)

# Declare the targets as phony targets
.PHONY:clean 