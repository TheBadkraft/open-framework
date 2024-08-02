#include <stdio.h>
#include <stdlib.h>

#include "../include/file.h"
#include "../include/codex.h"

int main(int argc, char **argv)
{
	printf("OpenPlatform Sigma.C Compiler\n");
	//	sigc [OPTIONS] FILE

	//	requires minimum 2 args - sigc, FILE
	// printf("argc: %d\n", argc);

	Codex.load();
	Codex.glyphs(LETTER);
	Codex.glyphs(DIGIT);

	char **source = (char *[]){
		"a2A", "jkduB", "17h0E", "foo", "FooB1", "2Goo", "\0"};

	// Reader.souce(source);
	// Reader.is_letter();

	/* 	char *filename = NULL;
		int ndx = 0;

		while (ndx < argc)
		{
			// printf("[%d] %s\n", ndx, argv[ndx]);
			++ndx;
			if (ndx == argc)
			{
				filename = argv[ndx - 1];
			}
		}

		file *pFile = NULL;
		if (filename)
		{
			pFile = file_exists(filename);
			// printf("file [%s] exists(%s)\n", pFile->name, pFile->exists ? "true" : "false");
		}
		free(pFile);
	 */
	Codex.destroy();
}