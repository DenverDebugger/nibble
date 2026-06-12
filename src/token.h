#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_SEMICOLON,
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_GREATER, 
    TOKEN_LESS_EQUAL,
    TOKEN_LESS,

    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,

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
