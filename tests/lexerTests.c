#include <stdio.h>
#include <stdlib.h>
#include "../src/lexer.h"
#include "../src/lexer.c"

void assertToken(Token token, TokenType expected) {
    if (token.type != expected) {
        printf("Expected %d, got %d\n", expected, token.type);
    exit(1);
    }
}

/*
Input:

    5 + 2

Expected Tokens:

    TOKEN_NUMBER
    TOKEN_PLUS
    TOKEN_NUMBER
    TOKEN_EOF
*/
void testArithmetic(void) {
    Lexer lexer;
    initLexer(&lexer, "5 + 2");

    assertToken(scanToken(&lexer), TOKEN_NUMBER);
    assertToken(scanToken(&lexer), TOKEN_PLUS);
    assertToken(scanToken(&lexer), TOKEN_NUMBER);
    assertToken(scanToken(&lexer), TOKEN_EOF);
}

/*
Input:

    let x

Expected Tokens:

    TOKEN_LET
    TOKEN_IDENTIFIER
    TOKEN_EOF
*/
void testKeyword(void) {
    Lexer lexer;
    initLexer(&lexer, "let x");

    assertToken(scanToken(&lexer), TOKEN_LET);
    assertToken(scanToken(&lexer), TOKEN_IDENTIFIER);
    assertToken(scanToken(&lexer), TOKEN_EOF);
}

/*
Input:

    == !=

Expected Tokens:

    TOKEN_EQUAL_EQUAL
    TOKEN_BANG_EQUAL
    TOKEN_EOF
*/
void testComparisonOperators(void) {
    Lexer lexer;
    initLexer(&lexer, "== !=");

    assertToken(scanToken(&lexer), TOKEN_EQUAL_EQUAL);
    assertToken(scanToken(&lexer), TOKEN_BANG_EQUAL);
    assertToken(scanToken(&lexer), TOKEN_EOF);
}

int main(void) {
    testArithmetic();
    testKeyword();
    testComparisonOperators();

    printf("All tests passed.\n");

    return 0;
}
