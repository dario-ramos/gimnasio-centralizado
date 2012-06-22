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
	UPRINTLN( "Socio", printBuffer, "Socio %d decidio entrar por puerta %d", id, puerta );
	MsjSocio msj;
	MsjRespSocio msjRespuesta;
	msj.idSocio = id;
	msj.nroPuerta = puerta;
	msj.operacion = Operaciones::ENTRAR_AL_PREDIO;
	msj.tipo = BASE_ID_PUERTA + puerta;

	comunicacion.enviar_mensaje(&msj, sizeof(msj));
	comunicacion.recibir_mensaje(&msjRespuesta, sizeof(msjRespuesta), id);
	if (msjRespuesta.codOp != Operaciones::ENTRAR_AL_PREDIO) {
		UPRINTLN( "Socio", printBuffer, "%d Error en la puerta.", id);
		return false;
	}
	if (msjRespuesta.codResultado != Resultado::EXITO){
		UPRINTLN( "Socio", printBuffer, "%d No pudo entrar, predio lleno.", id);
		return false;
	}
	UPRINTLN( "Socio", printBuffer, "%d Entro al predio, y espera el bus en la sala.", id);
	return true;
}

void Socio::TomarBusDeSalaEntradaAGimnasio(){
	char printBuffer[200];
	MsjRespSocio msj;
	comunicacion.recibir_mensaje(&msj, sizeof(msj), id);
	if ( msj.idSocio != id) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (msj.codOp != Operaciones::SUBIR_AL_BUS) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (msj.codResultado != Resultado::EXITO) {
		UPRINTLN( "Socio", printBuffer, "%d Error al subir al bus.", id);
		return;
	}
	UPRINTLN( "Socio", printBuffer, "%d Subio al bus", id);

}

void Socio::Ejercitar(){
	char printBuffer[200];
	MsjRespSocio rsp;
	comunicacion.recibir_mensaje(&rsp, sizeof(rsp), id);
	if (rsp.codOp != Operaciones::BAJAR_DEL_BUS) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus, El socio esperaba bajarse del bus.", id);
		return;
	}
	if (rsp.codResultado != Resultado::EXITO) {
		UPRINTLN( "Socio", printBuffer, "%d Hubo un error al bajarse del bus.", id);
		return;
	}
	sleep(15);
}

void Socio::TomarBusDeGimnasioASalida(int puerta){
	char printBuffer[200];
	puertaDeSalida = puerta;
	MsjSocio msj;
	msj.idSocio = id;
	msj.operacion = Operaciones::SALIR_DEL_GIMNASIO;
	msj.nroPuerta = puerta;
	msj.tipo = ID_GIMNACIO;
	comunicacion.enviar_mensaje(&msj, sizeof(msj));
	//Espero al mensaje del bus.
	MsjRespSocio rsp;
	comunicacion.recibir_mensaje(&rsp, sizeof(rsp), id);
	if ( rsp.idSocio != id) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (rsp.codOp != Operaciones::SUBIR_AL_BUS) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (rsp.codResultado != Resultado::EXITO) {
		UPRINTLN( "Socio", printBuffer, "%d Error al subir al bus.", id);
		return;
	}
	UPRINTLN( "Socio", printBuffer, "%d Subio al bus y se dirige a la puerta de salida.", id);
}

void Socio::SalirDelPredio(){
	char printBuffer[200];
	MsjRespSocio rsp;
	comunicacion.recibir_mensaje(&rsp, sizeof(rsp), id);
	if ( rsp.idSocio != id) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (rsp.codOp != Operaciones::BAJAR_DEL_BUS) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	UPRINTLN( "Socio", printBuffer, "%d Bajo del bus y va a salir del predio", id);

	MsjSocio msj;
	msj.idSocio = id;
	msj.nroPuerta = puertaDeSalida;
	msj.operacion = Operaciones::SALIR_DEL_PREDIO;
	msj.tipo = BASE_ID_PUERTA + puertaDeSalida;
	comunicacion.enviar_mensaje(&msj, sizeof(msj));

	comunicacion.recibir_mensaje(&rsp, sizeof(rsp), id);
	if ( rsp.idSocio != id) {
		UPRINTLN( "Socio", printBuffer, "%d Error en la puerta.", id);
		return;
	}
	if (rsp.codOp != Operaciones::SALIR_DEL_PREDIO) {
		UPRINTLN( "Socio", printBuffer, "%d Error en el bus.", id);
		return;
	}
	if (rsp.codResultado != Resultado::EXITO){
		UPRINTLN( "Socio", printBuffer, "%d El socio no puede salir del predio", id);
		return;
	}
	UPRINTLN( "Socio", printBuffer, "%d El socio ha salido del predio.", id);

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
