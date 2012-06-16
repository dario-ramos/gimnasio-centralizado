#include "../middleware/socio/Socio.h"

int main(){
	Socio socio;
	int puerta = Random::EnteroEnRango( 0, Constantes::CANT_PUERTAS - 1 );
	if( !socio.IngresarAlPredio(puerta) ){
		return 1;
	}
	socio.TomarBusDeSalaEntradaAGimnasio();
	socio.Ejercitar();
	socio.TomarBusDeGimnasioASalaSalida();
	puerta = Random::EnteroEnRango( 0, Constantes::CANT_PUERTAS - 1 );
	socio.SalirDelPredio(puerta);
	return 0;
}
