#include <stdio.h>

#include "bnflexer.h"

int main(int argc, char *argv[])
{
	printf("building the Sigma.C EBNF parser ...\n");

	//	test EBNF Reader init - ... should have the sgmac.def file
	if (!Reader.init())
	{
		printf("could not find '%s/%s'\n", PATH, DEF_FILE);
	}
	else
	{
		printf("load EBNF Reader ...\n");
	}
}
