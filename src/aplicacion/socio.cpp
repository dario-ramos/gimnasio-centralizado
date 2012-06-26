#include "../middleware/socio/Socio.h"
#include "../common/Random.h"
#include "../common/Constantes.h"

int main(){
	Socio socio( "localhost" );
	int puerta = Random::EnteroEnRango( 1, Constantes::CANT_PUERTAS );
	if( !socio.IngresarAlPredio(puerta) ){
		return 1;
	}
	socio.TomarBusDeSalaEntradaAGimnasio();
	socio.Ejercitar();
	puerta = Random::EnteroEnRango( 1, Constantes::CANT_PUERTAS );
	socio.TomarBusDeGimnasioASalida(puerta);
	socio.SalirDelPredio();
	return 0;
}
