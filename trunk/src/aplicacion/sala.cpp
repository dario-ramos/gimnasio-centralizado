#include "../middleware/sala/Sala.h"

int main(){
	Sala sala; //TODO <CHK> ¿Pedir id de la sala a servidor, o recibir como argumento?
	while(true){
		const Socio& socio  = sala.EsperarSocio();
		sala.PonerSocioAEsperarBus( socio );
	}
	return 0;
}
