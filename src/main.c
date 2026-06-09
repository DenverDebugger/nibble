#include <stdio.h>
#include "lexer.h"

int main(void) {
    const char* source = "let x = 5 + 3;";

    Lexer lexer;
    initLexer(&lexer, source);

    for (;;) {
        Token token = scanToken(&lexer);

        printf("type=%d text='%.*s'\n",
                token.type,
                token.length,
                token.start);

        if (token.type == TOKEN_EOF) break;
    }

    return 0;
}
