MAKE = make
CC = gcc

ADDITIONAL_LIB =
ADDITIONAL_INC =

build:
	{CC} - -std=gnu99 -O2 -Wall -Wextra  -llua -ldl -lm -o cmd-processor.so run_lua.c $@

