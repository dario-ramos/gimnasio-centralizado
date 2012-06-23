#pragma once
#include "../common/Comunicacion.h"

class ComunicacionBus : public Comunicacion {

	int recibir_mensaje_gim(void *, int msg_size, long msgtype);
	/*enviar_mensaje la usamos para enviar a los socios, y recibir_mensaje
	 * para recibir mensajes de las puertas.*/

private:
	int recep_gim_qid;
	int nroBus;

	int inicializarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();
};

int ComunicacionBus::inicializarComunicacion(){
	Comunicacion::inicializarComunicacion();
	key_t clave;
	/*Creo la cola de recepcion de el gimnacio*/
	clave = ftok(DIRECTORIO, BASE_SALA_SALIDA + nroBus);//TODO cambiar inicializacion de la clase para que inicialice nroBus.  Esto no va a funcionar sin eso!
	if((recep_gim_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
	return 0;
}

key_t ComunicacionBus::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
}

key_t ComunicacionBus::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, BASE_SALA_ENTRADA + nroBus);
}

