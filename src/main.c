#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "interpreter.h"

int main(void) {
    printf("Nibble: A small expression REPL.\n");
    printf("Type an expression, or 'exit' to quit.\n");
    char line[1024];

    while(1) {
        printf("> ");

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        if (strcmp(line, "exit\n") == 0) {
            break;
        }

        Parser parser;
        initParser(&parser, line);

        Expr* expr = parse(&parser);

        printf("AST: ");
        printExpr(expr);
        printf("\n");

        double result = evalExpr(expr);
        printf("= %g\n", result);

        freeExpr(expr);
    }
    return 0;
}

