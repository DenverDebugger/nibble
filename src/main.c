#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void) {
    const char* source = "let x = 5 + 2";

    Parser parser;
    initParser(&parser, source);

    parse(&parser);

   if (parser.hadError) {
       return 1;
   }

   printf("Parse succeeded.\n");
    return 0;
}
