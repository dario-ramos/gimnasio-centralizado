#pragma once

class Socio{
public:
	Socio();
	~Socio();
	void IngresarAlPredio( int puerta );
	void TomarBusDeSalaEntradaAGimnasio();
	void Ejercitar();
	void TomarBusDeGimnasioASalaSalida();
	void SalirDelPredio(int puerta);

private: 
	bool PedirId();
	bool DevolverId();
	int id; 
};
