COMPILADOR_CPP = g++
FLAGS_PARA_WARNINGS_GPP = -ansi -Wall -Wextra -Weffc++ -Wno-variadic-macros
DIR_BINARIOS = bin
DIR_CODIGO = src

socio: servidor_ids puerta bus
	${COMPILADOR_CPP} ${FLAGS_PARA_WARNINGS_GPP} -o ${DIR_BINARIOS}/socio ${DIR_CODIGO}/aplicacion/socio.cpp \
									      ${DIR_CODIGO}/middleware/socio/Socio.cpp \
									      ${DIR_CODIGO}/middleware/socio/ComunicacionSocio.cpp \
									      ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_clnt.c \
									      ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_xdr.c
servidor_ids:
	cd ${DIR_CODIGO}/middleware/servidor_ids; make

puerta:
	${COMPILADOR_CPP} ${FLAGS_PARA_WARNINGS_GPP} -o ${DIR_BINARIOS}/puerta ${DIR_CODIGO}/aplicacion/puerta.cpp \
									       ${DIR_CODIGO}/middleware/puerta/Puerta.cpp \
									       ${DIR_CODIGO}/middleware/puerta/ComunicacionPuerta.cpp \
									       ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_clnt.c \
									       ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_xdr.c

bus:
	${COMPILADOR_CPP} ${FLAGS_PARA_WARNINGS_GPP} -o ${DIR_BINARIOS}/puerta ${DIR_CODIGO}/aplicacion/bus.cpp \
									       ${DIR_CODIGO}/middleware/bus/Bus.cpp \
									       ${DIR_CODIGO}/middleware/bus/ComunicacionBus.cpp \
									       ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_clnt.c \
									       ${DIR_CODIGO}/middleware/servidor_ids/servidor_ids_xdr.c

clean:
	cd ${DIR_CODIGO}/middleware/servidor_ids; make clean
	rm -f ${DIR_BINARIOS}/*.o socio
