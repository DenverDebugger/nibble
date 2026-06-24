#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

Expr* newNumberExpr(Token number) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_NUMBER;
    expr->as.number = number;
    return expr;
}

Expr* newVariableExpr(Token variable) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable = variable;
    return expr;
}

Expr* newBinaryExpr(Expr* left, Token operator, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_BINARY;
    expr->as.binary.left = left;
    expr->as.binary.operator = operator;
    expr->as.binary.right = right;
    return expr;
}

static void printToken(Token token) {
    printf("%.*s", token.length, token.start);
}

void printExpr(Expr* expr) {
    if (expr == NULL) {
        return;
    }

    switch (expr->type) {
        case EXPR_NUMBER:
            printToken(expr->as.number);
            break;

       case EXPR_VARIABLE:
            printToken(expr->as.variable);
            break;

       case EXPR_BINARY:
            printf("(");
            printToken(expr->as.binary.operator);
            printf(" ");
            printExpr(expr->as.binary.left);
            printf(" ");
            printExpr(expr->as.binary.right);
            printf(")");
            break;
    }
}

void freeExpr(Expr* expr) {
    if (expr == NULL) {
        return;
    }

    switch (expr->type) {
        case EXPR_NUMBER:
            break;

       case EXPR_VARIABLE:
            break;

       case EXPR_BINARY:
            freeExpr(expr->as.binary.left);
            freeExpr(expr->as.binary.right);
            break;
    }
    free(expr);
}
