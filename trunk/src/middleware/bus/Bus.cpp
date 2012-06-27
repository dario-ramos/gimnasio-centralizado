#include "Bus.h"
#include "../common/semaforo.h"
#include "../../common/Uprintf.h"
#include "../../common/Random.h"

Bus::Bus(const string & ip_srv_ids) :
		id(-1), nroBus(-1),
		ip_servidor_ids(ip_srv_ids),
		socios(),
		cantidadPasajeros(0),
		comunicacion(),
		shmBus( NULL ),
		shmBusId(-1), semBus(-1), mutexShmBus(-1),
		posicion( POS_PUERTA ),
		clnt( NULL ) {
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
		clnt_pcreateerror (ip_servidor_ids.c_str());
		return false;
	}
	result_1 = obtener_nuevo_id_bus_1((void*)&obtener_nuevo_id_cliente_1_arg, clnt);
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
    result_1 = devolver_id_bus_1(&nroBus, clnt);
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
	clave = ftok(DIRECTORIO, SHM_BUS + nroBus);
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
	if(posicion == POS_PUERTA){
		subirPasajerosPuerta();
	} else if (posicion == POS_GIMNASIO) {
		subirPasajerosGimnacio();
	} else {
		char printBuffer[200];
		UPRINTLN( "Bus", printBuffer, "%d No se puede subir pasajeros estando en transito.", id);
	}
}

void Bus::subirPasajerosPuerta() {
	char printBuffer[200];
	if (cantidadPasajeros != 0){
		UPRINTLN( "Bus", printBuffer, "%d No se puede inicial la subidad de pasajeros a menos que el bus este vacio.", id);
		return;
	}
	p(mutexShmBus);
	//verifico si tiene que esperar
	bool esperar = shmBus->entrada == 0 && shmBus->salida == 0;
	v(mutexShmBus);
	if (esperar){
		//espera
		UPRINTLN( "Bus", printBuffer, "%d: No hay pasajeros en la entrada ni salida, el bus permanece parado en la puerta.", id);
		p(mutexShmBus);
		shmBus->estado = BUS_ESPERANDO;
		v(mutexShmBus);
		p(semBus);
		esperar = false;
		UPRINTLN( "Bus", printBuffer, "%d: Se ha notificado el ingreso de un pasajero, el bus entra en funcionamiento.", id);
	}
	int i = 0;
	p(mutexShmBus);
	//verifico si hay pasajeros a la entrada
	int cant_entrada = shmBus->entrada;
	int cant_salida = shmBus->salida;
	bool subir_pasajero = shmBus->entrada > 0;
	v(mutexShmBus);
	while (subir_pasajero){
		UPRINTLN( "Bus", printBuffer, "%d: Va a subir un pasajero!", id);
		comunicacion.recibir_mensaje_sala_entrada(socios+i, sizeof(socios[i]), id);
		UPRINTLN( "Bus", printBuffer, "%d: Se sube el pasajero %d al bus", id, (socios+i)->idSocio);
		p(mutexShmBus);
		shmBus->entrada --;
		v(mutexShmBus);
		UPRINTLN( "Bus", printBuffer, "%d: Decrementa shmBus->entrada", id);
		notificarPasajero(*(socios+i), OPS_SUBIR_AL_BUS, RES_EXITO);
		UPRINTLN( "Bus", printBuffer, "%d: Notifico al pasajero", id);
		i++;
		p(mutexShmBus);
		subir_pasajero = shmBus->entrada > 0 && i < CAPACIDAD_BUS;
		v(mutexShmBus);
		UPRINTLN( "Bus", printBuffer, "%d: Hace la validacion", id);
		sleep(Random::EnteroEnRango(1,5));
	}
	cantidadPasajeros = i;

	UPRINTLN( "Bus", printBuffer, "%d Se han subido %d pasajeros. Hay %d pasajeros en la entrada y %d en la salida", id, cantidadPasajeros, cant_entrada, cant_salida);

}

