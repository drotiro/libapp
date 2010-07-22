#include "app.h"
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
	
	opt myopts[] = {
		{ 'l', "long", OPT_FLAG, val: &long_flag, NULL },
		{ 'v', "verbosity", type: OPT_INT, val: &verbose, "level (sets the verbosity level)"},
		{ 's', NULL, type: OPT_STRING, val: &str, NULL},
		{ 'p', NULL, type: OPT_PASSWD, val: &pass, NULL}
	};

	bool flag_a, flag_b, flag_c, flag_d, flag_e;
	flag_a = flag_b = flag_c = flag_d = flag_e = false;

	app * theapp = app_new();
	app_opt_add_help(theapp);
	app_opt_on_error(theapp, app_help);
	app_opt_add_flag(theapp, 'a', &flag_a);
	app_opt_add_flag(theapp, 'b', &flag_b);
	app_opt_add_flag(theapp, 'c', &flag_c);
	app_opt_add_flag(theapp, 'd', &flag_d);
	app_opt_add_flag(theapp, 'e', &flag_e);
	app_opts_add(theapp, myopts, 4);
	app_set_description(theapp, "A testing app for libapp");
	
	res = app_parse_opts(theapp, argc, argv);

	if(!res) exit(1);
	if(!str) {
		fprintf(stderr,"Option '-s' is required!\n");
		app_help(theapp, NULL);
		exit(1);
	}
	
	if(stat(str, &st)<0) {
		fprintf(stderr, "Can't stat file %s\n", str);
	} else {
		config = fopen(str, "r");
		res = app_parse_opts_from(theapp, config);
		fclose(config);
		if(!res) exit(1);
	}
	
	printf("options: %d, %d, %d, %d, %d\n", 
		flag_a, flag_b, flag_c, flag_d, flag_e);
	printf("verbosity: %d\n", verbose);
	if(!pass) pass = app_term_askpass("please insert the password: ");
	
	app_daemonize();
	//from now on, we're in bg
	printf("are we connected to stdout? (we shouldn't...)\n");
}
