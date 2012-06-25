#include "common/baseComunicacion.h"
#include <sys/stat.h>
#include "../common/Uprintf.h"
#include "common/semaforo.h"

int main(void){
	ShmCantidadSocios *shm_puertas;
	int shm_puertas_id;
	ShmBus *shm_bus;
	int sem_shm_bus_id;
	int shm_bus_id;
	int sem_bus_id;
	int mutex_puertas_id;
	int id_salida_sys;
	int id_entrada_sys;
	int id_sala_salida;
	int id_sala_entrada;
	char mostrar[200];
	key_t clave;

	/*Verifico que exista el directorio para los IPCs*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("inicializar",mostrar,"inicializar: El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}

	/*Creo la memoria compartida de puertas y su semaforo*/
    clave = ftok(DIRECTORIO, SHM_PUERTAS);
    if((shm_puertas_id = shmget(clave, sizeof(ShmCantidadSocios), IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error obteniendo la memoria compartida");
		exit(1);
    }
    if((shm_puertas = (ShmCantidadSocios *) shmat(shm_puertas_id, 0, 0)) == (ShmCantidadSocios * ) -1){
		perror("inicializar: error al vincular la memoria compartida");
		exit(1);
    }
    shm_puertas->cantidad =0;

    if((mutex_puertas_id = creasem(SEM_SHM_PUERTAS)) == -1){
		perror("inicializar: error al crear el semaforo");
		exit(1);
    }
    if(inisem(mutex_puertas_id, 1)){
		perror("inicializar: error al inicializar el semaforo");
		exit(1);
    }

	/*Creo la memoria compartida de salas y su semaforo*/
    clave = ftok(DIRECTORIO, SHM_BUS);
    if((shm_bus_id = shmget(clave, sizeof(ShmBus), IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error obteniendo la memoria compartida");
		exit(1);
    }
    if((shm_bus = (ShmBus *) shmat(shm_puertas_id, 0, 0)) == (ShmBus * ) -1){
		perror("inicializar: error al vincular la memoria compartida");
		exit(1);
    }
    shm_bus->entrada = 0;
    shm_bus->salida = 0;

    if((sem_shm_bus_id = creasem(SEM_SHM_BUS)) == -1){
		perror("inicializar: error al crear el semaforo de la memoria compartida del bus");
		exit(1);
    }
    if(inisem(sem_shm_bus_id, 1)){
		perror("inicializar: error al inicializar el semaforo de la memoria compartida del bus");
		exit(1);
    }

    /*Creo el semaforo del bus*/
    if((sem_bus_id = creasem(SEM_BUS)) == -1){
		perror("inicializar: error al crear el semaforo de la memoria compartida del bus");
		exit(1);
	}
	if(inisem(sem_bus_id, 0)){//el bus se crea en estado de espera
		perror("inicializar: error al inicializar el semaforo del bus");
		exit(1);
	}



    /*Creo las colas*/
	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((id_salida_sys = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error al crear la cola de salida del sistema");
		return false;
	}

	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if((id_entrada_sys = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error al crear la cola de entrada del sistema");
		return false;
	}

	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if((id_sala_salida = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error al crear la sala de salida");
		return false;
	}

	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((id_sala_entrada = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializar: error al crear la sala de entrada");
		return false;
	}
}
