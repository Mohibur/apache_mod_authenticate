PROC = lua
PROCESSOR = ${PROC}_mod

MAKE = make
CC = gcc

PROCESS_LIB_PATH=
ADDITIONAL_LIB=
ADDITIONAL_INC=

COMPILE_OPTION = "-Wall -Wextra"

build: 
	${MAKE} ${PROCESSOR}

lua_mod:
	${MAKE} ${CC} CCMODE="-std=gnu99" ${COMPILE_OPTION} -C $(PWD)/${PROCESSOR}/ 

php_mod:
	${MAKE} ${CC} -C $(PWD)/${PROCESSOR}/
