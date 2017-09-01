#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
	//Conversion de presetacion a struct

	struct sockaddr_in sa;	//ipv4

	//almaceno la direccion ip indicada en el struct
	inet_pton(AF_INET, "192.168.1.10", &sa.sin_addr); 

	char ip4[INET_ADDRSTRLEN];
	//Obtengo la dir ip del struct al string
	inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
	printf("\nLa dir IP es: %s\n",ip4);

	//Preparar los structs con getaddrinfo()
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;	// will point to the results

	memset(&hints, 0, sizeof hints); //make sure the struct is empty
	hints.ai_family = AF_UNSPEC;	 //don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
	hints.ai_flags = AI_PASSIVE;	 //fill in my IP for me

	if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) != 0) {

		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	// servinfo now points to a linked list of 1 or more struct addrinfos
	// ... do everything until you don't need servinfo anymore ....

	freeaddrinfo(servinfo); // free the linked-list

	return 0;
}
