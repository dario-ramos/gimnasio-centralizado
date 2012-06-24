#include "../middleware/puerta/Puerta.h"

int main(){
	Puerta puerta("localhost"); //TODO <CHK> ¿Pedir id de la puerta a servidor, o recibir como argumento?
	while(true){
		const MsjSocio &socio  = puerta.EsperarSocio();
		if( socio.operacion == Operaciones::ENTRAR_AL_PREDIO )
			puerta.IngresarSocio();
		else
			puerta.EgresarSocio();
		puerta.BorrarSocio();
	}
	return 0;
}
