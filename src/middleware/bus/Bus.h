#pragma once

#include "ComunicacionBus.h"

class Bus{
	ComunicacionBus comunicacionBus;
public:
	void SubirPasajeros();
	void ViajarProximoDestino();
	void BajarPasajeros();
};
