#include "Socio.h"
#include "../puerta/Puerta.h"
#include "../common/Constantes.h"
#include "../common/Uprintf.h"

Socio::Socio(){
	
}

Socio::~Socio(){
	
}

bool Socio::IngresarAlPredio( int puerta ){
	int puerta = Random::EnteroEnRango( 0, Constantes::CANT_PUERTAS - 1 );
	char printBuffer[200];
	UPRINTLN( "Socio", printBuffer, "Socio %d decidio entrar por puerta %d", id, puerta ); //TODO <NIM> Imprimir id de socio
}

void Socio::TomarBusDeSalaEntradaAGimnasio(){
	throw std::runtime_error( "Not implemented" );
}

void Socio::Ejercitar(){
	throw std::runtime_error( "Not implemented" );
}

void Socio::TomarBusDeGimnasioASalaSalida(){
	throw std::runtime_error( "Not implemented" );
}

void Socio::SalirDelPredio(){
	throw std::runtime_error( "Not implemented" );
}

bool Socio::pedirId() {
	
}

bool Socio::devolverId() {
	
}
