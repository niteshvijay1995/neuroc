program: lexer.o parser.o ast.o symbolTable.o driver.c
	gcc -o stage1exe driver.c lexer.o parser.o ast.o symbolTable.o
lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c
ast.o: ast.c ast.h astDef.h
	gcc -c ast.c
symbolTable.o: symbolTable.c symbolTable.h symbolTableDef.h
	gcc -c symbolTable.c
clean:
	rm -f lexer.o parser.o ast.o symbolTable.o
