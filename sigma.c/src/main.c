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
	puts("Sigma.C");

	char **ndx = argv;
	char *arg = *ndx;

	while(arg) {
		printf("[%d] %s\n", ndx - argv, arg);
		arg = *++ndx;
	}
	return EXIT_SUCCESS;
}
