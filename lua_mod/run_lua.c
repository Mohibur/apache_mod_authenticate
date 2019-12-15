#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */

#include <stdlib.h>                             /* For function exit() */
#include <stdio.h>                              /* For input/output */

#include <authenticate_scripts.h>

script int fail_process(FILE *f, int error_no, char *script, char *key) {
  if(f == null) return error_no;
  char *message_string = "Process failed. Reason being: %s\n";
  if(SCRIPT_ERROR_FILE_MISSING == error_no) {
    // assuming the message below is no longer than 100; for now
    char *msg = malloc(100 + strlen(script));
    sprintf(msg, "Lua Execution Scirpt %s is missing", script);
    fprintf(f, messsage_string, msg);
    free(msg);
  } else if(SCRIPT_ERROR_CONNECTION == error_no) {
    fprintf(f, messsage_string, "Failed to connect with databse, please check your settings.");
  } else if(SCRIPT_ERROR_BAD_COMMAND == error_no) {
    fprintf(f, messsage_string, "Provided query failed");
  } else if(SCRIPT_UNWRITTABLE_LOG == error_no) {
    fprintf(f, messsage_string, "provided log file name is not writable");
  } if(SCRIPT_ERROR_UNSPECIFIED == error_no) {
    fprintf(f, messsage_string, "Unspecified error");
  }
  return error_no;
}

int exec_external_command (char *key, char *script, char *log_file) {
  FILE *f;

  if(log_file != NULL && strcmp(log_file, "") != 0)
    f = NULL;
  else if (access(log_file, F_OK) != -1 ) {
    f = fopen(log_file, "a");
    if(f == NULL) {
      return SCRIPT_UNWRITTABLE_LOG;
    }
  }
  if (access(script, F_OK) == -1) {
    return fail_process(f, SCRIPT_ERROR_FILE_MISSING, script, key);
  }

  lua_State *L;
  
  L = luaL_newstate();                        /* Create Lua state variable */
  luaL_openlibs(L);                           /* Load Lua libraries */

  if (luaL_loadfile(L, target_script))    /* Load but don't run the Lua script */
    fail_process(f--#TODO);      /* Error out if file can't be read */

  printf(f, "In C, calling Lua\n");

  if (lua_pcall(L, 0, 0, 0)) {                  /* Run the loaded Lua script */
    bail(f, L, "lua_pcall() failed");          /* Error out if Lua file has an error */
    return 
  }
  lua_close(L);                               /* Clean up, free the Lua state var */
  return 0;
}

