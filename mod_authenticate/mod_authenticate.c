#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include <string.h>
/* Define prototypes of our functions in this module */
static void register_hooks(apr_pool_t *pool);
static int mod_session_validation(request_rec *r);

static const char *set_script_path(cmd_parms *cmd, void *cfg, const char *arg);
static const char *set_session_cookie_name(cmd_parms *cmd, void *cfg, const char *arg);
static const char *set_processor_name(cmd_parms *cmd, void *cfg, const char *arg);

typedef struct {
  const char *script_path;                   /* Scirpt Path */
  const char *cookie_name;       /* target cookie name */
  const char *script_processor;       /* Pick up which tool we would like to validate with*/
} configuration;

#define VALIDATE_SCRIPT_PATH  "ValidateScriptPath"
#define SESSION_COOKIE_NAME   "SessionCookieName"
#define VALIDATE_PROCESSOR    "ValidateProcessor"

static configuration config;

/* Define our module as an entity and assign a function for registering hooks  */

static const command_rec configuration_directive[] =
{
    AP_INIT_TAKE1(VALIDATE_SCRIPT_PATH, set_script_path, NULL, RSRC_CONF, "The path"),
    AP_INIT_TAKE1(SESSION_COOKIE_NAME,  set_session_cookie_name, NULL, RSRC_CONF, "The path to whatever"),
    AP_INIT_TAKE1(VALIDATE_PROCESSOR,   set_processor_name, NULL, RSRC_CONF, "The path to whatever"),
    { NULL }
};

const char *set_script_path(cmd_parms *cmd, void *cfg, const char *arg) {
  //void *pointer = malloc(strlen(arg)+1);
  //strcpy(pointer, arg);
  config.script_path=arg;
  return NULL;
}
const char *set_session_cookie_name(cmd_parms *cmd, void *cfg, const char *arg) {
  //void *pointer = malloc(strlen(arg)+1);
  //strcpy(pointer, arg);
  config.cookie_name=arg;
  return NULL;
}

const char *set_processor_name(cmd_parms *cmd, void *cfg, const char *arg) {
  //void *pointer = malloc(strlen(arg)+1);
  //strcpy(pointer, arg);
  config.script_processor=arg;
  return NULL;
}



module AP_MODULE_DECLARE_DATA  validating_module =
{
  STANDARD20_MODULE_STUFF,
  NULL,                       // Per-directory configuration handler
  NULL,                       // Merge handler for per-directory configurations
  NULL,                       // Per-server configuration handler
  NULL,                       // Merge handler for per-server configurations
  configuration_directive,    //tives we may have for httpd
  register_hooks              // Our hook registering function
};


/* register_hooks: Adds a hook to the httpd process */
static void register_hooks(apr_pool_t *pool) {
  /* Hook the request handler */
  ap_hook_handler(mod_session_validation, NULL, NULL, APR_HOOK_FIRST);
}

/* The handler function for our module.
 * This is where all the fun happens!
 */

static int mod_session_validation(request_rec *r) {
  /* First off, we need to check if this is a call for the "our target handler" handler.
   * If it is, we accept it and do our things, it not, we simply return DECLINED,
   * and Apache will try somewhere else.
   */
  const char *cookie = NULL;
  ap_cookie_read(r, config.cookie_name, &cookie, 0);
 
#ifdef TESTINGNOW
  if (!r->handler || strcmp(r->handler, "validation-handler")) return (DECLINED);
  ap_set_content_type(r, "text/html");    
  // The first thing we will do is write a simple "Hello, world!" back to the client.
 ap_rprintf(r, "Cookie Value: %s<br>", cookie);
  ap_rprintf(r, "r->uri: %s<br>", r->uri);
  ap_rprintf(r, "script_path: %s<br>", config.script_path);
  ap_rprintf(r, "cookie_name: %s<br>", config.cookie_name);
  ap_rprintf(r, "script_processor: %s<br>", config.script_processor);
#else
  if(cookie == NULL || strcmp(cookie, "") == 0) {
    return HTTP_UNAUTHORIZED;
  }
#endif
  
  return DECLINED;
}

