/* SERVER
 */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>//inet_ntop()

#define MYPORT "3490" //Puerto al que se conectaran los usuarios
#define BACKLOG 10 	//número de conexiones pendientes que podra haber en cola

int main (void)
{
	struct sockaddr_storage their_addr; 	//se utiliza el struct
	//sockaddr_storage cuando se
	//desconoce si se va a utilizar ipv4
	//o ipv6, ya que en este struct
	//caben ambas structuras. Más tarde
	//se casteará al tipo conveniente
	socklen_t addr_size;
	struct addrinfo hints, *res;	//Necesarios para getaddrinfo()
	int sockfd, new_fd;

	//Primero cargamos direcciones con getaddrinfo()
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; //usa ipv4 o ipv6, cualquiera
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = AI_PASSIVE;	//fill in my ip for me. Como somos el
	//servidor, daremos nuestra ip como conexion	
	int status;
	status = getaddrinfo(NULL, MYPORT, &hints, &res);
	if(status!=0)
	{
			printf("Error en getaddrinfo(): %d\n", status);
	}
	else// FALTA PONER LAS COMPROBACIONES DE ERRORES EN LA LLAMADAS AL SISTEMA
	{
			printf("llamada a getaddrinfo() exitosa\n");

			//make a socket, bind it, and listen on it.
			//Obtenemos el filedescriptor del socket a utilizar para las conexiones
			//entrantes
			sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

			//Asignamos el puerto MYPORT al socket
			bind(sockfd, res->ai_addr, res->ai_addrlen);

			//Escuchamos conexiones entrantes
			listen(sockfd, BACKLOG);

			//Ahora podemos aceptar conexiones entrantes.
			addr_size = sizeof their_addr;

			//Este es el socket que utilizaremos paraofrecerle los servicios al
			//cliente cuya conexion hayamos aceptado.
			new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);

			//who are you?--ES PROBABLE QUE ROMAP EL PROGRAMA
			printf("Who are you?\n");
			struct sockaddr_in you;
			socklen_t you_len = sizeof you;
			status = getpeername(new_fd, (struct sockaddr*) &you, &you_len);
			
			void* dir_net = &(you.sin_addr);
			char dir_char [INET_ADDRSTRLEN];
			inet_ntop(AF_INET, dir_net, dir_char, sizeof dir_char);
			printf("dir: %s\n", dir_char);
			//-----------------

			//Todo listo para utilizar el socket new_fd para comunicarnos
			int max_len = 1024;
			char msg [max_len];
			memset(msg, 0, max_len);
			int bytes_rcv;
			do
			{
				memset(msg,0,max_len);
				bytes_rcv = recv(new_fd, msg, max_len, 0);
				printf("Mensaje recibido: %s\n%d bytes\n", msg, bytes_rcv);
			}while(bytes_rcv > 0);

			printf("Conexion cerrada por el cliente\n");

			//FINAL
			close(sockfd);
			close(new_fd);
	}

	return 0;
}
