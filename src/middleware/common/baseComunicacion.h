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
#define COLA_ENTRADA_SOCIOS 4
#define SALA_ENTRADA 5
#define SALA_SALIDA 6
#define COLA_ENTRADA_GIMNACIO 7
#define SEM_BUS 9


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
	int nroPuerta;
} MsjSocio;

typedef struct {
	long int tipo;
	int idSocio;
	int codigoRespuesta; //0 = entre tranquilo, 1 = No puede entrar.
} MsjRespPuerta;

typedef struct {
	int cantidad;
} ShmCantidadSocios;

typedef struct {
	int entrada;
	int saldia;
} ShmBus;


