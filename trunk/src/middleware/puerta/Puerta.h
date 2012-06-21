#pragma once
#include <string>
#include "../../common/Constantes.h"
#include "../../common/Uprintf.h"
#include "../../common/Random.h"
#include "../servidor_ids/servidor_ids.h"
#include "ComunicacionPuerta.h"
#include "../common/baseComunicacion.h"

using namespace std;

class Puerta {
public:
	Puerta(const string & ip_srv_ids);
	~Puerta();

	MsjSocio &EsperarSocio();
	bool IngresarSocio();
	bool EgresarSocio();
	void BorrarSocio();
	void NotificarExito();//notifica que la operacion se pudo llevar a cabo (entrada o salida)
	void NotificarFallo();//notifica que la operacion no se pudo llevar a cabo (entrada o salida)
	

private:
	int id;
	int nroPuerta;
	string ip_servidor_ids;
	MsjSocio socioActual;

	ShmCantidadSocios *shmPuertas;
	ShmBus *shmBus;
	int shmPuertasId, shmBusId, semBus, mutexShmBus, mutexPuertas;
	ComunicacionPuerta comunicacion;
	CLIENT *clnt;

	bool ObtenerMemoriaCompartidaPuertas();
	bool ObtenerMemoriaCompartidaBus();
	bool ObtenerSemaforoBus();
	bool liberarMemoriaCompartidaPuertas();
	bool liberarMemoriaCompartidaBus();
	bool PedirId();
	bool DevolverId();

	
};
