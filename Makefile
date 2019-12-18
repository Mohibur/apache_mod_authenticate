PROC = lua
PROCESSOR = ${PROC}_mod

MAKE = make
CC = gcc

PROCESS_LIB_PATH =
ADDITIONAL_LIB = 
ADDITIONAL_INC = -I$(PWD)/mod_authenticate/


LIB=lib
CMODE="-std=gnu99"
COMPILE_OPTION = "-Wall -Wextra"

compile: clear
	${MAKE} ${PROCESSOR}

clear:
	

lua_mod: clear
	${MAKE} compile  CC=${CC} CMODE=${CMODE} ADDITIONAL_INC=${ADDITIONAL_INC} ${COMPILE_OPTION} LIB=../${LIB}/ -C $(PWD)/${PROCESSOR}/ 

php_mod:
	${MAKE} CC=${CC} -C $(PWD)/${PROCESSOR}/
