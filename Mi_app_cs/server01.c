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
#include <pthread.h>
#include <stdlib.h>//exit()

#define MYPORT "3490" //Puerto al que se conectaran los usuarios
#define BACKLOG 2 	//número de conexiones pendientes que podra haber en cola

//Variables globales, principalmente para los hilos
//Como mucho habrá tantos hilos como
//conexiones permitidas
pthread_t lista_hilos[BACKLOG];

//Semaforo. Creacion
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

//Con contador sabremos cuantos hilos hay activos en un momento dado.
//Para modificar esta variable tenemos que utilizar el samforo
int contador=0;

//Dado el fd de un socket, devuelve la dir ip del cliente conectado al mismo
//en la variable dir_char, que debe tener de longitud INET_ADDRSTRLEN
//char dir_char [INET_ADDRSTRLEN];
void whoareyou(int sockfd, char * dir_char, int charsize)
{
		//who are you?--ES PROBABLE QUE ROMAP EL PROGRAMA
		//printf("Who are you?\n");
		struct sockaddr_in you;
		socklen_t you_len = sizeof you;
		int status = getpeername(sockfd, (struct sockaddr*) &you, &you_len);
		char aux [INET_ADDRSTRLEN];

		void* dir_net = &(you.sin_addr);
		if(status == -1)
		{
			printf("ERROR whoareyou(): No se pudo obtener la dir ip del cliente.\n");
			dir_char[0]='-';
			dir_char[1]='1';
			dir_char[2]= 0 ;
		}
		else
		{
			inet_ntop(AF_INET, dir_net, dir_char, charsize);
			inet_ntop(AF_INET, dir_net, aux, sizeof aux);
			printf("Antes de salir:\n dir_net: %s ! - size: %lu\naux: %s ! - size: %lu\n", dir_char, sizeof	dir_char, aux, sizeof aux );
		}

}

//Funcion utilizada por el hilo que recibira los mensajes del cliente
//Recibira como argumento el fd del socket que se utiliza para recibir nuevas
//conexiones
void * servirCliente(void * sockfdvoid)
{
		pthread_mutex_lock(&mutex);
		//id del hilo.
		int id = contador;
		contador++;
		pthread_mutex_unlock(&mutex);

		//casteo el argumento recibido.
		//CUIDADO!
		int sockfd = *((int*) sockfdvoid);

		//Lo hago local ya que solo se utiliza aqui
		struct sockaddr_storage their_addr; 	//se utiliza el struct
		//sockaddr_storage cuando se
		//desconoce si se va a utilizar ipv4
		//o ipv6, ya que en este struct
		//caben ambas structuras. Más tarde
		//se casteará al tipo conveniente

		socklen_t addr_size = sizeof their_addr;

		//Este es el socket que utilizaremos paraofrecerle los servicios al
		//cliente cuya conexion hayamos aceptado.
		int new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
		if(new_fd<0)
		{
				printf("Error en accept(): %d\nSALIENDO...\n", new_fd);
				exit(1);
		}
		printf("llamada a accept() exitosa\n");

		//who are you?--ES PROBABLE QUE ROMAP EL PROGRAMA
		printf("Who are you?\n");
		//struct sockaddr_in you;
		//socklen_t you_len = sizeof you;
		//status = getpeername(new_fd, (struct sockaddr*) &you, &you_len);

		//void* dir_net = &(you.sin_addr);
		//char dir_char [INET_ADDRSTRLEN];
		//inet_ntop(AF_INET, dir_net, dir_char, sizeof dir_char);
		//printf("dir: %s\n", dir_char);
		char dir_char [INET_ADDRSTRLEN];
		//memset(dir_char, 0, sizeof dir_char);
		whoareyou(new_fd, dir_char, sizeof dir_char);
		printf("id[%d] - dir: %s\n", id, dir_char);
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
				printf("id:[%d] - Mensaje recibido: %s\n%d bytes\n",id, msg, bytes_rcv);
		}while(bytes_rcv > 0);

		printf("id[%d] - Conexion cerrada por el cliente\n", id);
		
		//cierro socket
		close(new_fd);
		//termino ejecucion del hilo
		pthread_exit((void *)0);

}


int main (void)
{
		//Iniciar semaforo
		pthread_mutex_init(&mutex,NULL);

		//Lo hago local del hilo ya que solo se utiliza alli
		//struct sockaddr_storage their_addr; 	//se utiliza el struct
		//sockaddr_storage cuando se
		//desconoce si se va a utilizar ipv4
		//o ipv6, ya que en este struct
		//caben ambas structuras. Más tarde
		//se casteará al tipo conveniente

		//Lo hago tambien local del hilo
		//socklen_t addr_size;

		struct addrinfo hints, *res;	//Necesarios para getaddrinfo()
		int sockfd;

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
				printf("Error en getaddrinfo(): %d\nSALIENDO...\n", status);
				exit(1);
		}
		// FALTA PONER LAS COMPROBACIONES DE ERRORES EN LA LLAMADAS AL SISTEMA

		printf("llamada a getaddrinfo() exitosa\n");

		//make a socket, bind it, and listen on it.
		//Obtenemos el filedescriptor del socket a utilizar para las conexiones
		//entrantes
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if(sockfd==-1)
		{
				printf("Error en socket(): %d\nSALIENDO...\n", sockfd);
				exit(1);
		}
		printf("llamada a socket() exitosa\n");

		//Asignamos el puerto MYPORT al socket
		status = bind(sockfd, res->ai_addr, res->ai_addrlen);
		if(status==-1)
		{
				printf("Error en bind(): %d\nSALIENDO...\n", status);
				exit(1);
		}
		printf("llamada a bind() exitosa\n");

		//Escuchamos conexiones entrantes
		status = listen(sockfd, BACKLOG);
		if(status==-1)
		{
				printf("Error en listen(): %d\nSALIENDO...\n", status);
				exit(1);
		}
		printf("llamada a listen() exitosa\n");
		

		//A PARTIR DE AQUI -- HILOS!
		
		//NUMERO ESTATICO
		int j;
		for(j=0;j<BACKLOG;j++)
		{
			pthread_create(&lista_hilos[j], NULL, servirCliente,
							(void *) &sockfd);
		}
		
		printf("CREACION HILOS COMPLETADA!\n");
		//Si se ha llegado ha este punto,
		//Ahora podemos aceptar conexiones entrantes.
		/*
		addr_size = sizeof their_addr;

		//Este es el socket que utilizaremos paraofrecerle los servicios al
		//cliente cuya conexion hayamos aceptado.
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &addr_size);
		if(new_fd<0)
		{
				printf("Error en accept(): %d\nSALIENDO...\n", new_fd);
				exit(1);
		}
		printf("llamada a accept() exitosa\n");

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
		*/

		//FINAL
		for(j=0;j<BACKLOG;j++)
		{
			pthread_join(lista_hilos[j],NULL);
		}

		close(sockfd);
		//close(new_fd);


		return 0;
}
