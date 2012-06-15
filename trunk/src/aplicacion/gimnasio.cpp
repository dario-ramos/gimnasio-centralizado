#include "../middleware/gimnasio/Gimnasio.h"

int main(){
	Gimnasio gimnasio();
	while(true){
		gimnasio.EsperarSocio();
		gimnasio.ReportarEstado(); //Mostrar por pantalla cuantos socios hay y otras estadisticas de interes
	}
	return 0;
}
