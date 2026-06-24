#include "parser.h"
#include "ast.h"

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

static Expr* primary(Parser* parser) {
    if (match(parser, TOKEN_NUMBER)) {
        return newNumberExpr(parser->previous);
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        return newVariableExpr(parser->previous);
    }

    errorAtCurrent(parser, "expected expression");
    return NULL;
}

static Expr* expression(Parser* parser) {
    Expr* expr =  primary(parser);

    while (match(parser, TOKEN_PLUS)) {
        Token operator = parser->previous;
        Expr* right = primary(parser);
        expr = newBinaryExpr(expr, operator, right);
    }
    return expr;
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

    Expr* initializer = expression(parser);

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

Expr*  parse(Parser* parser) {
    Expr* expr = expression(parser);

    while (!check(parser, TOKEN_EOF)) {
       errorAtCurrent(parser, "expected end of input."); 
    }

    return expr;
}

