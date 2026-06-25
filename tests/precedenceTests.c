#include <stdio.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/ast.h"

static int runParseTest(const char* source, const char* expectedShape) {
    Parser parser;
    initParser(&parser, source);

    Expr* expr = parse(&parser);

    printf("\nInput:    %s\n", source);
    printf("Expected: %s\n", expectedShape);

    if (parser.hadError || expr == NULL) {
        printf("Actual:   parse failed\n");
        freeExpr(expr);
        return 0;
    }

    printf("Actual:   ");
    printExpr(expr);
    printf("\n");

    freeExpr(expr);
    return 1;
}

int main(void) {
    int passed = 0;
    int total = 0;

    /*
      Multiplication should bind tighter than addition.
      Input:    1 + 2 * 3
      Expected: (+ 1 (* 2 3))
    */
    total++;
    passed += runParseTest("1 + 2 * 3", "(+ 1 (* 2 3))");

    /*
      Division should bind tighter than subtraction.
      Input:    10 - 8 / 2
      Expected: (- 10 (/ 8 2))
    */
    total++;
    passed += runParseTest("10 - 8 / 2", "(- 10 (/ 8 2))");

    /*
      Parentheses should override normal precedence.
      Input:    (1 + 2) * 3
      Expected: (* (+ 1 2) 3)
    */
    total++;
    passed += runParseTest("(1 + 2) * 3", "(* (+ 1 2) 3)");

    /*
      Parentheses should also work on the right side.
      Input:    10 / (3 - 1)
      Expected: (/ 10 (- 3 1))
    */
    total++;
    passed += runParseTest("10 / (3 - 1)", "(/ 10 (- 3 1))");

    /*
      Operators at the same precedence level should group left-to-right.
      Input:    10 - 3 - 2
      Expected: (- (- 10 3) 2)
    */
    total++;
    passed += runParseTest("10 - 3 - 2", "(- (- 10 3) 2)");

    /*
      Mixed operators and nested parentheses.
      Input:    (1 + 2) * (3 - 4) / 5
      Expected: (/ (* (+ 1 2) (- 3 4)) 5)
    */
    total++;
    passed += runParseTest("(1 + 2) * (3 - 4) / 5", "(/ (* (+ 1 2) (- 3 4)) 5)");

    printf("\n%d/%d parse tests completed without parser errors.\n", passed, total);
    printf("Compare each Actual AST against its Expected shape above.\n");

    return passed == total ? 0 : 1;
}

