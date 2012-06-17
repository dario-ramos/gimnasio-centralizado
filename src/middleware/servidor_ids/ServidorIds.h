#ifndef SERVIDOR_IDS_H
#define SERVIDOR_IDS_H

#include <stdio.h>

#define ARCHIVO_SOCIOS "/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes/ids_socios.ids"
#define ARCHIVO_PUERTAS "/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes/ids_puertas.ids"
#define ARCHIVO_BUSES "/home/tomas/Documents/Distribuidos/trunk/servidor_ids_clientes/ids_buses.ids"
#define MAX_CANT_CLIENTES 10
#define SERVIDOR_IDS_MAX_LONG_MSJ_ERROR 200

typedef struct ServidorIds{
	FILE* archivoIds;
	int huboError;
	char msjError[SERVIDOR_IDS_MAX_LONG_MSJ_ERROR];
}ServidorIds;

void ServidorIds_Conectar( ServidorIds* pThis , char *archivo);

void ServidorIds_Desconectar( ServidorIds* pThis, char *archivo );

int ServidorIds_PedirId( ServidorIds* pThis );

int ServidorIds_DevolverId( ServidorIds* pThis, int id );

#endif
