#pragma once

class ComunicacionPuerta {
public:
	ComunicacionPuerta();
	~ComunicacionPuerta();
	bool enviar_mensaje_socio(const void * msg, int msg_size);
	bool recibir_mensaje_socio(void *, int msg_size, long msgtype);
	bool enviar_mensaje_bus(const void * msg, int msg_size);

private:
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_bus_qid;

	bool inicializarComunicacion();

	key_t obtenerClaveEnvio();
	key_t obtenerClaveRecepcion();

};

