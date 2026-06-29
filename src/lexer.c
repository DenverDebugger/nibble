#include "lexer.h"
#include <ctype.h>
#include <string.h>

/**
 * @brief Checks if the lexer has reached the end of the source string.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return int 1 if the end of source is reached, 0 otherwise.
 */
static int isAtEnd(Lexer* lexer) {
    return *lexer->current == '\0';
}

/**
 * @brief Consumes the current character and moves the lexer pointer forward.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return char The consumed character.
 */
static char advance(Lexer* lexer) {
    lexer->current++;
    return lexer->current[-1];
}

/**
 * @brief Skips whitespace characters and increments the line count on newlines.
 * 
 * @param lexer A pointer to the lexer instance.
 */
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

/**
 * @brief Helper function to construct a Token of a specific type.
 * 
 * @param lexer A pointer to the lexer instance.
 * @param type The type of token to create.
 * @return Token The constructed Token.
 */
static Token makeToken(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->start;
    token.length = (int)(lexer->current - lexer->start);
    token.line = lexer->line;
    return token;
}

/**
 * @brief Helper function to construct an error Token with a specific message.
 * 
 * @param lexer A pointer to the lexer instance.
 * @param message The error message to associate with the token.
 * @return Token The constructed error Token.
 */
static Token errorToken(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lexer->line;
    return token;
}

/**
 * @brief Initializes the lexer with the given source string.
 * 
 * @param lexer A pointer to the lexer instance to initialize.
 * @param source The source string to be tokenized.
 */
void initLexer(Lexer* lexer, const char* source) {
    lexer->start = source;
    lexer->current = source;
    lexer->line = 1;
}

/**
 * @brief Checks if a character is an alphabetic letter or underscore.
 * 
 * @param c The character to check.
 * @return int 1 if alphabetic or underscore, 0 otherwise.
 */
static int isAlpha(char c) {
    return isalpha(c) || c == '_';
}

/**
 * @brief Checks if a character is a numeric digit.
 * 
 * @param c The character to check.
 * @return int 1 if a digit, 0 otherwise.
 */
static int isDigit(char c) {
    return isdigit(c);
}

/**
 * @brief Scans a numeric literal token.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return Token The scanned number Token.
 */
static Token number(Lexer* lexer) {
    while (isdigit(*lexer->current)) {
        advance(lexer);
    }
    return makeToken(lexer, TOKEN_NUMBER);
}

/**
 * @brief Determines if an identifier matches any keyword.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return TokenType TOKEN_LET if the identifier matches "let", TOKEN_IDENTIFIER otherwise.
 */
static TokenType identifierType(Lexer* lexer) {
    int length = (int)(lexer->current - lexer->start);

    if (length == 3 && memcmp(lexer->start, "let", 3) == 0) {
        return TOKEN_LET;
    }

    return TOKEN_IDENTIFIER;
}

/**
 * @brief Scans an identifier or keyword token.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return Token The scanned identifier or keyword Token.
 */
static Token identifier(Lexer* lexer) {
    while (isAlpha(*lexer->current) || isDigit(*lexer->current)) {
        advance(lexer);
    }
    return makeToken(lexer, identifierType(lexer));
}

/**
 * @brief Scans and returns the next token from the source string.
 * 
 * @param lexer A pointer to the lexer instance.
 * @return Token The next scanned Token.
 */
Token scanToken(Lexer* lexer) {
    skipWhitespace(lexer);

    lexer->start = lexer->current;

    if (isAtEnd(lexer)) return makeToken(lexer, TOKEN_EOF);

    char c = advance(lexer);

    if (isAlpha(c)) return identifier(lexer);
    if (isDigit(c)) return number(lexer);

    switch(c) {
        case '+': return makeToken(lexer, TOKEN_PLUS);
        case '=': return makeToken(lexer, TOKEN_EQUAL); 
        case ';': return makeToken(lexer, TOKEN_SEMICOLON);
        case '(': return makeToken(lexer, TOKEN_LEFT_PAREN);
        case ')': return makeToken(lexer, TOKEN_RIGHT_PAREN);
        case '-': return makeToken(lexer, TOKEN_MINUS);
        case '*': return makeToken(lexer, TOKEN_STAR);
        case '/': return makeToken(lexer, TOKEN_SLASH);
    }

    return errorToken(lexer, "unexpected character.");
}
