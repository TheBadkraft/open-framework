#include <stdio.h>

#include "open/io_stream.h"
#include "../open/indexer.h"

bool ebnf_tokenize(document *, token **);

int main(int argc, char *argv[])
{
	printf("building the Sigma.C indexer ...\n");
	char *fPath = "./.data/sigmac.def";
	stream *pStream;
	document *doc;
	if (Stream.new(File.new(fPath), READ, &pStream))
	{
		// printf("load file '%s' ...\n", fPath);
		if (!Document.load(pStream, &doc))
		{
			printf("error loading doc (%s) from stream (%s)\n", doc->name->buffer, fPath);
		}
	}

	// printf("document:\n  source: %s\n  size: %ld\n", doc->pFile->name, String.length(doc->pContent));
	// printf("%s\n", doc->pContent->buffer);

	indexer *pIndexer;

	if (argc > 1 && strcmp("-g", argv[1]) == 0)
	{
		pIndexer = Indexer.init("Generic", NULL);
	}
	else
	{
		pIndexer = Indexer.init("Sigma.C", ebnf_tokenize);
	}
	token *pToken;
	if (!pIndexer->tokenizer(doc, &pToken))
	{
		printf("error indexing doc\n");
	}
	else
	{
		printf("token:\n");
	}

	char *word = NULL;
	token *ptr = pToken;
	while (ptr)
	{
		Token.word(ptr, &word);
		printf("%s\n", word);

		ptr = ptr->next;
	}
	free(word);
	Token.free(pToken);
}

const char *PATH = "/.data";
const char *DEF_FILE = "sigmac.def";
const char *PATH_FORMAT = "%s/%s";

const char SPACE = ' ';
const char NEWLINE = '\n';
const char PCT_ENDLINE = ';';

const char *ASSIGNMENT = ":=";

static bool is(char c, char cmp)
{
	return c == cmp;
}
static bool is_noise(char *c)
{
	return is(*c, SPACE) || is(*c, NEWLINE);
}
static char *get_word(char *ptr, size_t *wrdLen, size_t strLen)
{
	char *pPos = ptr;
	char *end;

	while (pPos - ptr < strLen)
	{
		if (is(*pPos, SPACE))
		{
			//  (*j)++;
			(*wrdLen) = (pPos - ptr);
			end = pPos - 1;

			break;
		}
		++pPos;
	}

	return end;
}
bool ebnf_tokenize(document *pDoc, token **pTknHead)
{
	char *pSrc;
	char *ptr = pSrc = pDoc->content->buffer;
	size_t ndx = 0;
	size_t length = 0;
	size_t srcLen = strlen(pSrc);
	int line = 1;
	char *pWrd;
	(*pTknHead) = NULL;
	token *pCurrent = NULL;

	while (ndx < srcLen)
	{
		if (is_noise(ptr))
		{
			++ptr;
			++ndx;
			continue;
		}
		else if (is(*ptr, PCT_ENDLINE))
		{
			pWrd = ptr;
			length = 1;
		}
		else
		{
			pWrd = get_word(ptr, &length, srcLen - ndx);
		}
		//    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

		if (!(*pTknHead))
		{
			pCurrent = (*pTknHead) = Token.new(ptr, length);
		}
		else if (!pCurrent->next)
		{
			pCurrent = pCurrent->next = Token.new(ptr, length);
		}

		ndx += length;
		ptr = pWrd + 1;
	}
	return (*pTknHead) != NULL;
}
