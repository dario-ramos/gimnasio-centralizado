#pragma once
#include "../common/Comunicacion.h"

class ComunicacionPuerta {
public:
	ComunicacionPuerta();
	virtual ~ComunicacionPuerta();
	bool enviar_mensaje_socio(const void * msg, int msg_size);
	bool recibir_mensaje_socio(void *, int msg_size, long msgtype);
	bool enviar_mensaje_bus(const void * msg, int msg_size);

private:
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_bus_qid;

	bool inicializarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

ComunicacionPuerta::ComunicacionPuerta() {
	inicializarComunicacion();
}

bool ComunicacionPuerta::inicializarComunicacion(){
	char mostrar[200];
	key_t clave;

	/*Creo la cola de recepcion*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("Comunicacion",mostrar,"comunicacion: El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}

	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((envio_socio_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de peticiones");
		return false;
	}

	/*Creo la cola de respuesta*/
	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if((recep_socio_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return false;
	}


	/*Creo la cola de envio al bus*/
	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((envio_bus_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return false;
	}
	return true;
}

bool ComunicacionPuerta::enviar_mensaje_bus(const void * msg, int msg_size) {
	if(msgsnd(envio_bus_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "La sala se ha destruido");
		}else{
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "Hubo un error desconocido al registrar el socio en la sala.");
		}
		return false;
	}
	return true;
}

bool ComunicacionPuerta::enviar_mensaje_socio(const void * msg, int msg_size) {
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

bool ComunicacionPuerta::recibir_mensaje_socio(void *msg, int msg_size, long msgtyp){
	if (msgrcv(recep_socio_qid, msg, msg_size - sizeof(long), msgtyp, 0)== -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "La cola de entrada del sistema se ha destruido.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "Hubo un error desconocido al recibir mensaje del socio");
		}
		return false;
	}
	return true;
}
