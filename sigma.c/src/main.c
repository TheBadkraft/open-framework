#include <stdio.h>
#include <stdlib.h>

// #include "../include/file.h"
// #include "../include/sigctypes.h"

#include "codex.h"
#include "loader.h"
#include "lexer.h"

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
		else {
			printf("*** ERROR file not found:: %s\n", pFile->name);
		}

		if (pDoc)
		{
			/*	********************************************************
			 * 	The Lexer's first pass will simply generate tokens. It's job is *`not`* to
			 * 	entirely make sense of coding structure or syntax. It will have two tasks
			 * 	to accomplish on it's 2nd pass:
			 * 	1. identify specific, unambiguous terms or keywords: int, bool, if, return
			 * 	2. match enclosing symbols: (..) {..} [..]
			 *	********************************************************/
			Lexer.tokenize(pDoc);
			struct token *pTkn = pDoc->pToken;

			while(pTkn) {
				printf("[%d] [ %-8s ] :: %d\n", pTkn->line, Token.word(pTkn), pTkn->type);
				pTkn = pTkn->next;
			}

			/*	********************************************************
			 * 	Now that we have our linked token list, we can run through the Parser.
			 *
			 *	********************************************************/
		}
	}

	//	clean up
	Loader.destroy();
	Codex.destroy();

	printf("\nSigma.C [exit: 0]\n");
}
