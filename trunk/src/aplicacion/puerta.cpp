#include "../middleware/puerta/Puerta.h"

int main(){
	Puerta puerta(ID_PUERTA_1); //TODO <CHK> ¿Pedir id de la puerta a servidor, o recibir como argumento?
	while(true){
		const MsjSocio &socio  = puerta.EsperarSocio();
		if( socio.entrando )
			if (puerta.IngresarSocio())
				puerta.NotificarExito();
			else
				puerta.NotificarFallo();
		else
			if(puerta.EgresarSocio())
				puerta.NotificarExito();
			else
				puerta.NotificarFallo();
		puerta.BorrarSocio();
	}
	return 0;
}
