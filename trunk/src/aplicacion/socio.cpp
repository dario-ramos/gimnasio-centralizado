#include "../middleware/socio/Socio.h"

int main(){
	Socio socio;
	if( !socio.IngresarAlPredio() ){
		return 1;
	}
	socio.TomarBusDeSalaEntradaAGimnasio();
	socio.Ejercitar();
	socio.TomarBusDeGimnasioASalaSalida();
	socio.SalirDelPredio();
	return 0;
}
