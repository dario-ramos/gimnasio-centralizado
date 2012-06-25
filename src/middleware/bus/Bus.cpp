#include "Bus.h"


Bus::Bus(const string & ip_srv_ids) : ip_servidor_ids(ip_srv_ids) {
	if(!PedirId()){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "No pudo obtener Id.");
	}
	if(!ObtenerMemoriaCompartida()){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d Error al obtener la memoria compartida", id);
	}
	if(!ObtenerSemaforo()){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d Error al obtener el semaforo.", id);
	}

}

bool Bus::PedirId() {
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
	nroBus = result_1->retorno_u.id;
	id = BASE_ID_BUS + nroBus;
	return true;
}

bool Bus::DevolverId() {
    retorno  *result_1;
    result_1 = devolver_id_puerta_1(&nroBus, clnt);
    if (result_1 == (retorno *) NULL) {
            clnt_perror (clnt, "Error al devolver el Id");
            return false;
    }
    clnt_destroy (clnt);
    return true;
}

bool Bus::ObtenerMemoriaCompartida() {
	key_t clave;

	/*Creo la memoria compartida entre puertas*/
	struct stat fileInfo;
	if( !( stat(DIRECTORIO,&fileInfo) == 0 && S_ISDIR(fileInfo.st_mode) ) ){
		printf( "El directorio del ftok, %s, no existe\n", DIRECTORIO );
		return false;
	}
	clave = ftok(DIRECTORIO, SHM_SALAS + nroBus);
	if((shmBusId = shmget(clave, sizeof(ShmBus),  0660)) == -1){
		perror("servidor: error obteniendo la memoria compartida");
		return false;
	}
	if((shmBus = (ShmBus *) shmat(shmBusId, 0, 0)) == (ShmBus * ) -1){
		perror("servidor: error al vincular la memoria compartida");
		return false;
	}

	/*Obtengo el semaforo para la memoria compartida entre puertas*/
	if((mutexShmBus = getsem(SEM_SHM_BUS + nroBus)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

bool Bus::ObtenerSemaforo() {
	/*Obtengo el semaforo del bus*/
	if((semBus = getsem(SEM_BUS + nroBus)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}

void Bus::SubirPasajeros(){
	if(posicion == PUERTA){
		subirPasajerosPuerta();
	} else if (posicion == GIMNACIO) {
		subirPasajerosGimnacio();
	} else {
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d No se puede subir pasajeros estando en transito.", id);
	}
}

void Bus::subirPasajerosPuerta() {
	if (cantidadPasajeros != 0){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d No se puede inicial la subidad de pasajeros a menos que el bus este vacio.", id);
		return;
	}
	p(mutexShmBus);
	//verifico si tiene que esperar
	bool esperar = shmBus->entrada == 0 && shmBus->salida == 0;
	v(mutexShmBus);
	if (esperar){
		//espera
		p(semBus);
		esperar = false;
	}
	int i = 0;
	bool subir_pasajero = !esperar;
	while (subir_pasajero){
		comunicacion.recibir_mensaje_sala_entrada(socios+i, sizeof(*socios+i), id);
		p(mutexShmBus);
		shmBus->entrada --;
		v(mutexShmBus);
		notificarPasajero(*(socios+i), Operaciones::SUBIR_AL_BUS, Resultado::EXITO);
		p(mutexShmBus);
		subir_pasajero = shmBus->entrada > 0 && i < CAPACIDAD_BUS;
		v(mutexShmBus);
	}
	cantidadPasajeros = i;
}

void Bus::subirPasajerosGimnacio() {
	if (cantidadPasajeros != 0){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d No se puede inicial la subidad de pasajeros a menos que el bus este vacio.", id);
		return;
	}
	p(mutexShmBus);
	//verifico si tiene que esperar
	bool esperar = shmBus->entrada == 0 && shmBus->salida == 0;
	v(mutexShmBus);
	if (esperar){
		//espera
		p(semBus);
		esperar = false;
	}
	int i = 0;
	bool subir_pasajero = !esperar;
	while (subir_pasajero){
		comunicacion.recibir_mensaje_gim(socios+i, sizeof(*socios+i), id);
		p(mutexShmBus);
		shmBus->salida --;
		v(mutexShmBus);
		notificarPasajero(*(socios+i), Operaciones::SUBIR_AL_BUS, Resultado::EXITO);
		p(mutexShmBus);
		subir_pasajero = shmBus->salida > 0 && i < CAPACIDAD_BUS;
		v(mutexShmBus);
	}
	cantidadPasajeros = i;
}

void Bus::notificarPasajero(MsjSocio &socio, Operaciones op, Resultado res) {
	MsjRespSocio rsp;
	rsp.idSocio = socio.idSocio;
	rsp.codOp = op; //se subio al bus.
	rsp.codResultado = res;
	rsp.tipo = socio.idSocio;
	comunicacion.enviar_mensaje_socio(&rsp, sizeof(rsp));
}

void Bus::ViajarProximoDestino(){
	Posicion destino;
	if(posicion == PUERTA) {
		destino = GIMNACIO;
	} else if (posicion == GIMNACIO) {
		destino = PUERTA;
	} else {
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d El bus se encuentra en transito.", id);
	}
	posicion = TRANSITO;
	sleep(15);
	posicion = destino;
}

void Bus::BajarPasajeros(){
	if(posicion == PUERTA || posicion == GIMNACIO){
		int i;
		for (i = 0; i < cantidadPasajeros; i++){
			notificarPasajero(*(socios+i), Operaciones::BAJAR_DEL_BUS, Resultado::EXITO);
		}
		cantidadPasajeros -= i;
	} else {
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d No se puede bajar pasajeros estando en transito.", id);
	}
}
