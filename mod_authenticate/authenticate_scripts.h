#ifdef __scripts_h__
#define __scripts_h__


// ERROR messages
#define SCRIPT_SUCCESS_VALIDATE                 0x000
#define SCRIPT_FAILED_VALIDATE                  0x001
#define SCRIPT_ERROR_FILE_MISSING               0x002
#define SCRIPT_ERROR_CONNECTION                 0x003
#define SCRIPT_ERROR_BAD_COMMAND                0x004
#define SCRIPT_UNWRITTABLE_LOG                  0x005
#define SCRIPT_ERROR_UNSPECIFIED                0x500

// COMMON FUNCTIONS
/*
  * key: if empty, will return failed: Expected to be the key that needed to be retrieved from your source
  * script: target script absolute path. If not valid path or file does not exists, apache will failed to start
  * log: if not properly written 
  */
int exec_external_command (const char *key, const char *script, const char *log_file, const char *uri);

#endif
