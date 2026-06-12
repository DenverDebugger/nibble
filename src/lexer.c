#include "lexer.h"
#include <ctype.h>
#include <string.h>

static int isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

static int match(Lexer* lexer, char expected) {
    if (isAtEnd(lexer)) return 0;
    if (*lexer->current != expected) return 0;

    lexer->current++;
    return 1;
}

static char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}


static void skipWhitespace(Lexer* lexer){
    for (;;) {
        char c = *lexer->current;

        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;

            case '\n':
                lexer->line++;
                advance(lexer);
                break;


            default:
                return;
        }
    }
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

static int isAlpha(char c) {
    return isalpha(c) || c == '_';
}

static int isDigit(char c) {
    return isdigit(c);
}

static Token number(Lexer* lexer) {
    while (isdigit(*lexer->current)) {
        advance(lexer);
    }
    return makeToken(lexer, TOKEN_NUMBER);
}

static TokenType identifierType(Lexer* lexer) {
    int length = (int)(lexer->current - lexer->start);

    if (length == 3 && memcmp(lexer->start, "let", 3) == 0) {
        return TOKEN_LET;
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier(Lexer* lexer) {
    while (isAlpha(*lexer->current) || isDigit(*lexer->current)) {
        advance(lexer);
    }
    return makeToken(lexer, identifierType(lexer));
}

static Token string(Lexer* lexer) {
    while(*lexer->current != '"' && !isAtEnd(lexer)) {
        if (*lexer->current != '\n') lexer->line++;
        advance(lexer);
    }

    if (isAtEnd(lexer)) {
        return errorToken(lexer, "unterminated string.");
    }

    // consume closing "
    advance(lexer);

    return makeToken(lexer, TOKEN_STRING);
}

Token scanToken(Lexer* lexer) {
    skipWhitespace(lexer);

    lexer->start = lexer->current;

    if (isAtEnd(lexer)) return makeToken(lexer, TOKEN_EOF);

    char c = advance(lexer);

    if (isAlpha(c)) return identifier(lexer);
    if (isDigit(c)) return number(lexer);

    switch(c) {
        case '(': return makeToken(lexer, TOKEN_LEFT_PAREN);
        case ')': return makeToken(lexer, TOKEN_RIGHT_PAREN);
        case '+': return makeToken(lexer, TOKEN_PLUS);
        case '-': return makeToken(lexer, TOKEN_MINUS);
        case '*': return makeToken(lexer, TOKEN_STAR);
        case '/': 
            if(match(lexer, '/')){
                while(*lexer->current != '\n' && !isAtEnd(lexer)) {
                    advance(lexer);
                }
                return scanToken(lexer);
            }
            return makeToken(lexer, TOKEN_SLASH);
        case '=': 
            return makeToken(lexer, match(lexer, '=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case ';': return makeToken(lexer, TOKEN_SEMICOLON);
        case '!': return makeToken(lexer, match(lexer, '=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '<': return makeToken(lexer, match(lexer, '=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(lexer, match(lexer, '=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string(lexer);
    }

    return errorToken(lexer, "unexpected character.");
}
