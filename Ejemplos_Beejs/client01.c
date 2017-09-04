/* SERVER
*/
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>

#define MYPORT "3490" //Puerto al que se conectaran los usuarios
#define SERVER_ADDR "127.0.0.1"//deberia cambierse para que sea aportado con args

int main (void)
{
	//Preparar conexiones
	struct addrinfo hints, *res;
	int sockfd;

	//getaddrinfo()
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;	//IPV4 o IPV6
	hints.ai_socktype = SOCK_STREAM;//TCP
	
	int status;
	status = getaddrinfo(SERVER_ADDR, MYPORT, &hints, &res);
	printf("getaddrinfo() -> %d\n",status);
	

	//make a socket
	sockfd = socket(res->ai_family ,res->ai_socktype, res->ai_protocol);

	//Nos saltamos bind(), pues realmente nos es indiferente que socket utilice el
	//clinte. Dejamos que el SO escoja uno disponible.

	//connect

	status = connect(sockfd, res->ai_addr, res->ai_addrlen);
	printf("connect() -> %d\n",status);
	
	//mensaje
	char *msg = "Mensaje del cliente";
	int len, bytes_sent;
	
	//enviar mensaje
	len = strlen(msg);
	bytes_sent = send(sockfd, msg, len, 0);
	printf("Mensaje enviado: %s\nbytes: %d\n",msg,bytes_sent);
	//recibir mensaje

	//FIN
	close(sockfd);

	return 0;
}
