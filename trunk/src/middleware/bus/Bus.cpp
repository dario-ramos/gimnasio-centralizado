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
	if (shmBus->entrada == 0 && shmBus->salida == 0){
		p(semBus);
	}
	int i;
	for (i = 0; shmBus->entrada > 0 && i < CAPACIDAD_BUS; i++) {
		comunicacion.recibir_mensaje(socios+i, sizeof(*socios+i), id);
		shmBus->entrada --;
	}
	cantidadPasajeros = i;
	v(mutexShmBus);
}


void Bus::ViajarProximoDestino(){
	//TODO <NIM>
}

void Bus::BajarPasajeros(){
	//TODO <NIM>
}
