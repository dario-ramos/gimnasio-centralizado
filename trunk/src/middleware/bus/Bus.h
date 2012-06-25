#pragma once
#include "../common/baseComunicacion.h"
#include "ComunicacionBus.h"
#include "../servidor_ids/servidor_ids.h"
#include <string>

using namespace std;
class Bus {
public:
	Bus(const string & ip_srv_ids);
	void BajarPasajeros();
	void SubirPasajeros();
	void ViajarProximoDestino();

	enum Posicion { POS_PUERTA, POS_TRANSITO, POS_GIMNASIO };

private:
//Estado
	int id;
	int nroBus;
	string ip_servidor_ids;
	MsjSocio socios[CAPACIDAD_BUS];
	int cantidadPasajeros;
	ComunicacionBus comunicacion;
	ShmBus *shmBus;
	int shmBusId, semBus, mutexShmBus;
	Posicion posicion;
	CLIENT *clnt;
//Comportamiento
	bool ObtenerMemoriaCompartida();
	bool ObtenerSemaforo();
	bool PedirId();
	bool DevolverId();
	void subirPasajerosPuerta();
	void subirPasajerosGimnacio();
	void bajarPasajerosPuerta();
	void bajarPasajerosGimnacio();
	void notificarPasajero(MsjSocio &socio, Operaciones op, Resultado res);
	//Prohibo copia y asignacion
	Bus( const Bus& );
	Bus& operator=( const Bus& );
};
