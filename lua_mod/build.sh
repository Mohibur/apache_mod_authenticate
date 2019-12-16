gcc -c -std=gnu99 -O2 -Wall -Wextra  -llua -ldl -lm -o cmd-processor.so run_lua.c $@
