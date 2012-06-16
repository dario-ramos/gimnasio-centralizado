#pragma once


class Puerta {
public:
	Puerta();
	~Puerta();

	Socio &EsperarSocio();
	void IngresarSocio();
	void EgresarSocio();
	

private:
	long int id; //Lo hago long int para que coincida con el tipo de mensaje.
	
};
