#include "lexer.h"
#include <ctype.h>
#include <string.h>

static int isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

void skipWhitespace(Lexer* lexer){
    while (*lexer->current == ' '){
        lexer->current++;
    }
}


static char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}

static Token makeToken(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    return token;
}

static Token errorToken(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lexer->line;
    return token;
}

void initLexer(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

Token scanToken(Lexer* lexer) {
    skipWhitespace(lexer);

    lexer->start = lexer->current;

    if (isAtEnd(lexer)) return makeToken(lexer, TOKEN_EOF);

    char c = advance(lexer);

    switch(c) {
        case '(': return makeToken(lexer, TOKEN_LEFT_PAREN);
        case ')': return makeToken(lexer, TOKEN_RIGHT_PAREN);
        case '+': return makeToken(lexer, TOKEN_PLUS);
        case '-': return makeToken(lexer, TOKEN_MINUS);
        case '*': return makeToken(lexer, TOKEN_STAR);
        case '/': return makeToken(lexer, TOKEN_SLASH);
        case '=': return makeToken(lexer, TOKEN_EQUAL);
        case ';': return makeToken(lexer, TOKEN_SEMICOLON);
    }

    return errorToken(lexer, "unexpected character.");
}
