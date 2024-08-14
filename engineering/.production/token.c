#include <stdlib.h>
#include <string.h>

#include "token.h"

#ifdef __token_build
#ifndef __token
#define __token __token_build
#endif
#endif

document *load_doc_from_file(file *);
//	======================================

document *load_doc_from_path(char *pPath)
{
	file *pFile = File.new(pPath);

	return load_doc_from_file(pFile);
}
document *load_doc_from_file(file *pFile)
{
	document *pDoc = NULL;

	pFile->mode = READ;
	stream *pStream = File.open(pFile);
	if (pStream)
	{
		pDoc = malloc(sizeof(stream));
		pDoc->pFile = pFile;
		pDoc->pContent = String.new();
		String.capacity(pDoc->pContent, pStream->length + 1);

		size_t ndx = 0;
		while (Stream.read(pStream, pDoc->pContent->buffer + ndx))
		{
			++ndx;
		}
	}

	return pDoc;
}

/*
 * Create a new token from begin and length
 */
token *new_token(char *pBeg, size_t length)
{
	token *pTkn = malloc(sizeof(token));
	pTkn->pBeg = pBeg;
	pTkn->length = length;
	//	pTkn->prev = NULL;
	pTkn->next = NULL;

	return pTkn;
}
char *token_word(token *pTkn)
{
	char *pWrd = malloc(pTkn->length);
	strncpy(pWrd, pTkn->pBeg, pTkn->length);

	return pWrd;
}

const struct Document_T Document = {
	.loadp = &load_doc_from_path,
	.loadf = &load_doc_from_file};

const struct Token_T Token = {
	.new = &new_token,
	.word = &token_word};
