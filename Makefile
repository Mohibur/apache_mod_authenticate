PROC=lua
PROCESSOR=${PROC}_cmd
build: 
	$(${PROCESSOR})

lua_cmd:
	echo "${PROCESSOR}"

php_cmd:
	echo "${PROCESSOR}"
