#include <stdio.h>

#include "../open/core.h"
#include "open/mpc/mpc.h"

int main(int argc, char *argv[])
{
	printf("MPC Labs");

#ifdef mpc_h
	printf(" ... loaded.\n");
#else
	printf(" ... load fail!!!\n");
	goto exit_lab;
#endif

	string input = "(503 + 2.25) * 739";

	mpc_parser_t *Expr = mpc_new("expression");
	mpc_parser_t *Prod = mpc_new("product");
	mpc_parser_t *Value = mpc_new("value");
	mpc_parser_t *Number = mpc_new("number");
	mpc_parser_t *Integer = mpc_new("integer");
	mpc_parser_t *Float = mpc_new("float");
	mpc_parser_t *Maths = mpc_new("maths");
	mpc_parser_t *Word = mpc_new("word");

	mpc_err_t *err = mpca_lang(MPCA_LANG_DEFAULT,
							   " expression : <product> (('+' | '-') <product>)*; "
							   " product    : <value>   (('*' | '/')   <value>)*; "
							   " value      : <number> | '(' <expression> ')';    "
							   " number     : <integer> | <float>;                "
							   " integer    : /[0-9]+/;                           "
							   " float      : /([0-9]*[.])?[0-9]+/;               "
							   " word       : /[a-zA-Z]*/;                        "
							   " maths      : /^/ <expression> /$/;               ",
							   Expr, Prod, Value, Number, Integer, Float, Maths, Word, NULL);

	mpc_result_t r;

	printf("----------------------\ninput: %s\n", input);

	if (mpc_parse("input", input, Maths, &r))
	{
		mpc_ast_print(r.output);
		mpc_ast_delete(r.output);
	}
	else
	{
		mpc_err_print(r.error);
		mpc_err_delete(r.error);
	}

	mpc_cleanup(8, Expr, Prod, Value, Number, Integer, Float, Maths, Word);

	puts("\n");
	return 0;

exit_lab:
	return 1;
}
