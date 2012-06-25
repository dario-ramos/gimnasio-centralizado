#pragma once

class ComunicacionBus {
public:
	ComunicacionBus();
	/*enviar_mensaje la usamos para enviar a los socios, y recibir_mensaje
	 * para recibir mensajes de las puertas.*/
	bool recibir_mensaje_gim( void * msg, int msg_size, long msgtype );
	bool enviar_mensaje_socio(const void * msg, int msg_size);
	bool recibir_mensaje_sala_entrada( void * msg, int msg_size, long msgtype );

private:
	int envio_socio_qid;
	int recep_gim_qid;
	int recep_sala_entrada_qid;

	bool inicializarComunicacion();
};

