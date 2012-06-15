#include "../middleware/bus/Bus.h"

int main(){
	//Comienza originalmente en la sala de entrada
	Bus bus( Enums::UbicacionBus::SalaEntrada ); //TODO <CHK> ¿Pedir id de la sala a servidor, o recibir como argumento?
	while(true){
		bus.EsperarQuePasajerosAsciendan();
		bus.ViajarAProximoDestino();
		bus.EsperarQuePasajerosDesciendan();
	}
	return 0;
}
