#pragma once
#include "../common/baseComunicacion.h"
#include "ComunicacionGimnasio.h"

using namespace std;
class Gimnasio {
public:
	Gimnasio();
	void AtenderSocio();

	enum Posicion { PUERTA, TRANSITO, GIMNACIO };

private:
//Estado
	int id;
	ComunicacionGimnasio comunicacion;
	ShmBus *shmBus[CANT_BUSES];
	int shmBusId[CANT_BUSES], semBus[CANT_BUSES], mutexShmBus[CANT_BUSES];
//Comportamiento
	bool ObtenerMemoriasCompartidas();
	bool ObtenerSemaforos();
	void NotificarSocio(int idSocio, Operaciones op, Resultado res);
	//Prohibo copia y asignacion
	Gimnasio( const Gimnasio& );
	Gimnasio& operator=( const Gimnasio& );
};

