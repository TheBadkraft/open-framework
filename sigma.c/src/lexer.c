#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "token.h"

const size_t LEX_ENTRIES = 8;
//                                     ...
const char SPACE = ' ';
const char NEWLINE = '\n';
const char SMB_OPN_BRACE = '{';		// 2:'{'!'}'
const char SMB_CLS_BRACE = '}'; 	// ...
const char SMB_OPN_BRACK = '['; 	// 2:'['!']'
const char SMB_CLS_BRACK = ']'; 	//
const char SMB_OPN_PAREN = '('; 	// 2:'('!')'
const char SMB_CLS_PAREN = ')'; 	// ...
const char SMB_TERMINAL = ';';  	// 2:';'

const char *KWD_INT = "int";       	// 0:['int', 'byte', 'short', ...]
const char *KWD_RETURN = "return"; 	// 0:['return', 'void', 'while', ...]

const char *IDENTIFIER = "0:*";  	// * = any letters
const char *INT_LITERAL = "1:*"; 	// * = any digit

char **LEXTYPES = (char*[] ) { "UNKNOWN", "KEYWORD", "NAMED", "OPERATOR",
				"MATCHED", "PAIRED", "TERMINAL", "LITERAL",
				NULL };

static lexicon *LEXICON = NULL;

char **lex_unknown = (char*[] ) { NULL };
char **lex_keyword = (char*[] ) {
	"int", "return", NULL
};
char **lex_named = (char*[] ) { NULL };
char **lex_operator = (char*[] ) { NULL };
char **lex_matched = (char*[] ) { "'", "\"", NULL };
char **lex_paired = (char*[] ) { "(", ")", "{", "}", "[", "]", NULL };
char **lex_terminal = (char*[] ) { ";", NULL };
char **lex_literal = (char*[] ) { NULL };

static int reg_paired[3];

static bool is_whitespace(char *c) {
	return c[0] == SPACE;
}
static bool is(char *c, char cmp) {
	return c[0] == cmp;
}
static bool is_paired(char *c) {
	return is(c, SMB_OPN_PAREN) || is(c, SMB_CLS_PAREN) || is(c, SMB_OPN_BRACE)
			|| is(c, SMB_CLS_BRACE) || is(c, SMB_OPN_BRACK)
			|| is(c, SMB_CLS_BRACK);
}
static void resolve_pair(char *c) {
	static size_t count = 3;
	static char reg_pair_inc[] = { SMB_OPN_PAREN, SMB_OPN_BRACE, SMB_OPN_BRACK };
	static char reg_pair_dec[] = { SMB_CLS_PAREN, SMB_CLS_BRACE, SMB_CLS_BRACK };

	size_t ndx = 0;
	while (ndx < count) {
		if (c[0] == reg_pair_inc[ndx]) {
			reg_paired[ndx]++;
//			printf("resolved pair [%c]: %d\n", c[0], reg_paired[ndx]);
		}
		else if(c[0] == reg_pair_dec[ndx]) {
			reg_paired[ndx]--;
//			printf("resolved pair [%c]: %d\n", c[0], reg_paired[ndx]);
		}

		++ndx;
	}
}
static bool is_symbol(char *c) {
	return is_paired(c) || is(c, SMB_TERMINAL);
}
static bool is_lexical(char *c, enum LexType lexType) {
	char **ptr = LEXICON->catalog[lexType].entry;
	char *p = *ptr;
	bool isLexical = false;

	while (p && !isLexical) {
		isLexical = strcmp(c, p) == 0;
		p = *++ptr;
	}

	return isLexical;
}
static char* get_word(char *ptr, size_t *wrdLen, size_t strLen) {
	char *pPos = ptr;
	char *end;

	while (pPos - ptr < strLen) {
		if (is_whitespace(pPos)) {
			//  (*j)++;
			(*wrdLen) = (pPos - ptr);
			end = pPos - 1;

			break;
		}
		if (is_symbol(pPos)) {
			(*wrdLen) = (pPos - ptr);
			end = pPos - 1;

			break;
		}

		++pPos;
	}

	return end;
}
static void eval_tokens(struct token *pToken) {
	static enum LexType lexTerms[] = {
		KEYWORD, MATCHED, PAIRED, TERMINAL, NONE
	};
	int ndx = 0;

	struct token *pCurrent = pToken;
	while (pCurrent) {
		char *word = Token.word(pCurrent);

		//	iterate lex terms
		while(lexTerms[ndx] != NONE){
			if (is_lexical(word, lexTerms[ndx])) {
				pCurrent->type = lexTerms[ndx];

				if(is_paired(word)) {
					resolve_pair(word);
				}
			}
			++ndx;
		}

		pCurrent = pCurrent->next;
		ndx = 0;
	}
}

