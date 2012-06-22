#pragma once
#include "../common/Comunicacion.h"

class ComunicacionPuerta : public Comunicacion {
public:

	int enviar_mensaje_bus(const void * msg, int msg_size);

private:
	int envio_bus_qid;

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
	if((envio_bus_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
	return 0;
}

void ComunicacionPuerta::finalizarComunicacion(){
	Comunicacion::finalizarComunicacion();
	/*Creo la cola de envio al bus*/
	if(msgctl(envio_qid, IPC_RMID, (msqid_ds *) 0)){
		perror("Comunicacion: error al borrar la cola de envio.");
	}
}

key_t ComunicacionPuerta::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_ENTRADA_SOCIOS);
}

key_t ComunicacionPuerta::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_ENTRADA_PUERTAS);
}

int ComunicacionPuerta::enviar_mensaje_bus(const void * msg, int msg_size) {
	return msgsnd(envio_bus_qid, msg, msg_size - sizeof(long), 0);
}
