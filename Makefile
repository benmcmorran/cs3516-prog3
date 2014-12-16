HEADERS = error.h physical.h data_link.h network.h
SHARED_OBJECTS = error.o physical.o data_link.o network.o
CLIENT_OBJECTS = client.o $(SHARED_OBJECTS)
SERVER_OBJECTS = server.o $(SHARED_OBJECTS)
OBJECTS = $(CLIENT_OBJECTS) $(SERVER_OBJECTS)
CLIENT_NAME = client
SERVER_NAME = server

default: client server

%.o: %.c $(HEADERS)
	gcc -g -c $< -Wall -o $@

client: $(CLIENT_OBJECTS)
	gcc -g $(CLIENT_OBJECTS) -o $(CLIENT_NAME)

server: $(SERVER_OBJECTS)
	gcc -g $(SERVER_OBJECTS) -o $(SERVER_NAME)

clean:
	-rm -f $(OBJECTS)
