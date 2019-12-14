#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include <string.h>
/* Define prototypes of our functions in this module */
static void register_hooks(apr_pool_t *pool);
static int mod_sessionconfirm(request_rec *r);
static const char *validate_path(cmd_parms *cmd, void *cfg, const char *arg);
const char *validate_cookie_name(cmd_parms *cmd, void *cfg, const char *arg);
typedef struct {
    const char path[2000];         /* Some path to...something */
    const char cookie_name[2000]; /* 1 means action A, 2 means action B and so on */
} session_config;

static session_config config;

/* Define our module as an entity and assign a function for registering hooks  */

static const command_rec        configuration_directive[] =
{
    AP_INIT_TAKE1("ValidatePath", validate_path, NULL, RSRC_CONF, "The path"),
    AP_INIT_TAKE1("ValidateCookieName", validate_cookie_name, NULL, RSRC_CONF, "The path to whatever"),
    { NULL }
};

const char *validate_path(cmd_parms *cmd, void *cfg, const char *arg)
{
    strcpy(config.path, arg);
    return NULL;
}

const char *validate_cookie_name(cmd_parms *cmd, void *cfg, const char *arg)
{
    strcpy(config.cookie_name, arg);
    return NULL;
}




module AP_MODULE_DECLARE_DATA   example_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,            // Per-directory configuration handler
    NULL,            // Merge handler for per-directory configurations
    NULL,            // Per-server configuration handler
    NULL,            // Merge handler for per-server configurations
    configuration_directive, //tives we may have for httpd
    register_hooks   // Our hook registering function
};


/* register_hooks: Adds a hook to the httpd process */
static void register_hooks(apr_pool_t *pool) 
{
    /* Hook the request handler */
    ap_hook_handler(mod_sessionconfirm, NULL, NULL, APR_HOOK_LAST);
}

/* The handler function for our module.
 * This is where all the fun happens!
 */

static int mod_sessionconfirm(request_rec *r) {
    /* First off, we need to check if this is a call for the "example" handler.
     * If it is, we accept it and do our things, it not, we simply return DECLINED,
     * and Apache will try somewhere else.
     */
    if (!r->handler || strcmp(r->handler, "confirm-handler")) return (DECLINED);
//    ap_set_content_type(r, "text/html");    
    // The first thing we will do is write a simple "Hello, world!" back to the client.
    const char *cookie = NULL;
    ap_cookie_read(r, config.cookie_name, &cookie, 0);
    ap_rprintf(r, "%s<br>", config.cookie_name);
    ap_rprintf(r, "%s<br>", cookie);
    ap_rprintf(r, "%s<br>", config.path);
    ap_rprintf(r, "%s<br>", r->uri);
    if(strcmp(r->uri, config.path) == 0 && strcmp(config.cookie_name, "drama")==0) {
      return OK;
    }
    return OK;
}

