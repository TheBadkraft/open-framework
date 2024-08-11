#include <stdio.h>

#include "token.h"
#include "lexer.h"

token *tokenize(document *);

int main(int argc, char *argv[])
{
	printf("building the Sigma.C generic lexer ...\n");
	char *fPath = "./src/.data/sigmac.def";

	// printf("load file '%s' ...\n", fPath);
	document *doc = Document.loadp(fPath);
	// printf("document:\n  source: %s\n  size: %ld\n", doc->pFile->name, String.length(doc->pContent));
	// printf("%s\n", doc->pContent->buffer);

	lexer *pLex = Lexer.init("Generic", NULL);
	token *defTkn = Lexer.tokenize(doc);
	char *word = Token.word(defTkn);
	printf("token:\n%s\n", word);

	pLex = Lexer.init("EBNF", tokenize);
	token *lexTkn = pLex->tokenize(doc);
}

token *tokenize(document *pDoc)
{
}
