#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    int hadError;
} Parser;

void initParser(Parser* parser, const char* source);
Expr* parse(Parser* parser);

#endif
