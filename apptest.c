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
	
	opt longopt = {
		short_name: 'l',
		long_name: "long",
		type: OPT_FLAG,
		val: &long_flag,
		description: NULL
	}, intopt = {
		short_name: 'v',
		long_name: "verbosity",
		type: OPT_INT,
		val: &verbose,
		description: "level (sets the verbosity level)"
	};
	bool flag_a, flag_b, flag_c, flag_d, flag_e;
	flag_a = flag_b = flag_c = flag_d = flag_e = false;

	app * theapp = app_new();
	app_opt_add_help(theapp);
	app_opt_on_error(theapp, app_opt_error_handler);
	app_opt_add_short(theapp, 'a', OPT_FLAG, &flag_a);
	app_opt_add_short(theapp, 'b', OPT_FLAG, &flag_b);
	app_opt_add_short(theapp, 'c', OPT_FLAG, &flag_c);
	app_opt_add_short(theapp, 'd', OPT_FLAG, &flag_d);
	app_opt_add_short(theapp, 'e', OPT_FLAG, &flag_e);
	app_opt_add(theapp, &longopt);
	app_opt_add(theapp, &intopt);
	app_opt_add_short(theapp, 's', OPT_STRING, &str);
	app_opt_add_short(theapp, 'p', OPT_PASSWD, &pass);
	app_set_description(theapp, "A testing app for libapp");
	
	res = app_parse_opts(theapp, argc, argv);
	if(!res) {
		printf("doh!\n");
		exit(1);
	}
	if(!str) {
		fprintf(stderr,"Option '-s' is required!\n");
		app_help(theapp, NULL);
		exit(1);
	}
	
	if(stat(str, &st)<0) {
		fprintf(stderr, "ERROR: Can't stat file %s\n", str);
	} else {
		config = fopen(str, "r");
		res = app_parse_opts_from(theapp, config);
		fclose(config);
		if(!res) {
			printf("doh!\n");
			exit(1);
		}
	}
	
	printf("options: %d, %d, %d, %d, %d\n", 
		flag_a, flag_b, flag_c, flag_d, flag_e);
	printf("verbosity: %d\n", verbose);
	if(!pass) pass = app_term_askpass("please insert the password: ");
	
	app_daemonize();
	//from now on, we're in bg
	printf("are in connected to stdout?\n");
}
