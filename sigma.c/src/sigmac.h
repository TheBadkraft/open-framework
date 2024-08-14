/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.h
 */

#ifndef _SIGMAC_H_
#define _SIGMAC_H_

#include "version.h"


enum sc_opt_type {
	SC_OPT_SOURCE = 0
};

/*
 * sc_opt_param: parameter container for command line args
 * examples:
 * 1.	file parameter for source option				/dir/file.C
 * 2.	log dir parameter for log (-l) option			-l:../logs/
 */
struct sc_opt_param {
	enum sc_opt_type type;
	char *params;

	//	sc_opt_type will tell us which member of each union to access
	//	TODO: io_type_properties (???)
};
typedef bool(*config)(struct sc_opt_param*);


/*
 * sc_opt: command line argument structure
 */
struct sc_opt {
	//	single character switch option; ex: -<h>
	char key;
	//	word option; ex: --<help>
	char *tag;
	//	delegate to configure the option
	config configure;
	//	description (for help menu)
	char *descr;
	//	parameter if required by the option
	struct sc_opt_param *param;
};

struct sigc {
	const char *name;
	version *ver;
	int cfgc;
	void (*configure)(char**);
	struct sc_opt *options;
	bool (*execute)(void);
	/*
	 * TODO:
	 *	add exit mode - SILENT, WHISPER, SHOUT
	 */
};

struct sc_conf {
	config configure;
	struct sc_opt_param *param;
};

typedef struct sigc sigC;
typedef struct sc_opt sigC_option;
typedef struct sc_opt_param sigC_param;
typedef struct sc_conf sigC_config;

extern const struct sigmac {
	sigC *(*instance)();
} SC;
#endif /* _SIGMAC_H_ */