void Bus::subirPasajerosGimnacio() {
	char printBuffer[200];
	if (cantidadPasajeros != 0){
		UPRINTLN( "Bus", printBuffer, "%d No se puede inicial la subidad de pasajeros a menos que el bus este vacio.", id);
		return;
	}
	p(mutexShmBus);
	//verifico si tiene que esperar
	bool esperar = shmBus->entrada == 0 && shmBus->salida == 0;
	v(mutexShmBus);
	if (esperar){
		//espera
		UPRINTLN( "Bus", printBuffer, "%d: No hay pasajeros en la entrada ni salida, el bus permanece parado en el gimansio.", id);
		p(mutexShmBus);
		shmBus->estado = BUS_ESPERANDO;
		v(mutexShmBus);
		p(semBus);
		esperar = false;
		UPRINTLN( "Bus", printBuffer, "%d: Se ha notificado el ingreso de un pasajero, el bus entra en funcionamiento.", id);
	}
	int i = 0;
	p(mutexShmBus);
	//verifico si hay pasajeros a la salida
	bool subir_pasajero = shmBus->salida > 0;
	v(mutexShmBus);
	while (subir_pasajero){
		comunicacion.recibir_mensaje_gim(socios+i, sizeof(socios[i]), id);
		p(mutexShmBus);
		shmBus->salida --;
		v(mutexShmBus);
		notificarPasajero(*(socios+i), OPS_SUBIR_AL_BUS, RES_EXITO);
		i++;
		p(mutexShmBus);
		subir_pasajero = shmBus->salida > 0 && i < CAPACIDAD_BUS;
		v(mutexShmBus);
		sleep(Random::EnteroEnRango(1,5));
	}
	cantidadPasajeros = i;
	UPRINTLN( "Bus", printBuffer, "%d Se han subido %d pasajeros.", id, cantidadPasajeros);
}

void Bus::notificarPasajero(MsjSocio &socio, Operaciones op, Resultado res) {
	char printBuffer[200];
	MsjRespSocio rsp;
	rsp.idSocio = socio.idSocio;
	rsp.codOp = op; //se subio al bus.
	rsp.codResultado = res;
	rsp.tipo = socio.idSocio;
	UPRINTLN( "Bus", printBuffer, "%d Se va ha enviar mensaje al socio %d.", id, socio.idSocio);
	comunicacion.enviar_mensaje_socio(&rsp, sizeof(rsp));
	UPRINTLN( "Bus", printBuffer, "%d Se envia mensaje al socio %d.", id, socio.idSocio);
}

void Bus::ViajarProximoDestino(){
	char printBuffer[200];
	Posicion destino;
	if(posicion == POS_PUERTA) {
		destino = POS_GIMNASIO;
		UPRINTLN( "Bus", printBuffer, "%d El bus va a viajar al gimnasio", id);//TODO emprolijar
	} else if (posicion == POS_GIMNASIO) {
		destino = POS_PUERTA;
		UPRINTLN( "Bus", printBuffer, "%d El bus va a viajar a la puerta", id);
	} else {
		UPRINTLN( "Bus", printBuffer, "%d El bus se encuentra en transito. No es posible subir pasajeros", id);
	}

	posicion = POS_TRANSITO;

	p(mutexShmBus);
	shmBus->estado = posicion;
	v(mutexShmBus);
	UPRINTLN( "Bus", printBuffer, "%d El bus se encuentra en transito.", id);
	sleep(Random::EnteroEnRango(10,20));
	posicion = destino;

	p(mutexShmBus);
	shmBus->estado = posicion;
	v(mutexShmBus);
	UPRINTLN( "Bus", printBuffer, "%d El bus se llego a su destino", id);
}



void Bus::BajarPasajeros(){
	char printBuffer[200];
	if(posicion == POS_PUERTA || posicion == POS_GIMNASIO){
		UPRINTLN( "Bus", printBuffer, "%d El bus se dispone a bajar los pasajeros, cantidad de pasajeros a bajar = %d", id, cantidadPasajeros);
		int i;
		for (i = 0; i < cantidadPasajeros; i++){
			notificarPasajero(*(socios+i), OPS_BAJAR_DEL_BUS, RES_EXITO);
			sleep(Random::EnteroEnRango(1,5));
		}
		cantidadPasajeros -= i;
	} else {
		UPRINTLN( "Bus", printBuffer, "%d No se puede bajar pasajeros estando en transito.", id);
	}
}

