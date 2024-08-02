#ifndef _CODEX_H
#define _CODEX_H

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
    DIGIT
};
#define cdxglyphs enum Glyph
/*
    Codex API
*/
extern const struct Codex_T
{
    void (*load)();
    void (*destroy)();
    void (*info)();
    void (*glyphs)(cdxglyphs);
} Codex;

#endif //  _CODEX_H