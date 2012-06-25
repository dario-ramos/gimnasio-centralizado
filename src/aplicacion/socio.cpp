#include "../middleware/socio/Socio.h"
#include "../common/Random.h"
#include "../common/Constantes.h"

int main(){
	Socio socio( "localhost" );
	int puerta = Random::EnteroEnRango( 0, Constantes::CANT_PUERTAS - 1 );
	if( !socio.IngresarAlPredio(puerta) ){
		return 1;
	}
	socio.TomarBusDeSalaEntradaAGimnasio();
	socio.Ejercitar();
	puerta = Random::EnteroEnRango( 0, Constantes::CANT_PUERTAS - 1 );
	socio.TomarBusDeGimnasioASalida(puerta);
	socio.SalirDelPredio();
	return 0;
}
