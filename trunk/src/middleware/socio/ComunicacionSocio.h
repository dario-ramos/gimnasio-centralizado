#pragma once
#include "../common/Comunicacion.h"
#include "../common/baseComunicacion.h"

class ComunicacionSocio : public Comunicacion {

private:
	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();
};

key_t ComunicacionSocio::obtenerClaveEnvio(){
	return ftok(DIRECTORIO, COLA_ENTRADA_PUERTAS);
}

key_t ComunicacionSocio::obtenerClaveRecepcion(){
	return ftok(DIRECTORIO, COLA_SALIDA_PUERTAS);
}
