#pragma once
#include <vector>
#include "../common/baseComunicacion.h"
#include "ComunicacionBus.h"
#include "../servidor_ids/servidor_ids.h"

using namespace std;
class Bus {
public:
	Bus(const string & ip_srv_ids);
	~Bus();
	void BajarPasajeros();
	void SubirPasajeros();
	void ViajarProximoDestino();

	enum Posicion { PUERTA, TRANSITO, GIMNACION };


private:
	string ip_servidor_ids;
	int id;
	MsjSocio socios[CAPACIDAD_BUS];
	ComunicacionBus comunicacion;
	CLIENT *clnt;
	ShmBus *shmBus;
	int shmBusId, semBus, mutexShmBus;


	bool PedirId();
	bool DevolverId();
	bool ObtenerMemoriaCompartida();
	bool ObtenerSemaforo();
};

Bus::Bus(const string & ip_srv_ids) : ip_servidor_ids(ip_srv_ids) {
	if(!PedirId()){
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "No pudo obtener Id.");
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
	id = result_1->retorno_u.id;
	return true;
}

bool Bus::DevolverId() {
    retorno  *result_1;
    result_1 = devolver_id_puerta_1(&id, clnt);
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

bool Bus::ObtenerSemaforo() {
	/*Obtengo el semaforo del bus*/
	if((semBus = getsem(SEM_BUS)) == -1){
		perror("servidor: error al crear el semaforo");
		return false;
	}
	return true;
}
