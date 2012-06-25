#pragma once

#include <cstdio>	//sprintf
#include <cstring>	//strlen
#include <unistd.h>	//write

#define UPRINTLN( nombre_proceso, buffer, string_formato, ... ) \
	sprintf( buffer, "["nombre_proceso"]: " string_formato "\n", ##__VA_ARGS__ ); \
	write(stdout->_fileno, buffer, strlen(buffer))

