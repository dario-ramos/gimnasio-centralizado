#include "../middleware/bus/Bus.h"

int main(){
	//Comienza originalmente en la sala de entrada
	Bus bus("localhost"); //TODO <CHK> ¿Pedir id de la sala a servidor, o recibir como argumento?
	while(true){
		bus.SubirPasajeros();
		bus.ViajarProximoDestino();
		bus.BajarPasajeros();
	}
	return 0;
}
