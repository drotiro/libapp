#include "app.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <termios.h>

const static int START_LEN = 10;

struct app_t {
	char * program_name;
	char * description;
	opt** options;
	int len, pos;
	app_callback on_error;
};

app * app_new()
{
	app * this = (app*) malloc(sizeof(app));
	memset(this, 0, sizeof(app));
	this->len = START_LEN;
	this->pos=0;
	this->options = (opt**) malloc(START_LEN*sizeof(opt*));
	memset(this->options, 0, START_LEN*sizeof(opt*));
	return this;	
}

void app_free(app *this)
{
	if(!this) return;
	if(this->options) free(this->options);
	free(this);
}

char* as_long_opt(const char * ln)
{
	char * on;
	if(!ln) return strdup("");
	on = malloc(strlen(ln)+3);
	strcpy(on,"--");
	return strcat(on,ln);
}

void opt_display(opt * anopt)
{
	char * lo = as_long_opt(anopt->long_name);
	fprintf(stderr,"-%c %s\t%s\n",
		anopt->short_name,
		as_long_opt(anopt->long_name),
		(anopt->description ? anopt->description : "")
	);
	free(lo);
}

void app_auto_help(app* this, const char* theopt)
{
	int i;
	if(this->description) fprintf(stderr,"%s: %s\n", this->program_name, this->description);
	fprintf(stderr, "Usage: %s <options>\nOptions:\n", this->program_name);
	
	//list available options
	for(i=0; i<this->pos; ++i) opt_display(this->options[i]);
}

void app_make_room_for_opt(app* theapp)
{
	if (theapp->pos >= theapp->len-1) {
		theapp->len*=2;
		theapp->options = realloc(theapp->options, theapp->len * sizeof(opt*));
	}
}

void app_opt_add(app* theapp, opt* theopt)
{
	app_make_room_for_opt(theapp);
	theapp->options[theapp->pos++] = theopt;
}

static opt auto_help_opt = {
	short_name: 'h',
	long_name: "help",
	type: OPT_CALLBACK,
	val: &app_auto_help,
	description: "(show this help message)"
};

void app_opt_add_help(app* this)
{
	app_opt_add(this, &auto_help_opt);
}

void    app_opt_add_short(app* theapp, char optc, opt_type typ, void * v)
{
	opt * shopt = (opt*) malloc(sizeof(opt));
	shopt->short_name = optc;
	shopt->type = typ;
	shopt->val = v;
	shopt->description = NULL;
	app_opt_add (theapp,  shopt);
}

void app_opt_on_error(app* theapp, app_callback error_handler)
{
	theapp->on_error = error_handler;
}

void app_opt_default_error_handler(app* this, const char* theopt)
{
	fprintf(stderr, "Wrong or invalid option '%s'\n\n", theopt);
	app_auto_help(this, theopt);
}

app_callback app_opt_error_handler = &app_opt_default_error_handler;
app_callback app_help = &app_auto_help;

void app_arg_required(app* this, const char * theopt)
{
	fprintf(stderr, "Option '%s' requires an argument\n", theopt);
	this->on_error ? this->on_error(this, theopt) : app_auto_help(this, theopt);
}

void app_wipe(char * opt)
{
	memset(opt, 0, strlen(opt));
}

bool    app_parse_opts(app * theapp, int argc, char* argv[])
{
	int i=1, last_opt=0, pos;
	bool is_short, found;
	opt * curopt;
	app_callback cb;
	
	if(!argv) return false;
	theapp->program_name = basename(strdup(argv[0]));
	while( i < argc ) {
		//go to next opt, if not there
		while( i < argc && argv[i][0] != '-' ) ++i;
		if( i >= argc ) break;
		last_opt = i;
		
		//short or long opt?
		is_short = (argv[i][1]!='-');
		
		//search for opt
		found = false; pos = 0;
		while(pos < theapp->pos && ! found) {
			found = ( is_short ? 
				theapp->options[pos]->short_name == argv[i][1] && !argv[i][2] :
				theapp->options[pos]->long_name && !strcmp(theapp->options[pos]->long_name, argv[i]+2)
			);
			if(!found) ++pos;
		}
		
		//handle opt
		if(!found) {
			if(theapp->on_error) theapp->on_error(theapp, argv[i]);
			return false;
		}
		curopt = theapp->options[pos];
		switch(curopt->type) {
			case OPT_FLAG: 
				*(bool*)curopt->val = true;
				break;
			case OPT_INT:
				if(i==argc-1) {
					app_arg_required(theapp, argv[i]);
					return false;
				}
				*(int*)curopt->val = atoi(argv[++i]);
				break;
			case OPT_STRING:
				if(i==argc-1) {
					app_arg_required(theapp, argv[i]);
					return false;
				}
				*(char**)curopt->val = argv[++i];
				break;
			case OPT_PASSWD:
				if(i==argc-1) {
					app_arg_required(theapp, argv[i]);
					return false;
				}
				*(char**)curopt->val = strdup(argv[++i]);
				app_wipe(argv[i]);
				break;
			case OPT_CALLBACK:
				cb = (app_callback)curopt->val;
				cb(theapp, argv[i]);
				break;
			default:
				break;
		}
		++i;
	}
	return true;
}

void app_term_set_echo(bool enable)
{
  struct termios tio;
  int tty = fileno(stdin); //a better way?

  if(!tcgetattr(tty, &tio)) {
    if (enable) tio.c_lflag |= ECHO;
    else tio.c_lflag &= ~ECHO;

    tcsetattr(tty, TCSANOW, &tio);
  }
}

char * app_term_askpass(const char * what)
{
  char * val = malloc(512);
  printf("%s ",what);
  app_term_set_echo(0);
  scanf("%s",val);
  app_term_set_echo(1);
  printf("\n");
  return val;
}

const char * app_get_program_name(app * theapp)
{
	if(!theapp) return NULL;
	return theapp->program_name;
}