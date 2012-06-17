#include "Socio.h"


Socio::Socio(string &ip_serv_ids) : ip_servidor_ids(ip_serv_ids){
	if(!PedirId()){
		char printBuffer[200];
		UPRINTLN( "Socio", printBuffer, "No pudo obtener Id.");
	}
}

Socio::~Socio(){
	if(!DevolverId()){
		char printBuffer[200];
		UPRINTLN( "Socio", printBuffer, "%d no pudo devolver Id.", id);
	}
}

bool Socio::IngresarAlPredio( int puerta ){
	char printBuffer[200];
	UPRINTLN( "Socio", printBuffer, "Socio %d decidio entrar por puerta %d", id, puerta ); //TODO <NIM> Imprimir id de socio

	return true;
}

void Socio::TomarBusDeSalaEntradaAGimnasio(){
	//throw std::runtime_error( "Not implemented" );
}

void Socio::Ejercitar(){
	//throw std::runtime_error( "Not implemented" );
}

void Socio::TomarBusDeGimnasioASalaSalida(){
	//throw std::runtime_error( "Not implemented" );
}

void Socio::SalirDelPredio(int puerta){
	//throw std::runtime_error( "Not implemented" );
}

bool Socio::PedirId() {
	retorno  *result_1;
	char *obtener_nuevo_id_cliente_1_arg;
	clnt = clnt_create (ip_servidor_ids.c_str(), SERVIDOR_IDS_PROG, SERVIDOR_IDS_VERS, "udp");
	if (clnt == NULL) {
			clnt_pcreateerror (ip_servidor_);
			return false;
	}
	result_1 = obtener_nuevo_id_socio_1((void*)&obtener_nuevo_id_cliente_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
			clnt_perror (clnt, "Error al obtener el Id");
			return false;
	}
	id = result_1->retorno_u.id;
	return true;
}

bool Socio::DevolverId() {
    retorno  *result_1;
    result_1 = devolver_id_socio_1(&id, clnt);
    if (result_1 == (retorno *) NULL) {
            clnt_perror (clnt, "Error al obtener el Id");
            return false;
    }
    clnt_destroy (clnt);
    return true;
}
