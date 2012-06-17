#pragma once
#define DIRECTORIO "/home/tomas/Documents/Distribuidos/gimnacio/trunk"
#define TICKETS 50
#define VENDEDORES 4
#define COMPRAS 3
#define PRECIO 11
#define MONTO_BASE 9

#define PUERTO 5000

#define SHM_PUERTAS 1
#define SEM_SHM_PUERTAS 2
#define COLA_ENTRADA_PUERTAS 3
#define SALA_PUERTA_ENTRADA 4
#define SALA_PUERTA_SALIDA 5
#define SALA_GIMNACIO_ENTRADA 6
#define SALA_GIMNASIO_SALIDA 7


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>

typedef struct {
	long int tipo;
	int idSocio;

} socio;

typedef struct {
	int cantidad;
} cantidadSocios;


