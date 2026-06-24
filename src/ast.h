#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
    EXPR_NUMBER,
    EXPR_VARIABLE,
    EXPR_BINARY
} ExprType;

// forward declaration because the Expr type has Expr types inside it.
typedef struct Expr Expr;

struct Expr {
    ExprType type;

    union {
        Token number;
        Token variable;

        struct {
            Expr* left;
            Token operator;
            Expr* right;
        } binary;
    } as;
};

Expr* newNumberExpr(Token number);
Expr* newVariableExpr(Token variable);
Expr* newBinaryExpr(Expr* left, Token operator, Expr* right);

void printExpr(Expr* expr);
void freeExpr(Expr* expr);

#endif
