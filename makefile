COMPILADOR_CPP = g++
FLAGS_PARA_WARNINGS_GPP = -ansi -Wall -Wextra -Weffc++ -Wno-variadic-macros
DIR_BINARIOS = bin
DIR_CODIGO = src

socio: servidor_ids
	${COMPILADOR_CPP} ${FLAGS_PARA_WARNINGS_GPP} -o ${DIR_BINARIOS}/socio ${DIR_CODIGO}/aplicacion/socio.cpp \
									      ${DIR_CODIGO}/middleware/socio/Socio.cpp \
									      ${DIR_CODIGO}/middleware/socio/ComunicacionSocio.cpp \
									      ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_clnt.c \
									      ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_xdr.c
servidor_ids:
	cd ${DIR_CODIGO}/middleware/servidor_ids; make

clean:
	cd ${DIR_CODIGO}/middleware/servidor_ids; make clean
	rm -f ${DIR_BINARIOS}/*.o socio
