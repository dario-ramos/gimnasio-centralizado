/*
 * servidor_ids_clientes.x: Entrega el id para un nuevo cliente de forma remota y segura
 */

/* 
 * estructura de la devolucion de resultados de la invocacion a la subrutina en RPC 
 * ESTE TIPO DE ESTRUCTURAS SON OBLIGATORIAS SI HAY QUE DEVOLVER RESULTADOS
 */

union retorno 
	switch (int cod_ret) {
		case 1: int id;
		case 2: char msj_error[800];
		default: void;
	};

program SERVIDOR_IDS_PROG {
	version SERVIDOR_IDS_VERS {
		retorno OBTENER_NUEVO_ID_SOCIO(void) = 1;
		retorno DEVOLVER_ID_SOCIO(int) = 2;
		retorno OBTENER_NUEVO_ID_PUERTA(void) = 3;
		retorno DEVOLVER_ID_PUERTA(int) = 4;
		retorno OBTENER_NUEVO_ID_BUS(void) = 5;
		retorno DEVOLVER_ID_BUS(int) = 6;
	} = 1;
} = 0x20000099;
