#include <stdio.h>
#include <string.h>
#include "../src/lexer.h"
#include "../src/parser.h"
#include "../src/ast.h"

static void exprToString(Expr* expr, char* buf, int* offset, int maxLen) {
    if (expr == NULL || *offset >= maxLen - 1) return;
    if (expr->type == EXPR_NUMBER) {
        int len = expr->as.number.length;
        if (*offset + len < maxLen) {
            memcpy(buf + *offset, expr->as.number.start, len);
            *offset += len;
        }
    } else if (expr->type == EXPR_VARIABLE) {
        int len = expr->as.variable.length;
        if (*offset + len < maxLen) {
            memcpy(buf + *offset, expr->as.variable.start, len);
            *offset += len;
        }
    } else if (expr->type == EXPR_BINARY) {
        if (*offset + 1 < maxLen) buf[(*offset)++] = '(';
        int opLen = expr->as.binary.operator.length;
        if (*offset + opLen < maxLen) {
            memcpy(buf + *offset, expr->as.binary.operator.start, opLen);
            *offset += opLen;
        }
        if (*offset + 1 < maxLen) buf[(*offset)++] = ' ';
        exprToString(expr->as.binary.left, buf, offset, maxLen);
        if (*offset + 1 < maxLen) buf[(*offset)++] = ' ';
        exprToString(expr->as.binary.right, buf, offset, maxLen);
        if (*offset + 1 < maxLen) buf[(*offset)++] = ')';
    }
    buf[*offset] = '\0';
}

static int runParseTest(const char* source, const char* expectedShape) {
    Parser parser;
    initParser(&parser, source);

    Expr* expr = parse(&parser);

    printf("\nInput:    %s\n", source);
    printf("Expected: %s\n", expectedShape);

    if (parser.hadError || expr == NULL) {
        printf("Actual:   parse failed\n");
        if (expr != NULL) freeExpr(expr);
        return 0;
    }

    char actualShape[1024] = {0};
    int offset = 0;
    exprToString(expr, actualShape, &offset, sizeof(actualShape));

    printf("Actual:   %s\n", actualShape);

    int success = (strcmp(actualShape, expectedShape) == 0);
    if (!success) {
        printf("RESULT:   FAIL (mismatch)\n");
    } else {
        printf("RESULT:   PASS\n");
    }

    freeExpr(expr);
    return success;
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

