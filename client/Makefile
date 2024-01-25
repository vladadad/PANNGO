ARMCC = arm-linux-gnueabihf-gcc
ARMCFLAGS = -pthread -I./bbb/poll_event/ -I./stm/uart -I./bbb/server/tcp \
			-I./bbb/server/connection_check -I./stm/connection_check/ 

TARGET = bbb_pango_client

SRC_MAIN = 	main_client.c
SRC_FUNC = 	client_func.c
SRC_POLL = 	./bbb/poll_event/poll_functions.c 
SRC_UART = 	./stm/uart/uart.c  
SRC_TCP  = 	./bbb/server/tcp/tcp.c
SRC_SERVER_CHECK_CONNECTION = ./bbb/server/connection_check/server_connection_check_functions.c

HEAD_CLIENT = client.h
HEAD_POLL   = ./bbb/poll_event/poll_functions.h  
HEAD_UART   = ./stm/uart/uart.h 
HEAD_TCP    = ./bbb/server/tcp/tcp.h
HEAD_SERVER_CHECK_CONNECTION  =  ./bbb/server/connection_check/server_connection_check_functions.h
HEAD_ENUMS   = ./png_enums.h

all: $(TARGET)

$(TARGET):  $(SRC_MAIN) $(SRC_FUNC) $(SRC_POLL) $(SRC_UART) $(SRC_TCP) $(SRC_SERVER_CHECK_CONNECTION) \
			$(HEAD_CLIENT) $(HEAD_POLL) $(HEAD_UART) $(HEAD_TCP) $(HEAD_SERVER_CHECK_CONNECTION) $(HEAD_ENUMS)
	$(ARMCC) $^ $(ARMCFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

# Declare the targets as phony targets
.PHONY: clean