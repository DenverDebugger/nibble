#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/ast.h"
#include "../src/token.h"

void testNewNumberExpr(void) {
    Token numberToken;
    numberToken.type = TOKEN_NUMBER;
    numberToken.start = "42";
    numberToken.length = 2;
    numberToken.line = 1;

    Expr* expr = newNumberExpr(numberToken);
    if (expr == NULL) {
        fprintf(stderr, "FAIL: newNumberExpr returned NULL\n");
        exit(1);
    }

    if (expr->type != EXPR_NUMBER) {
        fprintf(stderr, "FAIL: Expected EXPR_NUMBER, got %d\n", expr->type);
        exit(1);
    }

    if (expr->as.number.type != TOKEN_NUMBER || 
        strncmp(expr->as.number.start, "42", 2) != 0 || 
        expr->as.number.length != 2) {
        fprintf(stderr, "FAIL: Token in number expression not copied correctly\n");
        exit(1);
    }

    freeExpr(expr);
    printf("PASS: testNewNumberExpr\n");
}

void testNewVariableExpr(void) {
    Token varToken;
    varToken.type = TOKEN_IDENTIFIER;
    varToken.start = "myVar";
    varToken.length = 5;
    varToken.line = 1;

    Expr* expr = newVariableExpr(varToken);
    if (expr == NULL) {
        fprintf(stderr, "FAIL: newVariableExpr returned NULL\n");
        exit(1);
    }

    if (expr->type != EXPR_VARIABLE) {
        fprintf(stderr, "FAIL: Expected EXPR_VARIABLE, got %d\n", expr->type);
        exit(1);
    }

    if (expr->as.variable.type != TOKEN_IDENTIFIER || 
        strncmp(expr->as.variable.start, "myVar", 5) != 0 || 
        expr->as.variable.length != 5) {
        fprintf(stderr, "FAIL: Token in variable expression not copied correctly\n");
        exit(1);
    }

    freeExpr(expr);
    printf("PASS: testNewVariableExpr\n");
}

void testNewBinaryExpr(void) {
    Token num1Token = {TOKEN_NUMBER, "1", 1, 1};
    Token num2Token = {TOKEN_NUMBER, "2", 1, 1};
    Token plusToken = {TOKEN_PLUS, "+", 1, 1};

    Expr* left = newNumberExpr(num1Token);
    Expr* right = newNumberExpr(num2Token);
    Expr* binary = newBinaryExpr(left, plusToken, right);

    if (binary == NULL) {
        fprintf(stderr, "FAIL: newBinaryExpr returned NULL\n");
        exit(1);
    }

    if (binary->type != EXPR_BINARY) {
        fprintf(stderr, "FAIL: Expected EXPR_BINARY, got %d\n", binary->type);
        exit(1);
    }

    if (binary->as.binary.left != left || binary->as.binary.right != right) {
        fprintf(stderr, "FAIL: Left or right child pointer mismatch in binary expr\n");
        exit(1);
    }

    if (binary->as.binary.operator.type != TOKEN_PLUS) {
        fprintf(stderr, "FAIL: Operator token type mismatch in binary expr\n");
        exit(1);
    }

    freeExpr(binary);
    printf("PASS: testNewBinaryExpr\n");
}

int main(void) {
    printf("Running AST tests...\n");
    testNewNumberExpr();
    testNewVariableExpr();
    testNewBinaryExpr();
    printf("All AST tests passed.\n");
    return 0;
}
