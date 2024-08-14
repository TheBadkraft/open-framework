#include <stdio.h>
#include <stdlib.h>

#include "../../.production/ctypes.h"
#include "../../.production/cstring.h"
#include "../../.production/file.h"
#include "../../.production/tokenizer.h"
#include "../../.production/version.h"

const char *const CSTRING = "cstring";
const char *const CFILE = "file";
const char *const LEXER = "tokenizer";
const char *const CTYPES = "ctypes";
const char *const VERSION = "version";

const char **const COMPONENTS = (const char *[]){
	"cstring", "file", "tokenizer", "ctypes", "version", NULL};

char *lookup_build(int);

#define BUILD(ndx) lookup_build(ndx)

#define EXPAND(name) name
#define IS_DEFINED(name) EXPAND(__##name##_build) ? true : false
#define CASE(ndx, name) \
	case ndx:           \
		return EXPAND(__##name##_build)

int main(int argc, char **argv)
{
	puts("OpenPlatform :: SigP [component builds]");
	puts("=======================================");

	const char **pos = COMPONENTS;
	const char *word = *pos;
	size_t ndx = pos - COMPONENTS;

	char *build = NULL;
	while (word)
	{
		build = BUILD(ndx);
		printf("[%ld] %-10s (%s)\n", ndx, word, build ? build : "----");

		word = *++pos;
		ndx = pos - COMPONENTS;
	}
}

char *lookup_build(int ndx)
{
	switch (ndx)
	{
#ifdef __cstring_build
		CASE(0, cstring);
#endif
#ifdef __file_build
		CASE(1, file);
#endif
#ifdef __tokenizer_build
		CASE(2, tokenizer);
#endif
#ifdef __ctypes_build
		CASE(3, ctypes);
#endif
#ifdef __ctypes_build
		CASE(4, version);
#endif
	default:
		return NULL;
	}
}