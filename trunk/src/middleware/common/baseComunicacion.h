#pragma once
#define DIRECTORIO "/home/tomas/Documents/Distribuidos/gimnacio/trunk"

#define CAPACIDAD_PREDIO 200
#define CAPACIDAD_BUS 25

#define PUERTO 5000

#define SHM_PUERTAS 1
#define SEM_SHM_PUERTAS 2
#define COLA_ENTRADA_PUERTAS 3 //Por donde reciben las puertas
#define COLA_ENTRADA_SOCIOS 4  //Por donde reciben los socios
#define SALA_ENTRADA 5         //Sala entre puerta de entrada y bus
#define SALA_SALIDA 6          //Sala entre gimnacio y bus
#define COLA_ENTRADA_GIMNACIO 7//Para comunica bus con gimnacio
#define SHM_SALAS 100 		   //Base para memorias compartidas entre puerta-bus-gimancio, habra una para cada puerta
#define SEM_SHM_BUS 200		   //Base para los semaforos que controlan el ingreso a la memoria compartida entre puerta-bus-gimnacio
#define SEM_BUS 300			   //Base para semaforos de buses, uno por cada bus.


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
	int entrando; //1 entrando, 0 saliendo.
} MsjSocio;

typedef struct {
	long int tipo;
	int idSocio;
	int resultado; //1 = exito, 0 = fallido.
	int salidaOentrada; //1 = saliendo, 0 = entrando.
} MsjRespPuerta;

typedef struct {
	int cantidad;
} ShmCantidadSocios;

typedef struct {
	int entrada;
	int salida;
} ShmBus;


