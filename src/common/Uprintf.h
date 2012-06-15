#pragma once

#include <cstdio>	//sprintf
#include <cstring>	//strlen
#include <unistd.h>	//write
#include <string>	//std::string

#define UPRINTLN( nombre_proceso, buffer, string_formato, ... ) \
	sprintf( buffer, "["nombre_proceso"]: " string_formato "\n", ##__VA_ARGS__ ); \
	write(stdout->_fileno, buffer, strlen(buffer))

inline void revisar_codigo_error( const std::string& nombre_proceso, int codigo, const std::string& msg ){
	if( codigo == -1 ){
		perror( std::string( "[" + nombre_proceso +"]: " + msg).c_str() );
		exit(1);
	}
}
