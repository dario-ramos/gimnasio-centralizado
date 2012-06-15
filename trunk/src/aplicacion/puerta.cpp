#include "../middleware/puerta/Puerta.h"

int main(){
	Puerta puerta; //TODO <CHK> ¿Pedir id de la puerta a servidor, o recibir como argumento?
	while(true){
		const Socio& socio  = puerta.EsperarSocio();
		if( socio.Entrando() )
			puerta.IngresarSocio( socio );
		else
			puerta.EgresarSocio( socio );
	}
	return 0;
}
