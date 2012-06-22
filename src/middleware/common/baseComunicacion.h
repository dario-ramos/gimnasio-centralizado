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
#define SHM_SALAS 100 		   //Base para memorias compartidas entre puerta-bus-gimancio, habra una para cada puerta
#define SEM_SHM_BUS 200		   //Base para los semaforos que controlan el ingreso a la memoria compartida entre puerta-bus-gimnacio
#define SEM_BUS 300			   //Base para semaforos de buses, uno por cada bus.

#define BASE_ID_PUERTA 1000
#define BASE_ID_BUS 2000
#define ID_GIMNACIO 3000


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

typedef enum Operaciones {ENTRAR_AL_PREDIO = 1, SUBIR_AL_BUS, BAJAR_DEL_BUS, SALIR_DEL_GIMNASIO, SALIR_DEL_PREDIO};
typedef enum Resultado {EXITO = 1, FALLO};

typedef struct {
	long int tipo;
	int idSocio;
	int nroPuerta;
	int operacion;
} MsjSocio;

typedef struct {
	long int tipo;
	int idSocio;
	int codOp; //Codigo de operacion: 1 = entrada al predio; 2 = subir al bus; 3 = bajar del bus; 4 = registrarse sala de salida; 5 salir del predio.
	int codResultado; //Resultado de la operacion. 1 = exito; 0 = operacion fallida.
} MsjRespSocio;

/*typedef struct {
	long int tipo;
	int idSocio;
	int operacion; //1 = subir, 0 = bajar
} MsjBusSocio;*/

typedef struct {
	int cantidad;
} ShmCantidadSocios;

typedef struct {
	int entrada;
	int salida;
} ShmBus;


