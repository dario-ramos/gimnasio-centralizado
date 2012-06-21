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

	enum Posicion { PUERTA, TRANSITO, GIMNACIO };


private:
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

	bool ObtenerMemoriaCompartida();
	bool ObtenerSemaforo();
	bool PedirId();
	bool DevolverId();
	void subirPasajerosPuerta();
	void subirPasajerosGimnacio();

};
