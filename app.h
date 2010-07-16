#ifndef APP_H
#define APP_H

/*
 * libapp: an utility library to simplify
 * common tasks in app development.
 * (C) 2010 - Domenico Rotiroti
 * Licensed under the LGPL 3.0
 */

typedef struct app_t app;

typedef enum {
	OPT_FLAG,
	OPT_STRING,
	OPT_PASSWD,
	OPT_INT,
	OPT_CALLBACK
} opt_type;

typedef enum {
	false = 0,
	true = 1
} bool;

typedef struct {
	char short_name;
	char * long_name;
	char * desc;
	opt_type type;
	bool is_mandatory;
	void * val;
	char * description;
} opt;

typedef void (*app_callback)( app*, const char* );
/* Generic help and error handler functions */
extern app_callback app_opt_error_handler, app_help;

/* Functions */
app*	app_new();
void	app_free(app* theapp);

void	app_opt_add(app* theapp, opt* theopt);
void	app_opt_add_short(app* theapp, char optc, opt_type type, void * val);
void	app_opt_add_help(app* theapp);
void	app_opt_on_error(app* theapp, app_callback error_handler);

bool	app_parse_opts(app * theapp, int argc, char* argv[]);

void	app_term_set_echo(bool enable);
char*	app_term_askpass(const char * prompt);
#endif //APP_H
