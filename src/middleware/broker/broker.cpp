#include "broker.h"

int main(void) {

	int entrada;
	int salida;
	int sockfd, newsockfd;
	int childpid;
	struct sockaddr_in cli_addr;
    unsigned clilen;
	static char par1[8];
	static char par2[8];
	key_t clave;

	/*Creo la cola de salida del sistema*/
	clave = ftok(DIRECTORIO, N_COLA_ENTRADA);
	if((entrada = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
			perror("broker: error al crear la cola de recepciones");
			exit(1);
	}
	/*Creo la cola de salida del sistema*/
	clave = ftok(DIRECTORIO, N_COLA_SALIDA);
	if((salida = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
			perror("broker: error al crear la cola de despacho");
			return 1;
	}
	/*abro socket para recivir entidades del sistema*/
	if ( (sockfd = tcpoppas(PUERTO_SISTEMA)) < 0){
			perror("broker: no se puede abrir el stream socket");
			exit(1);
	}

	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
			perror ("broker: error en el accept");
	}
	sprintf(par1, "%d", newsockfd);
	if((childpid = fork()) < 0){
			perror("broker: error al crear puerto de entrada para el sistema");
			exit(1);
	}else if(childpid == 0){
			execl("./puerto_entrada_sistema", "puerto_entrada_sistema", par1, (char *) 0);
			perror("broker: error al ejecutar un vendedor");
			exit(1);
	}
	if((childpid = fork()) < 0){
			perror("broker: error al crear el puerto de entrada para el sistema");
			exit(1);
	}else if(childpid == 0){
			execl("./puerto_salida_sistema", "puerto_salida_sistema", par1, (char *) 0);
			perror("broker: error al ejecutar un puerto de entrada del sistema");
			exit(1);
	}

	close(sockfd);

	/*abro socket para recivir socios*/
	if ( (sockfd = tcpoppas(PUERTO_SOCIOS)) < 0){
			perror("broker: no se puede abrir el stream socket");
			exit(1);
	}
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
				perror ("broker: error en el accept");
		}
		sprintf(par1, "%d", newsockfd);
		if((childpid = fork()) < 0){
				perror("broker: error al crear puerto de entrada para el socio");
		}else if(childpid == 0){
				execl("./puerto_entrada_socio", "puerto_entrada_socio", par1, (char *) 0);
				perror("broker: error al ejecutar un vendedor");
		}
		if((childpid = fork()) < 0){
				perror("broker: error al crear el puerto de entrada para el socio");
		}else if(childpid == 0){
				execl("./puerto_salida_socio", "puerto_salida_socio", par1, (char *) 0);
				perror("broker: error al ejecutar un puerto de entrada de un socio");
		}
	}
	close(sockfd);
	return 0;
}