void lexer_init() {
	LEXICON = malloc(sizeof(lexicon));
	LEXICON->catalog = malloc(sizeof(lexentry) * LEX_ENTRIES);

	lexentry *lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "UNKNOWN";
	lexEntry->entry = lex_unknown;
	LEXICON->catalog[UNKNOWN] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "KEYWORD";
	lexEntry->entry = lex_keyword;
	LEXICON->catalog[KEYWORD] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "NAMED";
	lexEntry->entry = lex_named;
	LEXICON->catalog[NAMED] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "OPERATOR";
	lexEntry->entry = lex_operator;
	LEXICON->catalog[OPERATOR] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "MATCHED";
	lexEntry->entry = lex_matched;
	LEXICON->catalog[MATCHED] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "PAIRED";
	lexEntry->entry = lex_paired;
	LEXICON->catalog[PAIRED] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "TERMINAL";
	lexEntry->entry = lex_terminal;
	LEXICON->catalog[TERMINAL] = *lexEntry;

	lexEntry = malloc(sizeof(lexentry));
	lexEntry->name = "LITERAL";
	lexEntry->entry = lex_literal;
	LEXICON->catalog[LITERAL] = *lexEntry;
}
void lexer_tokenize(srcdoc *pDoc) {
//     printf("%s\n", pDoc->source);
	char *pSrc;
	char *ptr = pSrc = pDoc->source;
	size_t ndx = 0;
	size_t length = 0;
	size_t srcLen = strlen(pSrc);
	int line = 1;
	char *pWrd;
	struct token *pHead = NULL, *pCurrent = NULL;

	while (ndx < srcLen) {
		if (is_whitespace(ptr)) {
			++ptr;
			++ndx;
			continue;
		} else if (is(ptr, NEWLINE)) {
			++line;
			++ptr;
			++ndx;
			continue;
		} else if (is_symbol(ptr)) {
//    		++ndx;
			pWrd = ptr;
			length = 1;
		} else {
			pWrd = get_word(ptr, &length, srcLen - ndx);
		}
//    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

		if (!pHead) {
			pCurrent = pHead = Token.new(ptr, length);
		} else if (!pCurrent->next) {
			pCurrent = pCurrent->next = Token.new(ptr, length);
		}

		pCurrent->type = UNKNOWN;
		pCurrent->line = line;

		ndx += length;
		ptr = pWrd + 1;
	}

	pDoc->pToken = pHead;

	if (pDoc->pToken) {
		//	2nd pass
		eval_tokens(pDoc->pToken);
	}
}
lexentry lexer_get_lexType(enum LexType lt) {
	return LEXICON->catalog[lt];
}
int lexer_stat(enum LexerStat lexStat) {
	int result = -1;

	switch (lexStat) {
	case PAIRED_PARENS:
	case PAIRED_BRACES:
	case PAIRED_BRACKS:
		result = reg_paired[lexStat];

	break;
	}

	return result;
}

const struct Lexer_T Lexer = {
		.init = &lexer_init,
		.tokenize = &lexer_tokenize,
		.get_lexicon = &lexer_get_lexType,
		.stat = &lexer_stat
};

