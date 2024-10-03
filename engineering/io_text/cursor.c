#include <stdlib.h>
#include <string.h>

#include "./open/core/string.h"
#include "./open/io/cursor.h"

cursor cursor_new(char *, size_t);
bool cursor_word(cursor, string *);
void cursor_dispose(cursor);
bool cursor_equals(cursor, const string);

/*
 * Create a new cursor from begin and length
 */
cursor cursor_new(char *pPos, size_t length)
{
	cursor pToken = Allocator.alloc(sizeof(cursor), UNINITIALIZED);
	pToken->pPos = pPos;
	pToken->length = length;
	//	pToken->prev = NULL;
	pToken->next = NULL;

	return pToken;
}
bool cursor_word(cursor pToken, string *pWord)
{
	if (!(*pWord))
	{
		(*pWord) = calloc(pToken->length + 1, sizeof(char));
	}
	else
	{
		free((*pWord));
		(*pWord) = calloc(pToken->length + 1, sizeof(char));
	}
	strncpy((*pWord), pToken->pPos, pToken->length);
	// String.append((*pWord), "\0");

	return (*pWord) != NULL;
}
void cursor_dispose(cursor pToken)
{
	cursor pTemp;
	while (pToken)
	{
		pTemp = pToken;
		pToken = pToken->next;
		free(pTemp);
	}
}
bool cursor_equals(cursor pCursor, const string word)
{
	bool retEqu = pCursor->length == strlen(word);
	char *pL = pCursor->pPos;
	char *pR = word;

	while ((pL - pCursor->pPos < pCursor->length) && (retEqu = pL == pR))
	{
		++pL;
		++pR;
	}

	return retEqu;
}

const struct ICursor Cursor = {
	.new = &cursor_new,
	.word = &cursor_word,
	.dispose = &cursor_dispose,
	.equals = &cursor_equals,
};
