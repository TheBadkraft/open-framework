#include <stdio.h>
#include <stdarg.h>

#include "../include/opio.h"
#include "../include/project.h"
#include "../include/cstring.h"

/*
	Examples:
		op -?|--help							//	output command line options, switches, etc; ignores any other args
		op [--debug]							//	optional does not execute commands; output command line args (if logging, debug output will go to log)
		op [-i|--info][:verbose]				//	output general info regarding version, configuration, etc with optional info
		op -n:ProjectName						//	creates a new project in the current directory
		op -n:... [-d:RelativePath]				//	creates a new project in the relative path
		np -n:... [-l][:RelativePath]			//	creates a log with optional location
*/

int main(int argc, char *argv[])
{
	init(__FILE__, argc, argv);

	if (argc <= 1)
	{
		//	no args are provided; default to -? (help)
		printf("TODO: display CL Options.\n");
		return -1;
	}

	run();
	cleanup();
}
