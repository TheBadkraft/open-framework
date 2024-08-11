#ifndef _AST_H
#define _AST_H

#include <string.h>

typedef struct Expr_t
{
    enum
    {
        integer_exp,
        string_exp,
        variable_exp,
        binary_exp,
        unary_exp,
        call_exp,
        projection_exp,
        record_exp
    } tag;
    union
    {
        int integerExp;
        char *stringExp;
        char *variableExp;
        struct
        {
            char *operator;
            struct Expr_t *left;
            struct Expr_t *right;
        } binaryExpr;
        struct
        {
            char *operator;
            struct Expr_t *operand;
        } unaryExpr;
        struct
        {
            char *name;
            struct Expr_list *arguments;
        } callExpr;
        struct
        {
            struct Expr_t *record;
            char *attribute;
        } projectionExpr;
        struct rec
        {
            char *attribute;
            struct Expr_t *value;
            struct rec *next;
        } recordExpr;
    } op;
} expr;

extern const struct ExprBuilder_T
{
    expr *(*binaryExpr)(char *, expr *, expr *);
} ExprBuilder;

#endif //  _AST_H