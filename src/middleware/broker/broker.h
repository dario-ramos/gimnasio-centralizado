#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define DIRECTORIO "/home/tomas/Documents"
#define N_COLA_ENTRADA 900
#define N_COLA_SALIDA 1400
#define PUERTO_SOCIOS 5000
#define PUERTO_SISTEMA 5001

int tcpoppas(int puerto);
