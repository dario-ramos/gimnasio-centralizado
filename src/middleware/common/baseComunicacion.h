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
#define COLA_ENTRADA_PUERTAS 3 //Por donde reciben las puertas
#define COLA_ENTRADA_SOCIOS 4  //Por donde reciben los socios
#define SALA_ENTRADA 5         //Sala entre puerta de entrada y bus
#define SALA_SALIDA 6          //Sala entre gimnacio y bus
#define COLA_ENTRADA_GIMNACIO 7//Para comunica bus con gimnacio
#define SHM_SALAS 100 		   //Base para memorias compartidas entre puerta-buse-gimancio, habra una para cada puerta
#define SEM_BUS 200			   //Base para semaforos de buses, uno por cada bus.


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


