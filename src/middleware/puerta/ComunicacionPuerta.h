#pragma once

class ComunicacionPuerta {
public:
	ComunicacionPuerta();
	bool enviar_mensaje_socio(const void * msg, int msg_size);
	bool recibir_mensaje_socio(void *, int msg_size, long msgtype);
	bool enviar_mensaje_bus(const void * msg, int msg_size);

private:
//Estado
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_bus_qid;
//Comportamiento
	bool inicializarComunicacion();

};

