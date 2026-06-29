#include "parser.h"
#include "ast.h"

#include <stdio.h>

static Expr* expression(Parser* parser);
static Expr* term(Parser* parser);
static Expr* factor(Parser* parser);
static Expr* primary(Parser* parser);

/**
 * @brief Advances the parser by consuming the current token and scanning the next one.
 * 
 * @param parser A pointer to the parser instance.
 */
static void advance(Parser* parser) {
    parser->previous = parser->current;
    parser->current = scanToken(&parser->lexer);
}

/**
 * @brief Checks if the current token is of the specified type.
 * 
 * @param parser A pointer to the parser instance.
 * @param type The TokenType to check against.
 * @return int 1 if the current token type matches, 0 otherwise.
 */
static int check(Parser* parser, TokenType type) {
    return parser->current.type == type;
}

/**
 * @brief Checks if the current token matches the specified type and consumes it if so.
 * 
 * @param parser A pointer to the parser instance.
 * @param type The TokenType to match and consume.
 * @return int 1 if the token matched and was consumed, 0 otherwise.
 */
static int match(Parser* parser, TokenType type) {
    if (!check(parser, type)) {
        return 0;
    }
    advance(parser);
    return 1;
}

/**
 * @brief Reports a parsing error at the current token and sets the error flag.
 * 
 * @param parser A pointer to the parser instance.
 * @param message The error message to print.
 */
static void errorAtCurrent(Parser* parser, const char* message) {
    fprintf(stderr, "[line %d] Error: %s\n",
            parser->current.line,
            message);
    parser->hadError = 1;
}

/**
 * @brief Consumes the current token if it matches the expected type, otherwise reports an error.
 * 
 * @param parser A pointer to the parser instance.
 * @param type The expected TokenType to consume.
 * @param message The error message to report if the token type does not match.
 */
static void consume(Parser* parser, TokenType type, const char* message) {
    if (parser->current.type == type) {
        advance(parser);
        return;
    }
    errorAtCurrent(parser, message);
}

/**
 * @brief Parses a factor expression (multiplication and division).
 * 
 * @param parser A pointer to the parser instance.
 * @return Expr* The parsed factor expression tree.
 */
static Expr* factor(Parser* parser) {
    Expr* expr = primary(parser);

    while (match(parser, TOKEN_STAR) || match(parser, TOKEN_SLASH)) {
        Token operator = parser->previous;
        Expr* right = primary(parser);
        expr = newBinaryExpr(expr, operator, right);
    }
    return expr;
}

/**
 * @brief Parses a term expression (addition and subtraction).
 * 
 * @param parser A pointer to the parser instance.
 * @return Expr* The parsed term expression tree.
 */
static Expr* term(Parser* parser) {
    Expr* expr = factor(parser);

    while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
        Token operator = parser->previous;
        Expr* right = factor(parser);
        expr = newBinaryExpr(expr, operator, right);
    }
    return expr;
}

/**
 * @brief Parses an expression (currently delegates to term).
 * 
 * @param parser A pointer to the parser instance.
 * @return Expr* The parsed expression tree.
 */
static Expr* expression(Parser* parser) {
    return term(parser);
}

/**
 * @brief Parses a primary expression (numbers, identifiers, or grouped expressions).
 * 
 * @param parser A pointer to the parser instance.
 * @return Expr* The parsed primary expression tree.
 */
static Expr* primary(Parser* parser) {
    if (match(parser, TOKEN_NUMBER)) {
        return newNumberExpr(parser->previous);
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        return newVariableExpr(parser->previous);
    }

    if (match(parser, TOKEN_LEFT_PAREN)) {
        Expr* expr = expression(parser);
        consume(parser, TOKEN_RIGHT_PAREN, "expected ')' after expression.");
        return expr;
    }

    errorAtCurrent(parser, "expected expression");
    return NULL;
}

/**
 * @brief Parses a variable declaration beginning with 'let'.
 * 
 * @param parser A pointer to the parser instance.
 */
static void declaration(Parser* parser) {
    consume(parser, TOKEN_LET, "expected 'let'");
    consume(parser, TOKEN_IDENTIFIER, "expected variable name");
    consume(parser, TOKEN_EQUAL, "expected '='");

    expression(parser);

    consume(parser, TOKEN_SEMICOLON, "expected ';'");
}

/**
 * @brief Parses the remaining portion of a 'let' variable declaration.
 * 
 * @param parser A pointer to the parser instance.
 */
static void letDeclaration(Parser* parser) {
    consume(parser, TOKEN_IDENTIFIER, "expected variable name.");
    consume(parser, TOKEN_EQUAL, "expected '=' after variable name.");

    Expr* initializer = expression(parser);

    consume(parser, TOKEN_SEMICOLON, "expected ';' after declaration.");
}

/**
 * @brief Initializes the parser with the given source string.
 * 
 * @param parser A pointer to the parser instance to initialize.
 * @param source The source string to be parsed.
 */
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

/**
 * @brief Parses an expression from the source and expects end of input.
 * 
 * @param parser A pointer to the parser instance.
 * @return Expr* The parsed expression tree.
 */
Expr*  parse(Parser* parser) {
    Expr* expr = expression(parser);

    if (!check(parser, TOKEN_EOF)) {
       errorAtCurrent(parser, "expected end of input."); 
    }

    return expr;
}

