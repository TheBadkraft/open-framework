#ifndef _BNFPARSER_H
#define _BNFPARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "file.h"

const char *PATH = "/.data";
const char *DEF_FILE = "sigmac.def";
const char *PATH_FORMAT = "%s/%s";

const char SPACE = ' ';
const char NEWLINE = '\n';
const char PCT_ENDLINE = ';';

const char *ASSIGNMENT = ":=";

struct Reader_T
{
    bool (*init)();
};

char **NON_TERMINALS = (char *[]){
    "<program>", "<function>", "<id>", "<statement>", "<expr>", "<int>"};

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
        // if (is_symbol(pPos))
        // {
        //     (*wrdLen) = (pPos - ptr);
        //     end = pPos - 1;

        //     break;
        // }

        ++pPos;
    }

    return end;
}

bool lexer_init()
{
    //  get the length of the path we want to create
    size_t len = snprintf(NULL, 0, PATH_FORMAT, PATH, DEF_FILE);
    char fileName[len];
    sprintf(fileName, PATH_FORMAT, PATH, DEF_FILE);

    file *pFile = file_exists(fileName);

    if (pFile)
    {
        document = load_doc(pFile);
    }

    return pFile != NULL;
}
void lexer_tokenize()
{
    //     printf("%s\n", pDoc->source);
    //  count the number of lines (NEWLINE) and size array

    char *pSrc;
    char *ptr = pSrc = document->source;
    size_t ndx = 0;
    size_t length = 0;
    size_t srcLen = strlen(pSrc);
    int line = 1;
    char *pWrd;
    struct token *pHead = NULL, *pCurrent = NULL;

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

        pCurrent->type = unknown;
        pCurrent->line = line;

        ndx += length;
        ptr = pWrd + 1;
    }

    document->pToken = pHead;

    // if (pDoc->pToken)
    // {
    //     //	2nd pass
    //     eval_tokens(pDoc->pToken);
    // }
}

const struct Reader_T Reader = {
    .init = &reader_init};

#endif //  _BNFPARSER_H