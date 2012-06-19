#pragma once

#include "../common/Comunicacion.h"

class ComunicacionBus : public Comunicacion {
public:
//	int enviar_mensaje_bus(const void * msg, int msg_size);

private:
	int salaEntrada_qid;
	int salaSalida_qid;

	int inicializarComunicacion();
	void finalizarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();
};

int ComunicacionBus::inicializarComunicacion(){
	Comunicacion::inicializarComunicacion();
	key_t clave;
	//Obtengo la cola de recepcion (de la sala de entrada)
	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((salaEntrada_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al obtener la cola de entrada");
		return 1;
	}
	//Obtengo la cola de envio (a la sala de salida)
	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if((salaSalida_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de envio");
		return 1;
	}
	return 0;
}

void ComunicacionPuerta::finalizarComunicacion(){
	Comunicacion::finalizarComunicacion();
	//Borro la cola de recepcion
	//if(msgctl(envio_qid, IPC_RMID, (msqid_ds *) 0)){
	//	perror("Comunicacion: error al borrar la cola de entrada.");
	//}
	
}

key_t ComunicacionPuerta::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, SALA_SALIDA);
}

key_t ComunicacionPuerta::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, SALA_ENTRADA);
}
