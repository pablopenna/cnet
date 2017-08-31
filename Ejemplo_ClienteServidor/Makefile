all: socket_server socket_client
	echo 'all done'
socket_server.o: socket_server.c
	gcc -Wall -c socket_server.c
socket_client.o: socket_client.c
	gcc -Wall -c socket_client.c
socket_server: socket_server.o
	gcc socket_server.o -o socket_server
socket_client: socket_client.o
	gcc socket_client.o -o socket_client
clean:
	rm *.o
	rm socket_server
	rm socket_client
