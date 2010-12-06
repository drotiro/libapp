#include "../app.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void main(int argc, char* argv[])
{
	bool long_flag = false, res=false;
	int verbose = 0;
	char *str=NULL, *pass=NULL;
	struct stat st;
	FILE * config;
	
	//app test
	
	bool flag_a, flag_b, flag_c, flag_d;
	flag_a = flag_b = flag_c = flag_d = false;

	opt myopts[] = {
	/*   SHRT, LONG,       TYPE,       DEST,       DESC */
		{ 'l', "long",     OPT_FLAG,   &long_flag, NULL },
		{ 'v', "verbose",  OPT_INT,    &verbose,   "level (sets the verbosity level)"},
		{ 'f', "file",     OPT_STRING, &str, "config_file (reads configuration from file config_file)"},
		{ 'p', "password", OPT_PASSWD, &pass},
		{ 'a', NULL,       OPT_FLAG,   &flag_a},
		{ 'b', NULL,       OPT_FLAG,   &flag_b},
		{ 'c', NULL,       OPT_FLAG,   &flag_c},
		{ 'd', NULL,       OPT_FLAG,   &flag_d},
	};

	app * theapp = app_new();
	app_opt_add_help(theapp);
	app_opt_on_error(theapp, app_help);
	app_opts_add(theapp, myopts, sizeof(myopts)/sizeof(myopts[0]));
	app_set_description(theapp, "A testing app for libapp");
	
	res = app_parse_opts(theapp, &argc, &argv);
	if(!res) exit(1);
	if(argc) {
		printf("remaining arguments: %d (%s%s)\n",argc, argv[0], argc > 1 ? " ... " : "" );
	}

	if(str && stat(str, &st)==0) {
		config = fopen(str, "r");
		res = app_parse_opts_from(theapp, config);
		fclose(config);
		if(!res) exit(1);
	}
	
	printf("options: %d, %d, %d, %d\n", 
		flag_a, flag_b, flag_c, flag_d);
	printf("verbosity: %d\n", verbose);
	printf("%s, %s\n", str, pass);
	if(!pass) pass = app_term_askpass("please insert the password: ");

	app_daemonize();
	//from now on, we're in bg
	printf("are we connected to stdout? (we shouldn't...)\n");
}
