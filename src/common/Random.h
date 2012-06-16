#pragma once

#include "stdlib.h"

namespace Random{
	int EnteroEnRango( int min, int max ){
		return min + rand() % ( max - min );
	}
};
