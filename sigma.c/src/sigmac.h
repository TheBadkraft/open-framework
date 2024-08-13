/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.h
 */

#ifndef _SIGMAC_H_
#define _SIGMAC_H_

#include "version.h"

typedef bool(*task)();

struct sc_conf {
	const char *name;
	version *ver;
	int (*parse_args)(char**);
	task* actions;
	bool (*execute)();
	/*
	 * TODO:
	 *	add exit mode - SILENT, WHISPER, SHOUT
	 */
};

/*
 * sc_opt: command line argument structure
 */
struct sc_opt {
	//	ex: -<h>
	char key;
	//	ex: --<help>
	char *tag;
	//	delegate: for now no arguments are expected
	bool (*delegate)(void);
	//	description (for help menu)
	char *descr;
};

extern struct sc_opt options[];

typedef struct sc_conf sigC;
typedef struct sc_opt sigC_option;

extern const struct sigmac {
	sigC *(*instance)();
} SC;
#endif /* _SIGMAC_H_ */
