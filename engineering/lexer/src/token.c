#include <stdlib.h>
#include <string.h>

#include "token.h"

/*
 * Create a new token from begin and end char pointers
 */
struct token* new_token(char *pBeg, size_t length) {
	struct token *pTkn = malloc(sizeof(struct token));
	pTkn->pBeg = pBeg;
	pTkn->length = length;
	pTkn->line = 0;
//	pTkn->prev = NULL;
	pTkn->next = NULL;

	return pTkn;
}
char* token_word(struct token *pTkn) {
	char *pWrd = malloc(pTkn->length);
	strncpy(pWrd, pTkn->pBeg, pTkn->length);

	return pWrd;
}

const struct Token_T Token = {
		.new = &new_token,
		.word = &token_word
};
