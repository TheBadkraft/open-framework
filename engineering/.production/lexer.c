#include <stdio.h>

#include "lexer.h"

#ifdef __lexer_build
#ifndef __lexer
#define __lexer __lexer_build
#endif
#endif

token *default_tokenizer(document *);

static tokenizer tokenizer_handler = default_tokenizer;

token *default_tokenizer(document *pDoc)
{
    printf("[default tokenizer]\n");

    token *pTkn = malloc(sizeof(token));
    pTkn->pBeg = pDoc->pContent->buffer;
    pTkn->length = String.length(pDoc->pContent);

    return pTkn;
}
//  =========================================================

lexer *lexer_init(char *name, tokenizer delegate)
{
    printf("Sig.C Lexer [%s]... initializing\n", name);
    lexer *pLexer = malloc(sizeof(lexer));

    if (!pLexer)
    {
        //  throwErr()
        printf("Error initializing Lexer ... OOPS!\n");
        return NULL;
    }

    //  configure
    pLexer->name = name;
    if (delegate)
    {
        pLexer->tokenize = delegate;
    }
    else
    {
        pLexer->tokenize = tokenizer_handler;
    }

    return pLexer;
}
token *lexer_tokenize(document *pDoc)
{
    return tokenizer_handler(pDoc);
}

const struct Lexer_T Lexer = {
    .init = &lexer_init,
    .tokenize = &lexer_tokenize};
