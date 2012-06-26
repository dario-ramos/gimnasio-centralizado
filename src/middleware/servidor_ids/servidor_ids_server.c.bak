#include "servidor_ids.h"
#include "ServidorIds.h"

retorno *
obtener_nuevo_id_socio_1_svc(void *argp, struct svc_req *rqstp)
{
	static retorno result;
	result.cod_ret = 1;
	result.retorno_u.id = 0;
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds , ARCHIVO_SOCIOS);
	int id = ServidorIds_PedirId( &servidorIds );
	printf("Se quiere entregar el id %i\n", id);
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = id;
		printf("Todod joya %i\n", id);
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
		printf("Errorrr %s\n", servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_SOCIOS );
	return &result;

}

retorno *
devolver_id_socio_1_svc(int *id, struct svc_req *rqstp)
{
	static retorno result;

	printf("Se quiere devolver el id %i\n", *id);
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds, ARCHIVO_SOCIOS );
	ServidorIds_DevolverId( &servidorIds, *id );
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = *id;
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_SOCIOS );
	return &result;
}

retorno *
obtener_nuevo_id_puerta_1_svc(void *argp, struct svc_req *rqstp)
{
	static retorno result;
	result.cod_ret = 1;
	result.retorno_u.id = 0;
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds , ARCHIVO_PUERTAS);
	int id = ServidorIds_PedirId( &servidorIds );
	printf("Se quiere entregar el id %i\n", id);
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = id;
		printf("Todod joya %i\n", id);
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
		printf("Errorrr %s\n", servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_PUERTAS );
	return &result;
}

retorno *
devolver_id_puerta_1_svc(int *id, struct svc_req *rqstp)
{
	static retorno result;

	printf("Se quiere devolver el id %i\n", *id);
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds, ARCHIVO_PUERTAS );
	ServidorIds_DevolverId( &servidorIds, *id );
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = *id;
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_PUERTAS );
	return &result;
}

retorno *
obtener_nuevo_id_bus_1_svc(void *argp, struct svc_req *rqstp)
{
	static retorno result;
	result.cod_ret = 1;
	result.retorno_u.id = 0;
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds , ARCHIVO_BUSES);
	int id = ServidorIds_PedirId( &servidorIds );
	printf("Se quiere entregar el id %i\n", id);
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = id;
		printf("Todod joya %i\n", id);
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
		printf("Errorrr %s\n", servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_BUSES );
	return &result;
}

retorno *
devolver_id_bus_1_svc(int *id, struct svc_req *rqstp)
{
	static retorno result;

	printf("Se quiere devolver el id %i\n", *id);
	ServidorIds servidorIds;
	ServidorIds_Conectar( &servidorIds, ARCHIVO_PUERTAS );
	ServidorIds_DevolverId( &servidorIds, *id );
	if( !servidorIds.huboError ){
		result.cod_ret = 1;
		result.retorno_u.id = *id;
	}else{
		result.cod_ret = 2;
		strcpy(result.retorno_u.msj_error, servidorIds.msjError);
	}
	ServidorIds_Desconectar( &servidorIds, ARCHIVO_PUERTAS );
	return &result;

}
