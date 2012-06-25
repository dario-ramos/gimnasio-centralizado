#pragma once

class ComunicacionGimnasio {
public:
	ComunicacionGimnasio();

	bool recibir_mensaje_socio(void * msg, unsigned int msg_size, long int id);
	bool enviar_mensaje_socio(const void * msg, unsigned int msg_size);
	bool enviar_mensaje_sala(const void * msg, unsigned int msg_size);

private:
//Estado
	int envio_socio_qid;
	int recep_socio_qid;
	int envio_sala_qid;
//Comportamiento
	bool inicializarComunicacion();
};

