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

		//Todo listo para utilizar el socket new_fd para comunicarnos
		
		//FINAL
		close(sockfd);
		close(new_fd);
	}

	return 0;
}
