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

static void expression(Parser* parser) {
    if (match(parser, TOKEN_NUMBER)) {
        return;
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        return;
    }

    errorAtCurrent(parser, "expected expression");
}

static void declaration(Parser* parser) {
    consume(parser, TOKEN_LET, "expected 'let'");
    consume(parser, TOKEN_IDENTIFIER, "expected variable name");
    consume(parser, TOKEN_EQUAL, "expected '='");

    expression(parser);

    consume(parser, TOKEN_SEMICOLON, "expected ';'");
}

static void letDeclaration(Parser* parser) {
    consume(parser, TOKEN_IDENTIFIER, "expected variable name.");
    consume(parser, TOKEN_EQUAL, "expected '=' after variable name.");

    expression(parser);

    consume(parser, TOKEN_SEMICOLON, "expected ';' after declaration.");
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
    while (!check(parser, TOKEN_EOF)) {
        declaration(parser); // top level of the grammar
    }
}

