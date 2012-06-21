#pragma once
#include "../common/Comunicacion.h"

class ComunicacionBus : public Comunicacion {

	int enviar_mensaje_gim(const void * msg, int msg_size);
	int recibir_mensaje_gim(void *, int msg_size, long msgtype);
	/*enviar_mensaje la usamos para enviar a las puertas, y recibir_mensaje
	 * para recibir mensajes de las puertas.*/

private:
	int envio_gim_qid;
	int recep_gim_qid;

	int inicializarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();
};

int ComunicacionBus::inicializarComunicacion(){
	Comunicacion::inicializarComunicacion();
	key_t clave;
	/*Creo la cola de envio al bus*/
	clave = ftok(DIRECTORIO, COLA_ENTRADA_GIMNACIO);
	if((envio_gim_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}

	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if((recep_gim_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
	return 0;
}

key_t ComunicacionPuerta::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_ENTRADA_PUERTAS);
}

key_t ComunicacionPuerta::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, SALA_ENTRADA);
}

