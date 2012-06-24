#pragma once
#include "../common/Comunicacion.h"

class ComunicacionGimnasio {
public:


	bool recibir_mensaje_socio(void * msg, unsigned int msg_size, long int id);
	bool enviar_mensaje_socio(const void * msg, unsigned int msg_size);
	bool enviar_mensaje_sala(const void * msg, unsigned int msg_size);

private:
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_sala_qid;

	bool inicializarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

bool ComunicacionGimnasio::inicializarComunicacion(){
	key_t clave;
	char mostrar[200];
	/*Creo la cola de envio al bus*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("ComunicacionGimnasio",mostrar," El directorio del ftok, %s, no existe\n");
		return false;
	}

	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((envio_socio_qid = msgget(clave, 0660)) == -1){
		UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de envio a socios\n");
		return false;
	}

	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if ((recep_socio_qid = msgget(clave, 0660)) == -1) {
		UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de recepcion de socios\n");
		return false;
	}
	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if ((envio_sala_qid = msgget(clave, 0660)) == -1) {
		UPRINTLN("ComunicacionGimnasio",mostrar,"Error al crear la cola de envio a la sala.\n");
		return false;
	}
	return true;
}

key_t ComunicacionGimnasio::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
}

key_t ComunicacionGimnasio::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
}

bool ComunicacionGimnasio::recibir_mensaje_socio(void * msg, unsigned int msg_size, long int tipo) {
	if (msgrcv(recep_socio_qid, msg, msg_size - sizeof(long), tipo, 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "La sala se ha destruido.");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "Hubo un error desconocido al enviar un socio a la sala.");
		}
		return false;
	}
	return true;
}

bool ComunicacionGimnasio::enviar_mensaje_sala(const void * msg, unsigned int msg_size) {
	if (msgsnd(envio_sala_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "La sala se ha destruido.");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "Hubo un error desconocido al enviar un socio a la sala.");
		}
		return false;
	}
	return true;
}

bool ComunicacionGimnasio::enviar_mensaje_socio(const void * msg, unsigned int msg_size) {
	if (msgsnd(envio_socio_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "La cola de salida del sistema se ha destruido.");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionGimnasio", printBuffer, "Hubo un error desconocido al recibir un mensaje del socio.");
		}
		return false;
	}
	return true;
}
