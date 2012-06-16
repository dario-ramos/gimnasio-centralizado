#pragma once
#include <string>
#include "../puerta/Puerta.h"
#include "../../common/Constantes.h"
#include "../../common/Uprintf.h"
#include "../../common/Random.h"
#include "../servidor_ids/servidor_ids_clientes.h"
#include "../common/Comunicacion.h"

using namespace std;

class Socio{
public:
	Socio(string & ip_serv_ids);
	~Socio();
	bool IngresarAlPredio( int puerta );
	void TomarBusDeSalaEntradaAGimnasio();
	void Ejercitar();
	void TomarBusDeGimnasioASalaSalida();
	void SalirDelPredio(int puerta);

private: 
	bool PedirId();
	bool DevolverId();
	int id;
    string ip_servidor_ids;

    CLIENT *clnt;
};
