#include "Puerta.h"

Puerta::Puerta(string & ip_srv_ids) : ip_servidor_ids(ip_srv_ids) {
	if(!PedirId()){
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "No pudo obtener Id.");
	}
	if(!ObtenerMemoriaCompartidaPuertas()) {
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "No se pudo obtener la memoria compartida entre puertas.");
	}
}

Puerta::~Puerta() {
	if(!DevolverId()){
		char printBuffer[200];
		UPRINTLN( "Puerta", printBuffer, "%d No pudo devolver Id.", id);
	}
}

MsjSocio & Puerta::EsperarSocio() {
}

void Puerta::IngresarSocio() {
}

void Puerta::EgresarSocio() {
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
	id = result_1->retorno_u.id;
	return true;
}

bool Puerta::DevolverId() {
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
