all:
	gcc src/main.c src/lexer.c src/parser.c src/ast.c -o dsl

debug:
	gcc -g src/main.c src/lexer.c src/ast.c -o dsl

run:
	./dsl

clean:
	rm -f dsl
