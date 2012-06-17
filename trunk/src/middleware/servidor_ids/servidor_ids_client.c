/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "servidor_ids.h"


void
servidor_ids_prog_1(char *host)
{
	CLIENT *clnt;
	retorno  *result_1;
	char *obtener_nuevo_id_socio_1_arg;
	retorno  *result_2;
	int  devolver_id_socio_1_arg;
	retorno  *result_3;
	char *obtener_nuevo_id_puerta_1_arg;
	retorno  *result_4;
	int  devolver_id_puerta_1_arg;
	retorno  *result_5;
	char *obtener_nuevo_id_bus_1_arg;
	retorno  *result_6;
	int  devolver_id_bus_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, SERVIDOR_IDS_PROG, SERVIDOR_IDS_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = obtener_nuevo_id_socio_1((void*)&obtener_nuevo_id_socio_1_arg, clnt);
	if (result_1 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_2 = devolver_id_socio_1(&devolver_id_socio_1_arg, clnt);
	if (result_2 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_3 = obtener_nuevo_id_puerta_1((void*)&obtener_nuevo_id_puerta_1_arg, clnt);
	if (result_3 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_4 = devolver_id_puerta_1(&devolver_id_puerta_1_arg, clnt);
	if (result_4 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_5 = obtener_nuevo_id_bus_1((void*)&obtener_nuevo_id_bus_1_arg, clnt);
	if (result_5 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	result_6 = devolver_id_bus_1(&devolver_id_bus_1_arg, clnt);
	if (result_6 == (retorno *) NULL) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	servidor_ids_prog_1 (host);
exit (0);
}
