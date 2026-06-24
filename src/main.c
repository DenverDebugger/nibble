#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

int main(void) {
    const char* source = "1 + 2 + 3";

    Parser parser;
    initParser(&parser, source);

    Expr* expr = parse(&parser);

   if (parser.hadError) {
       printf("parse failed.\n");
       freeExpr(expr);
       return 1;
   }

   printf("AST: ");
   printExpr(expr);
   printf("\n");

   freeExpr(expr);

   return 0;
}

