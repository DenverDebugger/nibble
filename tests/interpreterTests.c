#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/ast.h"
#include "../src/interpreter.h"

static void assertDouble(double actual, double expected, const char* exprStr) {
    // Using a small epsilon for floating-point comparison
    double epsilon = 0.000001;
    if (fabs(actual - expected) > epsilon && !(isnan(actual) && isnan(expected)) && !(isinf(actual) && isinf(expected))) {
        fprintf(stderr, "FAIL: %s - Expected %g, got %g\n", exprStr, expected, actual);
        exit(1);
    }
    printf("PASS: %s = %g\n", exprStr, actual);
}

void testBasicEvaluation(void) {
    const struct {
        const char* source;
        double expected;
    } cases[] = {
        {"5", 5.0},
        {"10 + 2", 12.0},
        {"10 - 2", 8.0},
        {"10 * 2", 20.0},
        {"10 / 2", 5.0},
        {"3 + 4 * 2", 11.0},
        {"(3 + 4) * 2", 14.0},
        {"50 - 10 * 3 + 6 / 2", 23.0},
        {"100 / (2 * (3 + 2))", 10.0}
    };

    int numCases = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < numCases; i++) {
        Parser parser;
        initParser(&parser, cases[i].source);
        Expr* expr = parse(&parser);
        
        if (parser.hadError || expr == NULL) {
            fprintf(stderr, "FAIL: Parse error on %s\n", cases[i].source);
            exit(1);
        }

        double result = evalExpr(expr);
        assertDouble(result, cases[i].expected, cases[i].source);
        freeExpr(expr);
    }
}

void testDivisionByZero(void) {
    Parser parser;
    initParser(&parser, "5 / 0");
    Expr* expr = parse(&parser);
    double result = evalExpr(expr);
    if (!isinf(result)) {
        fprintf(stderr, "FAIL: Expected infinity for 5 / 0, got %g\n", result);
        exit(1);
    }
    printf("PASS: 5 / 0 = infinity\n");
    freeExpr(expr);
}

void testInvalidExpressions(void) {
    // Manually construct a variable expression to check interpreter fallback.
    // In our parser, "let" variables aren't evaluated by evalExpr (they produce 0).
    Token varToken;
    varToken.type = TOKEN_IDENTIFIER;
    varToken.start = "x";
    varToken.length = 1;
    varToken.line = 1;

    Expr* expr = newVariableExpr(varToken);
    double result = evalExpr(expr);
    if (result != 0.0) {
        fprintf(stderr, "FAIL: Expected variable expression evaluation to return 0, got %g\n", result);
        exit(1);
    }
    printf("PASS: Variable expression fallback returns 0\n");
    freeExpr(expr);
}

int main(void) {
    printf("Running interpreter tests...\n");
    testBasicEvaluation();
    testDivisionByZero();
    testInvalidExpressions();
    printf("All interpreter tests passed.\n");
    return 0;
}
