#pragma once
#include "../common/Comunicacion.h"

class ComunicacionPuerta : public Comunicacion {

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
	clave = ftok(DIRECTORIO, SALA_PUERTA_SALIDA);
	if((recep_Bus_qid = msgget(clave, IPC_CREAT | IPC_EXCL | 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
}

key_t ComunicacionPuerta::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_SALIDA_PUERTAS);
}

key_t ComunicacionPuerta::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_ENTRADA_PUERTAS);
}
