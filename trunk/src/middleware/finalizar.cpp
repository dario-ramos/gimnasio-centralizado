#include "common/baseComunicacion.h"
#include <sys/stat.h>
#include "../common/Uprintf.h"
#include "common/semaforo.h"

int main(void){
	int shm_puertas_id;
	int mutex_puertas_id;
	int sem_shm_bus_id;
	int shm_bus_id;
	int sem_bus_id;
	int id_salida_sys;
	int id_entrada_sys;
	int id_sala_salida;
	int id_sala_entrada;
	char mostrar[200];
	key_t clave;

	/*Verifico que exista el directorio para los IPCs*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("finalizador",mostrar,"finalizador: El directorio del ftok, %s, no existe\n", DIRECTORIO );
	}

	/*Borro la memoria compartida de puertas y su semaforo*/
    clave = ftok(DIRECTORIO, SHM_PUERTAS);
    if((shm_puertas_id = shmget(clave, sizeof(ShmCantidadSocios), 0660)) == -1){
		perror("finalizador: error obteniendo la memoria compartida");
    } else if(shmctl(shm_puertas_id, IPC_RMID, (shmid_ds *) 0)){
        perror("finalizador: error al borrar la memoria compartida");
    }
    if((mutex_puertas_id = getsem(SEM_SHM_PUERTAS)) == -1){
		perror("finalizador: error al obtener el semaforo");
	} else if(elisem(mutex_puertas_id) == -1){
		perror("finalizador: error al borrar el semaforo");
    }

	/*Borro la memoria compartida de salas y su semaforo*/
    for ( int i = 1; i <= CANT_BUSES; i++) {
		clave = ftok(DIRECTORIO, SHM_BUS + i);
		if((shm_bus_id = shmget(clave, sizeof(ShmBus), 0660)) == -1){
			perror("finalizador: error obteniendo la memoria compartida del bus");
		} else if(shmctl(shm_bus_id, IPC_RMID, (shmid_ds *) 0)){
			perror("finalizador: error al borrar la cola de compras");
		}

		if((sem_shm_bus_id = getsem(SEM_SHM_BUS + i)) == -1){
			perror("finalizador: error al obtener el semaforo");
		} else if(elisem(sem_shm_bus_id) == -1){
			perror("finalizador: error al borrar el semaforo de la memoria compartida del bus");
		}

		/*Borro el semaforo del bus*/
		if((sem_bus_id = getsem(SEM_BUS + i)) == -1){
			perror("finalizador: error al obtener el semaforo");
		} else if((elisem(sem_bus_id)) == -1){
			perror("finalizador: error al borrar el semaforo del bus");
		}
    }


    /*Borro las colas*/
	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((id_salida_sys = msgget(clave, 0660)) == -1) {
		perror("finalizador: error al borrar la cola de salida del sistema");
	} else if(msgctl(id_salida_sys, IPC_RMID, (msqid_ds *) 0)) {
        perror("finalizador: error al borrar la cola de salida del sistema");
	}

	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if((id_entrada_sys = msgget(clave, 0660)) == -1) {
		perror("finalizador: error al borrar la cola de entrada del sistema");
	} else if(msgctl(id_entrada_sys, IPC_RMID, (msqid_ds *) 0)) {
        perror("finalizador: error al borrar la cola de entrada del sistema");
	}

	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if((id_sala_salida = msgget(clave, 0660)) == -1) {
		perror("finalizador: error al borrar la sala de salida");
	} else if(msgctl(id_sala_salida, IPC_RMID, (msqid_ds *) 0)) {
        perror("finalizador: error al borrar la sala de salida");
	}

	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((id_sala_entrada = msgget(clave, 0660)) == -1) {
		perror("finalizador: error al borrar la sala de entrada");
	} else if(msgctl(id_sala_entrada, IPC_RMID, (msqid_ds *) 0)) {
        perror("finalizador: error al borrar la sala de entrada");
	}

	return 0;
}
