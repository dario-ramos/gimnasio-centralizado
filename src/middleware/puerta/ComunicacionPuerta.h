#pragma once
#include "../common/Comunicacion.h"

class ComunicacionPuerta : public Comunicacion {
public:

	int enviar_mensaje_bus(const void * msg, int msg_size);

private:
	int envio_Bus_qid;
	int recep_Bus_qid;

	int inicializarComunicacion();
	void finalizarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

int ComunicacionPuerta::inicializarComunicacion(){
	Comunicacion::inicializarComunicacion();
	key_t clave;
	/*Creo la cola de envio al bus*/
	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((recep_Bus_qid = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
}

key_t ComunicacionPuerta::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_ENTRADA_SOCIOS);
}

key_t ComunicacionPuerta::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_ENTRADA_PUERTAS);
}

int ComunicacionPuerta::enviar_mensaje_bus(const void * msg, int msg_size) {
	return msgsnd(envio_Bus_qid, msg, msg_size - sizeof(long), 0);
}
