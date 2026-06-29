#include "ast.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Creates a new number expression node.
 * 
 * @param number The token representing the number.
 * @return Expr* A pointer to the newly allocated number expression.
 */
Expr* newNumberExpr(Token number) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_NUMBER;
    expr->as.number = number;
    return expr;
}

/**
 * @brief Creates a new variable expression node.
 * 
 * @param variable The token representing the variable identifier.
 * @return Expr* A pointer to the newly allocated variable expression.
 */
Expr* newVariableExpr(Token variable) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable = variable;
    return expr;
}

/**
 * @brief Creates a new binary expression node.
 * 
 * @param left The left operand expression.
 * @param operator The token representing the binary operator.
 * @param right The right operand expression.
 * @return Expr* A pointer to the newly allocated binary expression.
 */
Expr* newBinaryExpr(Expr* left, Token operator, Expr* right) {
    Expr* expr = malloc(sizeof(Expr));
    expr->type = EXPR_BINARY;
    expr->as.binary.left = left;
    expr->as.binary.operator = operator;
    expr->as.binary.right = right;
    return expr;
}

/**
 * @brief Prints the text content of a token to standard output.
 * 
 * @param token The token to print.
 */
static void printToken(Token token) {
    printf("%.*s", token.length, token.start);
}

/**
 * @brief Recursively prints the structure of an expression to standard output.
 * 
 * @param expr The expression tree to print.
 */
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

/**
 * @brief Recursively frees the memory allocated for an expression tree.
 * 
 * @param expr The expression tree to free.
 */
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
