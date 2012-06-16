#pragma once


class Puerta {
public:
	Puerta();
	~Puerta();

	Socio &EsperarSocio();
	void IngresarSocio();
	void EgresarSocio();
	

private:
	unsigned short id;
	
};
