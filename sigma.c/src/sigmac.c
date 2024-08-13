/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "sigmac.h"

const char *const CSTRING = "cstring";
const char *const CFILE = "file";
const char *const LEXER = "lexer";
const char *const CTYPES = "ctypes";
const char *const VERSION = "version";

const char **const COMPONENTS = (const char*[] ) {
	"cstring", "file", "lexer", "ctypes", "version", NULL
};

char *const OPT_HELP = "help";
char *const OPT_INFO = "info";
char *const OPT_VERSION = "version";

//	TODO: place into a file for configuration ...???
const size_t OPT_COUNT = 3;
struct sc_opt options[] = {
	//	TODO: add property to enable/disable
	{ '\0', OPT_HELP, NULL, "Display command-line options and usage." },
	{ '\0',	OPT_INFO, NULL, "Full compiler and component version information." },
	{ '\0', OPT_VERSION, NULL, "Simple compiler version" }
};

static sigC *sigc;

const byte maj = 0;
const byte min = 0;
const ushort bld = 1;
const relc rc = ALPHA;
char *label = "tuxedo";

static int sc_parse_args(char**);
static bool sc_execute();

static sigC_option *get_option_by_tag(char*);
static sigC_option *get_option_by_key(char);

static void parse_options();
static bool display_help();
static bool display_info();
static bool display_version();

static void sc_init() {
	sigc = malloc(sizeof(sigC));
	sigc->name = "Sigma.C";
	sigc->ver = Version.new(maj, min, bld, rc, label);
	sigc->parse_args = &sc_parse_args;
	sigc->execute = &sc_execute;

	char *sc_version = Version.to_string(sigc->ver);
	printf("===== %s [%s] =====\n", sigc->name, sc_version);
	free(sc_version);

	parse_options();
}
static int sc_parse_args(char **argv) {
	char **ndx = argv;
	//	skip arg[0]
	char *arg = *++ndx;
	int arg_count = 0;
	sigC_option *opt = NULL;
	//	allow for NULL at end
	sigc->actions = malloc(sizeof(task) * (OPT_COUNT + 1));

	/*
	 * 	Apparently, argv terminates with a NULL ...???
	 */
	while (arg) {
		arg_count = ndx - argv;
		//	need to know whether the arg starts with '--' or '-'
		if (arg[1] == '-' && arg[0] == '-') {
			arg += 2;
			//	find option by tag
			opt = get_option_by_tag(arg);
			//	TODO: some options will terminate any other options
		} else if (arg[0] == '-') {
			//	find option by key
			opt = get_option_by_key(arg[1]);
			//	TODO: some options will terminate any other options
		}
		else {
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			arg_count = 0;

			break;
		}

//		printf("[%ld] %s\n", ndx - argv, arg);

		if(opt) {
//			printf("option valid: %s\n", opt->tag);
			sigc->actions[arg_count - 1] = opt->delegate;
		}
		else {
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			free(sigc->actions);
			sigc->actions = NULL;
			arg_count = 0;

			break;
		}

		arg = *++ndx;
	}

	if(arg_count > 0) {
//		printf("sigc->actions[%d] = NULL\n", arg_count);
		sigc->actions[arg_count] = NULL;
	}

	return arg_count;
}
static bool sc_execute() {
	bool retOk = true;
	task *ptr = sigc->actions;
	task task = *ptr;

	while(task && retOk) {
		retOk = task();
		task = *++ptr;
	}

	return retOk;
}
static sigC_option *get_option_by_tag(char *tag) {
	sigC_option *ndx = options;
	sigC_option opt = *ndx;
	while (ndx - options < OPT_COUNT) {
		if(strcmp(opt.tag, tag) == 0) {
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option *get_option_by_key(char key) {
	sigC_option *ndx = options;
	sigC_option opt = *ndx;
	while (ndx - options < OPT_COUNT) {
		if(opt.key == key) {
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static void parse_options() {
//	printf("parse options:\n");

	sigC_option *ndx = options;
	while (ndx - options < OPT_COUNT) {
//		printf("option: %s\n", opt.tag);
		if (strcmp(OPT_HELP, ndx->tag) == 0) {
			ndx->delegate = &display_help;
		} else if (strcmp(OPT_INFO, ndx->tag) == 0) {
			ndx->delegate = &display_info;
		} else if (strcmp(OPT_VERSION, ndx->tag) == 0) {
			ndx->delegate = &display_version;
		}

		++ndx;
	}
}
static bool display_help() {
	printf("--help\n");

	return true;
}
static bool display_info() {
	printf("--info\n");

	return true;
}
static bool display_version() {
	printf("--version\n");

	return true;
}
//==============================================
sigC* sc_instance() {
	if (!sigc) {
		sc_init();
	}

	return sigc;
}

const struct sigmac SC = { .instance = &sc_instance };
