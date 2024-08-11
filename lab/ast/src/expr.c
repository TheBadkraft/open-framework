#include "expr.h"

expr *make_binaryExpr(char *operator, expr * left, expr *right)
{
    expr *e = (expr *)malloc(sizeof(expr));
    e->tag = binary_exp;
    e->op.binaryExpr.operator= operator;
    e->op.binaryExpr.left = left;
    e->op.binaryExpr.right = right;

    return e;
};

const struct ExprBuilder_T ExprBuilder =
    {
        .binaryExpr = &make_binaryExpr};