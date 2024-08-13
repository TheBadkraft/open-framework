/*
 *	 ============================================================================
 *	 Name        : Sigma.C Compiler (main)
 *	 Author      : The Badkraft
 *	 Version     :
 *	 Copyright   : 2024 Badkraft Productions
 *	 Description : Sigma.C Compiler entry
 *	 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "sigmac.h"

int main(int argc, char **argv) {
	int retOk = EXIT_SUCCESS;
	if(argc < 2) {
		goto quit_sigmac;
	}
	sigC *sigc = SC.instance();
//	printf("completed %s initialization ...\n", sigc->name);
	int arg_count = sigc->parse_args(argv);

//	printf("processed (%d) arg%s\n", arg_count, arg_count > 1 ? "s" : "");
	if (arg_count) {
		sigc->execute();
	}
	else {
		retOk = EXIT_FAILURE;
	}

	// this is the first time in 20+ years I've used a label to go to (other than ASM)
	quit_sigmac:
	printf("exiting Sigma.C Compiler (%d) ...\n", retOk);
	return retOk;
}
