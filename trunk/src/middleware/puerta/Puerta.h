#pragma once
#include <string>
#include "../../common/Constantes.h"
#include "../../common/Uprintf.h"
#include "../../common/Random.h"
#include "../servidor_ids/servidor_ids.h"
#include "../common/Comunicacion.h"

using namespace std;

class Puerta {
public:
	Puerta(string & ip_srv_ids);
	~Puerta();

	MsjSocio &EsperarSocio();
	void IngresarSocio();
	void EgresarSocio();
	

private:
	long int id; //Lo hago long int para que coincida con el tipo de mensaje.
	string ip_servidor_ids;
	CLIENT *clnt;
	Comunicacion comunicacion;
	bool PedirId();
	bool DevolverId();
	
};
