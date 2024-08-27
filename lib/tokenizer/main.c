#include <stdio.h>

#include "token.h"
#include "tokenizer.h"

token *ebnf_tokenize(document *);

int main(int argc, char *argv[])
{
	printf("building the Sigma.C generic lexer ...\n");
	char *fPath = "./src/.data/sigmac.def";

	// printf("load file '%s' ...\n", fPath);
	document *doc = Document.loadp(fPath);
	// printf("document:\n  source: %s\n  size: %ld\n", doc->pFile->name, String.length(doc->pContent));
	// printf("%s\n", doc->pContent->buffer);

	tokenizer *pTknzr = Tokenizer.init("Generic", NULL);
	token *defTkn = pTknzr->tokenize(doc);
	char *word = Token.word(defTkn);
	printf("token:\n%s\n", word);

	pTknzr = Tokenizer.init("EBNF", ebnf_tokenize);
	token *lexTkn = pTknzr->tokenize(doc);
	while (lexTkn)
	{
		printf("tkn: %s\n", Token.word(lexTkn));
		lexTkn = lexTkn->next;
	}
	printf("*** end tokens ***\n");
}

const char *PATH = "/.data";
const char *DEF_FILE = "sigmac.def";
const char *PATH_FORMAT = "%s/%s";

const char SPACE = ' ';
const char NEWLINE = '\n';
const char PCT_ENDLINE = ';';

const char *ASSIGNMENT = ":=";

static bool is(char *c, char cmp)
{
	return c[0] == cmp;
}
static bool is_noise(char *c)
{
	return is(c, SPACE) || is(c, NEWLINE);
}
static char *get_word(char *ptr, size_t *wrdLen, size_t strLen)
{
	char *pPos = ptr;
	char *end;

	while (pPos - ptr < strLen)
	{
		if (is(pPos, SPACE))
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
token *ebnf_tokenize(document *pDoc)
{
	char *pSrc;
	char *ptr = pSrc = pDoc->pContent->buffer;
	size_t ndx = 0;
	size_t length = 0;
	size_t srcLen = strlen(pSrc);
	int line = 1;
	char *pWrd;
	token *pHead = NULL, *pCurrent = NULL;

	while (ndx < srcLen)
	{
		if (is_noise(ptr))
		{
			++ptr;
			++ndx;
			continue;
		}
		else if (is(ptr, PCT_ENDLINE))
		{
			pWrd = ptr;
			length = 1;
		}
		else
		{
			pWrd = get_word(ptr, &length, srcLen - ndx);
		}
		//    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

		if (!pHead)
		{
			pCurrent = pHead = Token.new(ptr, length);
		}
		else if (!pCurrent->next)
		{
			pCurrent = pCurrent->next = Token.new(ptr, length);
		}

		ndx += length;
		ptr = pWrd + 1;
	}

	return pHead;
}
