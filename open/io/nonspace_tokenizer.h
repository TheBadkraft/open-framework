#ifndef _NONSPACE_INDEXER_H
#define _NONSPACE_INDEXER_H

#include "../core/types.h"
#include "indexer.h"

const string EXPANDS_TO = ":=";

bool index_nonspace(document pDoc, token *pTknHead)
{
    string pSrc;
    char *ndx = pSrc = pDoc->content;
    size_t pos = ndx - pSrc;
    size_t length = 0;
    size_t srcLen = String.length(pSrc);
    int line = 1;
    char *wrdEnd;
    (*pTknHead) = NULL;
    token pCurrent = NULL;

    while (pos < srcLen)
    {
        if (is(*ndx, SPACE))
        {
            ++ndx;
            ++pos;
            continue;
        }
        else if (is(*ndx, NEWLINE))
        {
            ++line;
            wrdEnd = ndx;
            length = 1;
        }
        else
        {
            //  pass NULL to delims param to default SPACE word delimeter
            wrdEnd = get_word(ndx, " \n\0", srcLen - pos);
            length = wrdEnd - ndx + 1;
        }
        //    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

        if (!(*pTknHead))
        {
            pCurrent = (*pTknHead) = Token.new(ndx, length);
        }
        else if (!pCurrent->next)
        {
            pCurrent = pCurrent->next = Token.new(ndx, length);
        }

        pos += length;
        ndx = ++wrdEnd;
    }

    return (*pTknHead) != NULL;
}

#endif //  _NONSPACE_INDEXER_H
