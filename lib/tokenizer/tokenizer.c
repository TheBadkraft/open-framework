#include <stdio.h>

#include "tokenizer.h"

#ifdef __tokenizer_build
#ifndef __tokenizer
#define __tokenizer __tokenizer_build
#endif
#endif

token *default_tokenizer(document *);

static tokenize tokenizer_handler = default_tokenizer;

token *default_tokenizer(document *pDoc)
{
    printf("[default tokenizer]\n");

    token *pTkn = malloc(sizeof(token));
    pTkn->pBeg = pDoc->pContent->buffer;
    pTkn->length = String.length(pDoc->pContent);

    return pTkn;
}
//  =========================================================

tokenizer *tknizer_init(char *name, tokenize delegate)
{
    printf("Sig.C Lexer [%s]... initializing\n", name);
    tokenizer *pTknizer = malloc(sizeof(tokenizer));

    if (!pTknizer)
    {
        //  throwErr()
        printf("Error initializing Lexer ... OOPS!\n");
        return NULL;
    }

    //  configure
    pTknizer->name = name;
    if (delegate)
    {
        pTknizer->tokenize = delegate;
    }
    else
    {
        pTknizer->tokenize = tokenizer_handler;
    }

    return pTknizer;
}
token *tknizer_tokenize(document *pDoc)
{
    return tokenizer_handler(pDoc);
}

const struct OP_Tokenizer Tokenizer = {
    .init = &tknizer_init,
    .tokenize = &tknizer_tokenize};
