#include "ComunicacionBus.h"
#include "../common/baseComunicacion.h"
#include "../../common/Uprintf.h"
#include <sys/types.h>
#include <sys/stat.h>

ComunicacionBus::ComunicacionBus() : envio_socio_qid(-1), recep_gim_qid(-1), recep_sala_entrada_qid(-1){
	inicializarComunicacion();
}

bool ComunicacionBus::inicializarComunicacion(){
	char mostrar[200];
	key_t clave;

	//Valido existencia de directorio para ftok
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		UPRINTLN("inicializarComunicacion",mostrar,"comunicacion: El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}

	//envio_socio
	clave = ftok(DIRECTORIO, COLA_SALIDA_SISTEMA);
	if((envio_socio_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de salida del sistema");
		return false;
	}

	//recepc_gim
	clave = ftok(DIRECTORIO, SALA_SALIDA);
	if((recep_gim_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de recepcion del gimnasio");
		return false;
	}

	//sala_entrada
	clave = ftok(DIRECTORIO, SALA_ENTRADA);
	if((recep_sala_entrada_qid = msgget(clave, 0660)) == -1){
		perror("inicializarComunicacion: error al crear la cola de respuesta");
		return false;
	}
	return true;
}

bool ComunicacionBus::recibir_mensaje_gim( void * msg, int msg_size, long msgtype ){
	if(msgrcv(recep_gim_qid, msg, msg_size - sizeof(long), msgtype,0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "El gimnasio ha sido destruido");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "Hubo un error desconocido al recibir un mensaje del gimnasio");
		}
		return false;
	}
	return true;
}

bool ComunicacionBus::enviar_mensaje_socio(const void * msg, int msg_size){
	if(msgsnd(envio_socio_qid, msg, msg_size - sizeof(long), 0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "La cola de salida del sistema ha sido destruida");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "Hubo un error desconocido al enviar un mensaje al socio");
		}
		return false;
	}
	return true;
}

bool ComunicacionBus::recibir_mensaje_sala_entrada( void * msg, int msg_size, long msgtype ){
	if(msgrcv(recep_sala_entrada_qid, msg, msg_size - sizeof(long), msgtype,0) == -1) {
		if(errno == EINVAL || errno == EIDRM){
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "La sala de entrada ha sido destruida");
		}else{
			char printBuffer[200];
			UPRINTLN( "ComunicacionBus", printBuffer, "Hubo un error desconocido al recibir un mensaje de la sala de entrada");
		}
		return false;
	}
	return true;
}

