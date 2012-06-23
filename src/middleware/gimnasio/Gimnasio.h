#pragma once
#include "../common/baseComunicacion.h"
#include "ComunicacionGimnasio.h"

using namespace std;
class Gimnasio {
public:
	Gimnasio() : id(ID_GIMNASIO){};
	~Gimnasio();
	void AtenderSocio();

	enum Posicion { PUERTA, TRANSITO, GIMNACIO };


private:
	int id;
	ComunicacionGimnasio comunicacion;
	ShmBus *shmBus[CANT_BUSES];
	int shmBusId[CANT_BUSES], semBus[CANT_BUSES], mutexShmBus[CANT_BUSES];

	bool ObtenerMemoriasCompartidas();
	bool ObtenerSemaforos();

};

void Gimnasio::AtenderSocio() {
	MsjSocio msj;
	comunicacion.recibir_mensaje(&msj, sizeof(msj), ID_GIMNASIO);
	int nroPuerta = msj.nroPuerta
	if(nroPuerta <= 0 || nroPuerta > CANT_BUSES){
		char printBuffer[200];
		UPRINTLN( "Gimansio", printBuffer, "El socio %d quiso salir por una puerta inexistente.", msj.idSocio);
		//TODO Enviar mensaje de respuesta cono notificacion de fallo
		//comunicacion.enviar_mensaje()
		return;
	}
	msj.tipo = nroPuerta + BASE_ID_BUS;//para que vaya a la sala correspondiente
	p(mutexShmBus[nroPuerta]);
	comunicacion.enviar_mensaje_sala(&msj, sizeof(msj), msj.nroPuerta);
	shmBus[nroPuerta]->salida++;
	//verifico si el bus estaba parado
	if(shmBus[nroPuerta]->salida == 1 && shmBus[nroPuerta]->entrada == 0){
		v(semBus[nroPuerta]);
	}
	v(mutexShmBus[nroPuerta]);
}

bool Gimnasio::ObtenerMemoriasCompartidas() {
	key_t clave;

	/*Creo la memoria compartida entre puertas*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		printf( "El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}
	for (int i = 0; i < CANT_BUSES; i++){
		clave = ftok(DIRECTORIO, SHM_SALAS+i);
		if((shmBusId[i] = shmget(clave, sizeof(ShmBus),  0660)) == -1){
			perror("servidor: error obteniendo la memoria compartida");
			return false;
		}
		if((shmBus[i] = (ShmBus *) shmat(shmBusId[i], 0, 0)) == (ShmBus * ) -1){
			perror("servidor: error al vincular la memoria compartida");
			return false;
		}

		/*Obtengo el semaforo para la memoria compartida entre puertas*/
		if((mutexShmBus = getsem(SEM_SHM_BUS+i)) == -1){
			perror("servidor: error al crear el semaforo");
			return false;
		}
	}
	return true;
}

bool Gimnasio::ObtenerSemaforos() {
	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	for (int i = 0; i < CANT_BUSES; i++){
		if((semBus = getsem(SEM_BUS+i)) == -1){
			perror("servidor: error al crear el semaforo");
			return false;
		}
	}
	return true;
}
