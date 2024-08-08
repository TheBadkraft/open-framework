#ifndef _CODEX_H
#define _CODEX_H

#include "sigctypes.h"

/*
    Codex encapsulates the full set of glyphs, lexicals, symbols, punctuals,
    and grammars for the entire Sigma.C language
*/
struct codex_t;
struct glyph_t;

/*
    Removed SYMBOL as I believe it is better suited in the Lexicon and Grammar
*/

enum Glyph
{
    LETTER,
    DIGIT,
    SYMBOL
};

/*
    Codex API
*/
extern const struct Codex_T
{
    bool (*load)();
    void (*destroy)();
    void (*info)();
    void (*glyphs)(enum Glyph);
} Codex;

#endif //  _CODEX_H