#pragma once

#include <cstdlib>

namespace Random{
	inline int EnteroEnRango( int min, int max ){
		return min + rand() % ( max - min );
	}
};
