#include <stdio.h>
#include <string.h>

#include "../include/lexer.h"
#include "../include/codex.h"

//                                     ...
const char SPACE = ' ';
const char NEWLINE = '\n';
const char SBL_OP_BRACE = '{'; // 2:'{'!'}'
const char SBL_CL_BRACE = '}'; // ...
const char SBL_OP_PAREN = '('; // 2:'('!')'
const char SBL_CL_PAREN = ')'; // ...
const char SBL_TERMINAL = ';'; // 2:';'

const char *KWD_INT = "int";       // 0:['int', 'byte', 'short', ...]
const char *KWD_RETURN = "return"; // 1:['return', 'void', 'while', ...]

const char *IDENTIFIER = "0:*";  // * means any letters
const char *INT_LITERAL = "1:*"; // any digit

/*
int main()
{
    return 2;
}
*/
static bool is_whitespace(char c)
{
    return c == SPACE;
}

static char *get_word(char *ptr, size_t *pos, size_t len)
{
    //  first break on space
    size_t ndx = *pos;
    char *end;

    while (ndx < len)
    {
        if (is_whitespace(ptr[ndx]))
        {
            //  (*j)++;
            (*pos) = ndx;
            end = ptr + ndx;

            break;
        }

        if (ptr[ndx] == SBL_OP_PAREN)
        {
            (*pos) = --ndx;
            end = ptr + ndx;

            break;
        }

        ++ndx;
    }

    return end;
}

void lexer_tokenize(srcdoc *pDoc)
{
    // printf("%s\n", pDoc->source);

    char *pSrc = pDoc->source;
    size_t ndx = 0;
    size_t len = strlen(pSrc);
    char *pWrd;

    while (ndx < len)
    {
        printf("%s\n", pSrc);

        pWrd = get_word(pSrc, &ndx, len - ndx);

        printf("ndx: %ld len: %ld    [w:%c]\n\n", ndx, pWrd - pSrc, pWrd[0]);

        // ++ndx;
        pSrc = ++pWrd;
    }

    printf("\n");
}

const struct Lexer_T Lexer = {
    .tokenize = &lexer_tokenize};