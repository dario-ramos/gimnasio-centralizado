/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "servidor_ids_clientes.h"

bool_t
xdr_retorno (XDR *xdrs, retorno *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->cod_ret))
		 return FALSE;
	switch (objp->cod_ret) {
	case 1:
		 if (!xdr_int (xdrs, &objp->retorno_u.id_cliente))
			 return FALSE;
		break;
	case 2:
		 if (!xdr_vector (xdrs, (char *)objp->retorno_u.msj_error, 800,
			sizeof (char), (xdrproc_t) xdr_char))
			 return FALSE;
		break;
	default:
		break;
	}
	return TRUE;
}
