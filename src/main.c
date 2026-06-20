#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void) {
    const char* source = "let a = abc;";

    Parser parser;
    initParser(&parser, source);

    parse(&parser);

   if (parser.hadError) {
       printf("parse failed.\n");
       return 1;
   }

   printf("Parse succeeded.\n");
    return 0;
}
