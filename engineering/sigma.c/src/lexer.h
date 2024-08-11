#ifndef _LEXER_H
#define _LEXER_H

#include "file.h"

typedef struct lexentry_t {
	char *name;
	char **entry;
} lexentry;

typedef struct lexicon_t {
	lexentry *catalog;
} lexicon;

enum LexerStat {
	PAIRED_PARENS = 0,
	PAIRED_BRACES = 1,
	PAIRED_BRACKS = 2
};

extern const struct Lexer_T {
	void (*init)();
	void (*tokenize)(srcdoc*);
	lexentry (*get_lexicon)(enum LexType);
	int (*stat)(enum LexerStat);
} Lexer;

#endif //  _LEXER_H
