#ifndef _CODEX_H
#define _CODEX_H

#include "sigctypes.h"

/*
    Codex encapsulates the full set of glyphs, lexicals, symbols, punctuals,
    and grammars for the entire Sigma.C language
*/
struct codex_t;
struct glyph_t;

enum Glyph {
    LETTER = 		0,
    DIGIT = 		1,
    PUNCTUATOR = 	2
};

enum LexType {
	NONE = 			-1,
	UNKNOWN = 		0,
	KEYWORD = 		1,
	IDENTIFIER = 	2,
	OPERATOR = 		3,
//	PARENS,
//	BRACES,
//	BRACKETS,
	MATCHED = 		4,		//	' & "
	PAIRED = 		5,		//	[ & ]  ( & )
	LITERAL = 		6
};

/*
    Codex API
*/
extern const struct Codex_T {
    bool (*load)();
    void (*destroy)();
    void (*info)();
    void (*glyphs)(enum Glyph);
    void (*lexicon)(enum LexType);
} Codex;

#endif //  _CODEX_H
