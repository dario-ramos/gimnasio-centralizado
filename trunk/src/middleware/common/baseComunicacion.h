#pragma once
#define DIRECTORIO "/home/tomas/Documents/Distribuidos/gimnasio/trunk"

#define CAPACIDAD_PREDIO 200
#define CAPACIDAD_BUS 25

#define CANT_BUSES 3

#define SHM_PUERTAS 1
#define SEM_SHM_PUERTAS 2
#define COLA_ENTRADA_SISTEMA 3 //Por donde reciben las puertas
#define COLA_SALIDA_SISTEMA 4  //Por donde reciben los socios
#define SALA_ENTRADA 5         //Salas de entrada
#define SALA_SALIDA 6          //Sala entre gimnacio y bus
#define SHM_BUS 100 		   //Base para memorias compartidas entre puerta-bus-gimancio, habra una para cada puerta
#define SEM_SHM_BUS 200		   //Base para los semaforos que controlan el ingreso a la memoria compartida entre puerta-bus-gimnacio
#define SEM_BUS 300			   //Base para semaforos de buses, uno por cada bus.

#define BASE_ID_PUERTA 1000
#define BASE_ID_BUS 2000
#define ID_GIMNASIO 3000


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>



typedef enum Operaciones {OPS_ENTRAR_AL_PREDIO = 1, OPS_SUBIR_AL_BUS, OPS_BAJAR_DEL_BUS, OPS_SALIR_DEL_GIMNASIO, OPS_SALIR_DEL_PREDIO} Operaciones;
typedef enum Resultado {RES_EXITO = 1, RES_FALLO} Resultado;
enum Posicion { POS_PUERTA, POS_TRANSITO, POS_GIMNASIO, BUS_ESPERANDO };

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

typedef struct {
	int cantidad;
} ShmCantidadSocios;

typedef struct {
	int entrada;
	int salida;
	int estado;
} ShmBus;


