#pragma once
#include "../common/Comunicacion.h"

class ComunicacionGimnasio {
public:


	bool recibir_mensaje_socio(void * msg, unsigned int msg_size, long int id);
	bool enviar_mensaje_socio(void * msg, unsigned int msg_size);
	bool enviar_mensaje_sala(const void * msg, unsigned int msg_size);

private:
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_sala_qid;

	int inicializarComunicacion();
	void finalizarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

int ComunicacionGimnasio::inicializarComunicacion(){
	key_t clave;
	char mostrar[200];
	/*Creo la cola de envio al bus*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("ComunicacionGimnasio",mostrar," El directorio del ftok, %s, no existe\n");
		return 1;
	}

	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((envio_socio_qid = msgget(clave, 0660)) == -1){
		UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de envio a socios\n");
		return 1;
	}

	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if ((recep_socio_qid = msgget(clave, 0660)) == -1) {
		UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de recepcion de socios\n");
		return 1;
	}
	for (int i = 0; i < CANT_BUSES; i++) {
		clave = ftok(DIRECTORIO, BASE_SALA_SALIDA+i);
		if ((envio_sala_qid[i] = msgget(clave, 0660)) == -1) {
			UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de envio a la sala.\n");
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

bool ComunicacionGimnasio::recibir_mensaje_socio(void * msg, unsigned int msg_size, long int tipo) {
	return msgrcv(recep_socio_qid, msg, msg_size - sizeof(long), tipo, 0);
}

bool ComunicacionGimnasio::enviar_mensaje_sala(const void * msg, unsigned int msg_size, int nroSala) {
	if(nroSala>CANT_BUSES || nroSala <= 0) {
		char printBuffer[200];
		UPRINTLN( "Gimansio", printBuffer, "Se desea enviar un socio a una sala inexistente.");
	}
	return msgsnd(envio_sala_qid[nroSala -1], msg, msg_size - sizeof(long), 0);
}

bool ComunicacionGimnasio::enviar_mensaje_socio(const void * msg, int msg_size) {
	if (msgsnd(envio_socio_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "La cola de salida del sistema se ha destruido.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "Hubo un error desconocido al recibir un mensaje del socio.");
		}
		return false;
	}
	return true;
}
