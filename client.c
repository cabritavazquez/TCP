#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#define PUERTO 4950
#define T 256

int main (){
	struct sockaddr_in dirServer;
	int dSocket;
	char buffer[T];
	int leidos = 0, longitud, bytes;
	uint16_t longitudS;
	
	dSocket = socket(PF_INET,SOCK_STREAM ,0);
	if(dSocket < 0){
		perror("error al abrir el socket");
		exit(1);
	}
	
	memset(&dirServer,0,sizeof(dirServer));
	
	dirServer.sin_family = PF_INET;
	dirServer.sin_addr.s_addr = INADDR_ANY;
	dirServer.sin_port = htons(PUERTO);
	
	if ( (connect(dSocket,(struct sockaddr *)&dirServer,sizeof(dirServer))) < 0){
		perror("error al conectar con el servidor");
		close(dSocket);
		exit(1);
	}
	printf("cliente conectado...\n");
	/// lectura (por teclado) anticipada
	if ( (leidos = read(0,buffer,T)) < 0){
		perror("error al leer");
		close(dSocket);
		exit(1);
	}
	buffer[leidos-1] = '\0';
	longitud = leidos;
	longitudS = htons(longitud);
	/// convertimos a host
	
	while ( strcmp(buffer,"fin") != 0){
		
		/// envio de long
		/// comprobamos error 
		bytes = write(dSocket,&longitudS,sizeof(longitudS));
		if ( bytes < 0 ){
			perror("error al enviar la longitud");
			close(dSocket);
			exit(1);
		}
		else if ( bytes==0 ){
			printf("servidor desconectado 1");
			exit(1);
		}
		else if ( bytes != sizeof(longitudS) ){
			perror("error al enviar la longitud 2");
			close(dSocket);
			exit(1);
		}
		
		/// envio cadena
		/// comprobamos error 

		bytes = write(dSocket,buffer,longitud);
		if ( bytes < 0 ){
			perror("error al enviar la cadena");
			close(dSocket);
			exit(1);
		}
		else if ( bytes==0 ){
			printf("servidor desconectado 2");
			exit(1);
		}
		else if ( bytes != longitud ){
			perror("error al enviar la cadena 2");
			close(dSocket);
			exit(1);
		}	
		/// muestro lo enviado
		/// vuelvo a leer 
		/// convertimos a host
		printf ("envido %d bytes \ny la cadena: %s\n",longitud,buffer);
		
		leidos = read(0,buffer,T);
		buffer[leidos-1] = '\0';
		longitud = leidos;
		longitudS = htons(longitud);

	 
	}
	printf("final de la conexion\n");

	
	return 0;
}
