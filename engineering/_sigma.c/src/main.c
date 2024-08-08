#include <stdio.h>
#include <stdlib.h>

// #include "../include/file.h"
// #include "../include/sigctypes.h"

#include "../include/codex.h"
#include "../include/loader.h"
#include "../include/lexer.h"

int main(int argc, char **argv)
{
	printf("OpenPlatform Sigma.C Compiler\n");
	//	sigc [OPTIONS] FILE

	//	requires minimum 2 args - sigc, FILE
	// printf("argc: %d\n", argc);

	char *filename = NULL;
	if (Codex.load())
	{
		//	iterate arguments
		int ndx = 0;

		while (ndx < argc)
		{
			// printf("[%d] %s\n", ndx, argv[ndx]);
			++ndx;

			//	source file expected to be last arg; minimum 2 args
			if (argc >= 2 && ndx == argc)
			{
				filename = argv[ndx - 1];
			}
		}

		//	if we have a file
		file *pFile = NULL;
		if (filename)
		{
			pFile = file_exists(filename);
			// printf("file [%s][%ld] exists(%s)\n", pFile->name, pFile->size, pFile->exists ? "true" : "false");
		}

		srcdoc *pDoc = NULL;
		if (pFile && pFile->exists)
		{
			Loader.init();
			pDoc = Loader.load(pFile);
		}

		if (pDoc)
		{
			Lexer.tokenize(pDoc);
			// printf("\nsource:\n%s\n", pDoc->source);
		}
	}

	//	clean up
	Loader.destroy();
	Codex.destroy();
}