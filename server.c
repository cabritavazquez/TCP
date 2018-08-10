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
#define N 256

int main (){
	struct sockaddr_in dirServer, dirClient;
	int dSocket, ndSocket,longitud, bytes;
	socklen_t longDirClient;
	char buffer[N];
	uint16_t longitudS;
	
	// CREAMOS EL SOCKET TCP
	dSocket = socket(PF_INET, SOCK_STREAM,0);
	if (dSocket < 0){
		perror("error al crear la socket\n");
		exit(1);
	}
	
	// PONEMOS LAS DIRECCIONES A 0
	memset(&dirServer,0,sizeof(dirServer));
	
	
	dirServer.sin_family = PF_INET; 
	dirServer.sin_addr.s_addr = INADDR_ANY; 
	dirServer.sin_port = htons(PUERTO);
	
	if ( bind(dSocket,(struct sockaddr*) &dirServer, sizeof(dirServer)) < 0 ){
			perror("error al conectar con la direccion");
			exit(1);
	}

	if ( (listen(dSocket,5)) < 0 ){
		perror("error al poner en modo escucha");
		exit(1);
	}
	while(1){
			printf("Esperando conexion . . .\n");
			longDirClient = sizeof(dirClient);
			
			ndSocket = accept(dSocket,(struct sockaddr*)&dirClient, &longDirClient);
			if (ndSocket < 0){
				perror("error al aceptar la conexion");
				close (dSocket);
				exit(1);
			}
			printf("Conexion realizada con exito :) \n");

			while(1){
				/// leemos primero la long de la cadena
				bytes = read(ndSocket,&longitudS,sizeof(longitudS));
				/// convertimos a host
				longitud = ntohs(longitudS);
				if (bytes < 0){
					perror("error a leer la longitud de la cadena");
					close(ndSocket);
					close(dSocket);
					exit(1);
				}
				if (bytes == 0){
					printf("cliente desconectado\n");
					close(ndSocket);
					exit(1);
				}
				if (bytes != sizeof(longitudS) ){
					perror("error a leer la longitud de la cadena 2");
					close(ndSocket);
					close(dSocket);		
					exit(1);		
				}
				printf("recibo %d bytes \n", longitud);
				/// ahora leemos la cadena
				bytes = read(ndSocket,buffer,longitud);
				if (bytes < 0){
					perror("error al leer la cadena");
					close(ndSocket);
					close(dSocket);
					exit(1);
				}
				if (bytes == 0){
					printf("cliente desconecta\n");
					close (ndSocket);
					exit(1);
				}
				if (bytes != longitud){
					perror("error al leer la cadena");
					close (ndSocket);
					close (dSocket);
					exit(1);
				}
				printf("recibo la cadena: %s\n",buffer);
				
			}
			break;
		
		
		}
	return 0;
}
       
