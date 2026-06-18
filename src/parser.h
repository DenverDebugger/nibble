#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    Lexer lexer;
    Token current;
    Token previous;
    int hadError;
} Parser;

void initParser(Parser* parser, const char* source);
void parse(Parser* parser);

#endif
