/* CLIENT
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

		//Who am i?
		printf("Who am I?\n");
		int ilen = 64;
		char I [ilen];
		gethostname(I,ilen);
		printf("dir: %s\n", I);
		//----
		

		//mensaje
		//char *msg = "Mensaje del cliente";
		int len, bytes_sent;

		//enviar mensaje
		int max_len = 1024;
		char mensaje [max_len];

		do
		{
				printf("Escribe mensaje: ");
				//scanf("%s", mensaje);
				//Utilizamos fgets() para leer solo una linea y poder mandar
				//mensajes vacios si pulsamos unicamente enter para el input.
				fgets(mensaje, max_len, stdin);

				//al utilizar fgets(), siempre se almacenara '\n' al final del
				//mensaje, pues pulsaremos enter para indicar el final del
				//input. Para eliminar este '\n', utilizaremos la funcion
				//strcspn(char *s, char *reject), que nos indica cuantos bytes
				//de s hay hasta que encontramos uno de los componentes de
				//reject.
				//Sustituimos final de linea por final de string.
				mensaje[strcspn(mensaje, "\n")] = 0;

				//len = strlen(msg);
				//bytes_sent = send(sockfd, msg, len, 0);
				//printf("Mensaje enviado: %s\nbytes: %d\n",msg,bytes_sent);
				len = strlen(mensaje);
				bytes_sent = send(sockfd, mensaje, len, 0);
				printf("Mensaje enviado: %s\nbytes: %d\n",mensaje,bytes_sent);
		}while(len > 0);

		//recibir mensaje
		//TODO


		//FIN
		close(sockfd);

		return 0;
}
