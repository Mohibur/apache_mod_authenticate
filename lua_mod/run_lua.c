#include <lua.h>                                /* Always include this when calling Lua */
#include <lauxlib.h>                            /* Always include this when calling Lua */
#include <lualib.h>                             /* Always include this when calling Lua */

#include <stdlib.h>                             /* For function exit() */
#include <stdio.h>                              /* For input/output */

#include <authenticate_scripts.h>

static int fail_process(FILE *f, int error_no, char *message) {
  if(f == null) return error_no;
  char *msg = NULL;
  if(message !=  NULL) msg = message;

  if(SCRIPT_ERROR_FILE_MISSING == error_no && msg == NULL) {
    msg = "Provided script file is missing";
  } else if(SCRIPT_ERROR_CONNECTION == error_no) {
    msg = "Failed to connect with database, please check your settings.";
  } else if(SCRIPT_ERROR_BAD_COMMAND == error_no) {
    msg = "Provided query failed";
  } else if(SCRIPT_UNWRITTABLE_LOG == error_no) {
    msg = "Provided log file name is not writable";
  } if(SCRIPT_ERROR_UNSPECIFIED == error_no) {
    msg = "Unspecified error";
  }
  fprintf(f, messsage_string, msg);
  return error_no;
}

int exec_external_command (const char *key, const char *script, const char *log_file, const char *uri) {
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
    char *p = malloc(100 + strlen(script));
    spritnf(p, "Provided script file %s is not found.", script);
    int to_ret = fail_process(f, SCRIPT_ERROR_FILE_MISSING, p);
    free(p);
    return to_ret;
  }

  lua_State *L;
  
  L = luaL_newstate();                        /* Create Lua state variable */
  luaL_openlibs(L);                           /* Load Lua libraries */

  if (luaL_loadfile(L, target_script))        /* Load but don't run the Lua script */
    return fail_process(f, L->lua_error(), NULL);                   /* Error out if file can't be read */

  if (lua_pcall(L, 0, 0, 0)) {                  /* Run the loaded Lua script */
    fail_process(f,L->lua_error(), NULL );                          /* Error out if Lua file has an error */
    return 
  }
  lua_close(L);                               /* Clean up, free the Lua state var */
  return 0;                                   /* no error */
}

