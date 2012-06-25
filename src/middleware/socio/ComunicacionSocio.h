#pragma once
#include "../common/baseComunicacion.h"

class ComunicacionSocio{
public:
	ComunicacionSocio();
	bool enviar_mensaje_puerta(const void * msg, int msg_size);
	bool recibir_mensaje_puerta(void *, int msg_size, long msgtype);
	bool recibir_mensaje_bus(void * msg, int msg_size, long msgtype);
	bool enviar_mensaje_gimnasio(const void * msg, int msg_size);
	bool recibir_mensaje_gimnasio(void *, int msg_size, long msgtype);
private:
//Estado
	int envio_sistema_qid;
	int recep_sistema_qid;
//Comportamiento
	bool inicializarComunicacion();
};

ComunicacionSocio::ComunicacionSocio() : envio_sistema_qid(-1), recep_sistema_qid(-1){
}

bool ComunicacionSocio::inicializarComunicacion(){
	char mostrar[200];
	key_t clave;
	//Valido existencia directorio ftok
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("ComunicacionSocio",mostrar,"comunicacion: El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}
	//envio_sistema
	clave = ftok(DIRECTORIO, COLA_ENTRADA_SISTEMA);
	if((envio_sistema_qid = msgget(clave, 0660)) == -1){
		perror("ComunicacionSocio::inicializarComunicacion: error al crear la cola de envio al sistema");
		return false;
	}
	//recep_sistema
	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((recep_sistema_qid = msgget(clave, 0660)) == -1){
		perror("ComunicacionSocio::inicializarComunicacion: error al crear la cola de recepcion para mensajes provinientes del sistema");
		return false;
	}
	return true;
}

//TODO Para que el broker sepa a quien redireccionar el mensaje, es necesario incluir el id de la entidad destinataria de alguna forma
// Por ahora, todas las funciones de abajo envian el mensaje tal cual. Deberian agregar el id de la entidad destinataria (o emisora).
bool ComunicacionSocio::enviar_mensaje_puerta(const void * msg, int msg_size){
	if (msgsnd(envio_sistema_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "La cola de entrada del sistema ha sido destruida.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "Hubo un error desconocido al recibir un mensaje del sistema.");
		}
		return false;
	}
	return true;
}

bool ComunicacionSocio::recibir_mensaje_puerta(void * msg, int msg_size, long msgtype){
	if (msgrcv(recep_sistema_qid, msg, msg_size - sizeof(long), msgtype, 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "La cola de entrada del sistema ha sido destruida.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "Hubo un error desconocido al recibir un mensaje del sistema.");
		}
		return false;
	}
	return true;
}

bool ComunicacionSocio::recibir_mensaje_bus(void * msg, int msg_size, long msgtype){
	if (msgrcv(recep_sistema_qid, msg, msg_size - sizeof(long), msgtype, 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "La cola de entrada del sistema ha sido destruida.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "Hubo un error desconocido al recibir un mensaje del sistema.");
		}
		return false;
	}
	return true;
}

bool ComunicacionSocio::enviar_mensaje_gimnasio(const void * msg, int msg_size){
	if (msgsnd(envio_sistema_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "La cola de entrada del sistema ha sido destruida.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "Hubo un error desconocido al recibir un mensaje del sistema.");
		}
		return false;
	}
	return true;
}

bool ComunicacionSocio::recibir_mensaje_gimnasio(void * msg, int msg_size, long msgtype){
	if (msgrcv(recep_sistema_qid, msg, msg_size - sizeof(long), msgtype, 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "La cola de entrada del sistema ha sido destruida.");
		}else{
			char printBuffer[200];
			UPRINTLN( "Socio", printBuffer, "Hubo un error desconocido al recibir un mensaje del sistema.");
		}
		return false;
	}
	return true;
}
