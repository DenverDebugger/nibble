all:
	gcc src/main.c src/lexer.c src/parser.c src/ast.c src/interpreter.c -o nibble

debug:
	gcc -g src/main.c src/lexer.c src/ast.c -o dsl

run:
	./nibble

test:
	gcc tests/lexerTests.c -o tests/lexerTests
	gcc tests/precedenceTests.c src/lexer.c src/parser.c src/ast.c -o tests/precedenceTests
	gcc tests/astTests.c src/ast.c -o tests/astTests
	gcc tests/interpreterTests.c src/lexer.c src/parser.c src/ast.c src/interpreter.c -lm -o tests/interpreterTests
	./tests/lexerTests
	./tests/precedenceTests
	./tests/astTests
	./tests/interpreterTests

clean:
	rm -f nibble tests/lexerTests tests/precedenceTests tests/astTests tests/interpreterTests
