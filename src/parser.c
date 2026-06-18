#include "parser.h"

#include <stdio.h>

static void advance(Parser* parser) {
    parser->previous = parser->current;
    parser->current = scanToken(&parser->lexer);
}

static int check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

static int match(Parser* parser, TokenType type) {
    if (!check(parser, type)) {
        return 0;
    }
    advance(parser);
    return 1;
}

static void errorAtCurrent(Parser* parser, const char* message) {
    fprintf(stderr, "[line %d] Error: %s\n",
            parser->current.line,
            message);
    parser->hadError = 1;
}

static void consume(Parser* parser, TokenType type, const char* message) {
    if (parser->current.type == type) {
        advance(parser);
        return;
    }
    errorAtCurrent(parser, message);
}

void initParser(Parser* parser, const char* source) {
    initLexer(&parser->lexer, source);

    parser->hadError = 0;

    parser->current.type = TOKEN_ERROR;
    parser->current.start = "";
    parser->current.length = 0;
    parser->current.line = 1;

    parser->previous = parser->current;

    advance(parser);
}

void parse(Parser* parser) {
    while (!match(parser, TOKEN_EOF)) {
        advance(parser);
    }
}

