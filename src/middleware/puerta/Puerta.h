#pragma once
#include <string>
#include "ComunicacionPuerta.h"
#include "../common/baseComunicacion.h"
#include "../servidor_ids/servidor_ids.h"

using namespace std;

class Puerta {
public:
	Puerta(const string & ip_srv_ids);
	~Puerta();

	MsjSocio &EsperarSocio();
	bool IngresarSocio();
	bool EgresarSocio();
	void BorrarSocio();
	void NotificarSocio(Operaciones op, Resultado res);

private:
//Estado
	int id;
	int nroPuerta;
	string ip_servidor_ids;
	MsjSocio socioActual;

	ShmCantidadSocios *shmPuertas;
	ShmBus *shmBus;
	int shmPuertasId, shmBusId, semBus, mutexShmBus, mutexPuertas;
	ComunicacionPuerta comunicacion;
	CLIENT *clnt;
//Comportamiento
	bool ObtenerMemoriaCompartidaPuertas();
	bool ObtenerMemoriaCompartidaBus();
	bool ObtenerSemaforoBus();
	bool liberarMemoriaCompartidaPuertas();
	bool liberarMemoriaCompartidaBus();
	bool PedirId();
	bool DevolverId();
	bool ingresarSocioMemoriaComparida();
	bool sacarSocioMemoriaComparida();
	//Prohibido copia y asignacion
	Puerta( const Puerta& );
	Puerta& operator=( const Puerta& );
};
