#pragma once
#include <string>
#include "../puerta/Puerta.h"
#include "../servidor_ids/servidor_ids.h"
#include "ComunicacionSocio.h"

using namespace std;

class Socio{	//TODO <NIM> Implementar manejo de errores: Devolver codigo de error en todas las funciones, o bien tirar excepciones. Lo mismo para el resto de la API
public:
	Socio( const string & ip_serv_ids );
	~Socio();
	bool IngresarAlPredio( int puerta );
	void TomarBusDeSalaEntradaAGimnasio();
	void Ejercitar();
	void TomarBusDeGimnasioASalida(int puerta);
	void SalirDelPredio();

private: 
//Estado
	int id;
	int puertaDeSalida;
	string ip_servidor_ids;
	ComunicacionSocio comunicacion;
	CLIENT *clnt;
//Comportamiento
	bool PedirId();
	bool DevolverId();
	//Prohibo copia y asignacion
	Socio( const Socio& );
	Socio& operator=( const Socio& );
};
