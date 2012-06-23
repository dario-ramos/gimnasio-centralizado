#include "Puerta.h"

Puerta::Puerta(const string & ip_srv_ids) : ip_servidor_ids(ip_srv_ids) {
	if(!PedirId()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%No se pudo obtener id y nro de puerta.");
	}
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
		UPRINTLN( "Puerta", printBuffer, "%d Error al devolver el nro de puerta", nroPuerta);
	}
	if (!liberarMemoriaCompartidaPuertas()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d Error al desasociarse de la memoria compartida entre puertas", id);
	}
	if (!liberarMemoriaCompartidaBus()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d Error al desasociarse de la memoria compartida del bus", id);
	}
}

bool Puerta::PedirId() {
	retorno  *result_1;
	char *obtener_nuevo_id_cliente_1_arg;
	clnt = clnt_create (ip_servidor_ids.c_str(), SERVIDOR_IDS_PROG, SERVIDOR_IDS_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (ip_servidor_);
		return false;
	}
	result_1 = obtener_nuevo_id_puerta_1((void*)&obtener_nuevo_id_cliente_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "Error al obtener el Id");
		return false;
	}
	nroPuerta = result_1->retorno_u.id;
	id = BASE_ID_PUERTA + nroPuerta;
	return true;
}

bool Puerta::DevolverId() {
    retorno  *result_1;
    result_1 = devolver_id_puerta_1(&nroPuerta, clnt);
    if (result_1 == (retorno *) NULL) {
            clnt_perror (clnt, "Error al devolver el Id");
            return false;
    }
    clnt_destroy (clnt);
    return true;
}


MsjSocio & Puerta::EsperarSocio() {
	comunicacion.recibir_mensaje(&socioActual, sizeof(socioActual), id);
	return socioActual;
}

bool Puerta::IngresarSocio() {
	if(ingresarSocioMemoriaComparida()){
		if (comunicacion.enviar_mensaje_bus(&socioActual, sizeof(socioActual)) == -1) {
			sacarSocioMemoriaComparida();
			NotificarSocio(Operaciones::ENTRAR_AL_PREDIO, Resultado::FALLO);
			return false;
		} else {
			p(mutexShmBus);//tomo mutex de memoria compartida con el bus
			shmBus->entrada++;
			if(shmBus->entrada == 1 && shmBus->salida == 0) { //estaban las 2 en 0, el bus estaba parado
				v(semBus);
			}
			v(mutexShmBus);//libero mutex de memoria compartida con el bus
			NotificarSocio(Operaciones::ENTRAR_AL_PREDIO, Resultado::EXITO);
		}
		return true;
	} else {
		NotificarSocio(Operaciones::ENTRAR_AL_PREDIO, Resultado::FALLO);
		return false;
	}

}

bool Puerta::EgresarSocio() {
	char printBuffer[200];
	if (sacarSocioMemoriaComparida()){
		NotificarSocio(Operaciones::SALIR_DEL_PREDIO, Resultado::EXITO);
		UPRINTLN( "Puerta", printBuffer, "%d: El socio %d ha salido del predio", id, socioActual.idSocio);
		return true;
	} else {
		NotificarSocio(Operaciones::SALIR_DEL_PREDIO, Resultado::FALLO);
		UPRINTLN( "Puerta", printBuffer, "%d: HUbo un error al abrir la puerta para el socio %d", id, socioActual.idSocio);
		return false;
	}
}

void Puerta::NotificarSocio(Operaciones op, Resultado res) {
	MsjRespSocio respuesta;
	respuesta.codOp = op;
	respuesta.codResultado = res;
	respuesta.idSocio = socioActual.idSocio;
	respuesta.tipo = socioActual.idSocio;
	comunicacion.enviar_mensaje(&respuesta, sizeof(respuesta));
}

bool Puerta::ObtenerMemoriaCompartidaPuertas() {
	key_t clave;

	/*Creo la memoria compartida entre puertas*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d El directorio del ftok, %s, no existe\n", id, DIRECTORIO);
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
	clave = ftok(DIRECTORIO, SHM_SALAS+nroPuerta);
	if((shmBusId = shmget(clave, sizeof(ShmBus),  0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		return false;
	}
	if((shmBus = (ShmBus *) shmat(shmBusId, 0, 0)) == (ShmBus * ) -1){
		perror("servidor: error al vincular la memoria compartida");
		return false;
	}

	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((mutexShmBus = getsem(SEM_SHM_BUS+nroPuerta)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

bool Puerta::ObtenerSemaforoBus() {
	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((semBus = getsem(SEM_BUS+nroPuerta)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

void Puerta::BorrarSocio() {
	socioActual.operacion = 0;
	socioActual.idSocio = 0;
	socioActual.nroPuerta = 0;
	socioActual.tipo = 0;
}


bool Puerta::ingresarSocioMemoriaComparida(){
	p(mutexPuertas);
	int cantSociosAdentro = shmPuertas->cantidad;
	if (cantSociosAdentro >= CAPACIDAD_PREDIO) {
		v(mutexPuertas);
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "El predio esta lleno.");
		return false;
	}
	shmPuertas->cantidad++;
	v(mutexPuertas);
	return true;
}

bool Puerta::sacarSocioMemoriaComparida(){
	p(mutexPuertas);
	if (shmPuertas->cantidad <= 0) {
		v(mutexPuertas);
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "El predio esta vacio. Error fatal en el sistema!");
		return false;
	}
	shmPuertas->cantidad--;
	v(mutexPuertas);
	return true;
}
