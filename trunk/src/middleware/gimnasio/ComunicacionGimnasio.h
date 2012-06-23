#pragma once
#include "../common/Comunicacion.h"

class ComunicacionGimnasio : public Comunicacion {
public:

	int recibir_mensaje_socio(void * msg, unsigned int msg_size, long int id);
	int enviar_mensaje_sala(const void * msg, unsigned int msg_size, int nroSala);

private:
	int recep_socio_qid;
	int envio_sala_qid[CANT_BUSES];

	int inicializarComunicacion();
	void finalizarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

int ComunicacionGimnasio::inicializarComunicacion(){
	Comunicacion::inicializarComunicacion();
	key_t clave;
	/*Creo la cola de envio al bus*/
	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if ((recep_socio_qid = msgget(clave, 0660)) == -1) {
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return 1;
	}
	for (int i = 0; i < CANT_BUSES; i++) {
		clave = ftok(DIRECTORIO, BASE_SALA_SALIDA+i);
		if ((envio_sala_qid[i] = msgget(clave, 0660)) == -1) {
			perror("inicializarComunicacionGimnasio: error al crear la cola de respuesta.");
			return 1;
		}
	}
	return 0;
}

key_t ComunicacionGimnasio::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
}

key_t ComunicacionGimnasio::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
}

int ComunicacionGimnasio::recibir_mensaje_socio(void * msg, unsigned int msg_size, long int tipo) {
	return msgrcv(recep_socio_qid, msg, msg_size - sizeof(long), tipo, 0);
}

int ComunicacionGimnasio::enviar_mensaje_sala(const void * msg, unsigned int msg_size, int nroSala) {
	if(nroSala>CANT_BUSES || nroSala <= 0){
		char printBuffer[200];
		UPRINTLN( "Gimansio", printBuffer, "Se desea enviar un socio a una sala inexistente.");
	}
	return msgsnd(envio_sala_qid[nroSala -1], msg, msg_size - sizeof(long), 0);
}
