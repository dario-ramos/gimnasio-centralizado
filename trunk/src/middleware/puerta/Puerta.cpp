#include "Puerta.h"

Puerta::Puerta(int idPuerta) : id(idPuerta) {
	if(!ObtenerMemoriaCompartidaPuertas()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d No se pudo obtener la memoria compartida entre puertas.", id);
	}
	if(!ObtenerMemoriaCompartidaBus()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d No se pudo obtener la memoria compartida con el bus.", id);
	}
	if(!ObtenerSemaforoBus()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d No se pudo obtener el semaforo del bus.", id);
	}

}

Puerta::~Puerta() {
	if (!liberarMemoriaCompartidaPuertas()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d Error al desasociarse de la memoria compartida entre puertas", id);
	}
	if (!liberarMemoriaCompartidaBus()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d Error al desasociarse de la memoria compartida del bus", id);
	}
}

MsjSocio & Puerta::EsperarSocio() {
	comunicacion.recibir_mensaje(&socioActual, sizeof(socioActual), id);
	return socioActual;
}

bool Puerta::IngresarSocio() {
	p(mutexPuertas);
	if (shmPuertas->cantidad >= CAPACIDAD_PREDIO) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "El predio esta lleno.");
		v(mutexPuertas);
		return false;
	}
	if (comunicacion.enviar_mensaje_bus(&socioActual, sizeof(socioActual)) == -1) {
        if(errno == EINVAL || errno == EIDRM){
        	char printBuffer[200];
        	UPRINTLN( "Puerta", printBuffer, "La sala se ha destruido");
		}else{
			char printBuffer[200];
			UPRINTLN( "Puerta", printBuffer, "Hubo un error desconocido al registrar el socio en la sala.");
		}
    	v(mutexPuertas);
		return false;
	} else {
		p(mutexShmBus);//tomo mutex de memoria compartida con el bus
		shmBus->entrada++;
		if(shmBus->entrada == 1 && shmBus->salida == 0) { //estaban las 2 en 0, el bus estaba parado
			v(semBus);
		}
		v(mutexShmBus);//libero mutex de memoria compartida con el bus
		shmPuertas->cantidad++;
	}
	v(mutexPuertas);
	return true;

}

bool Puerta::EgresarSocio() {
	p(mutexPuertas);
	shmPuertas->cantidad--;
	v(mutexPuertas);
	return true;
}

void Puerta::NotificarExito() {
	MsjRespPuerta respuesta;
	respuesta.resultado = 1; //1 = exito
	respuesta.idSocio = socioActual.idSocio;
	respuesta.tipo = socioActual.idSocio;
	comunicacion.enviar_mensaje(&respuesta, sizeof(respuesta));
}

void Puerta::NotificarFallo() {
	MsjRespPuerta respuesta;
	respuesta.resultado = 0; //0 = fallo la operacion
	respuesta.idSocio = socioActual.idSocio;
	respuesta.tipo = socioActual.idSocio;
	comunicacion.enviar_mensaje(&respuesta, sizeof(respuesta));
}

bool Puerta::ObtenerMemoriaCompartidaPuertas() {
	key_t clave;

	/*Creo la memoria compartida entre puertas*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		printf( "El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}
	clave = ftok(DIRECTORIO, SHM_PUERTAS);
	if((shmPuertasId = shmget(clave, sizeof(ShmCantidadSocios),  0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		return false;
	}
	if((shmPuertas = (ShmCantidadSocios *) shmat(shmPuertasId, 0, 0)) == (ShmCantidadSocios * ) -1){
		perror("servidor: error al vincular la memoria compartida");
		return false;
	}

	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((mutexPuertas = getsem(SEM_SHM_PUERTAS)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

bool Puerta::ObtenerMemoriaCompartidaBus() {
	key_t clave;

	/*Creo la memoria compartida entre puertas*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		printf( "El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}
	clave = ftok(DIRECTORIO, SHM_SALAS);
	if((shmBusId = shmget(clave, sizeof(ShmBus),  0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		return false;
	}
	if((shmBus = (ShmBus *) shmat(shmBusId, 0, 0)) == (ShmBus * ) -1){
		perror("servidor: error al vincular la memoria compartida");
		return false;
	}

	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((mutexShmBus = getsem(SEM_SHM_BUS)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

bool Puerta::ObtenerSemaforoBus() {
	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((semBus = getsem(SEM_BUS)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

void Puerta::BorrarSocio() {
	socioActual.entrando = 0;
	socioActual.idSocio = 0;
	socioActual.nroPuerta = 0;
	socioActual.tipo = 0;
}
