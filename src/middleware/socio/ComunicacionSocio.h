#pragma once

class ComunicacionSocio{
public:
	ComunicacionSocio();
	bool enviar_mensaje_puerta(const void * msg, int msg_size);
	bool recibir_mensaje_puerta(void *, int msg_size, long msgtype);
	bool recibir_mensaje_bus(void * msg, int msg_size, long msgtype);
	bool enviar_mensaje_gimnasio(const void * msg, int msg_size);
	bool recibir_mensaje_gimnasio(void *, int msg_size, long msgtype);
private:
//Estado
	int envio_sistema_qid;
	int recep_sistema_qid;
//Comportamiento
	bool inicializarComunicacion();
};

