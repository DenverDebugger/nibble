#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_PLUS,
    TOKEN_EQUAL,
    TOKEN_SEMICOLON,

    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,

    TOKEN_LET,
    TOKEN_PRINT,

    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;

#endif
