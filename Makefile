all:
	gcc src/main.c src/lexer.c src/parser.c src/ast.c src/interpreter.c -o nibble

debug:
	gcc -g src/main.c src/lexer.c src/ast.c -o dsl

run:
	./nibble

clean:
	rm -f nibble
