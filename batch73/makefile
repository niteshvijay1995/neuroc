program: lexer.o parser.o ast.o symbolTable.o typeChecker.o driver.c
	gcc -o stage1exe driver.c lexer.o parser.o ast.o symbolTable.o typeChecker.o
lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c
parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c
ast.o: ast.c ast.h astDef.h
	gcc -c ast.c
symbolTable.o: symbolTable.c symbolTable.h symbolTableDef.h
	gcc -c symbolTable.c
typeChecker.o: typeChecker.c typeChecker.h
	gcc -c typeChecker.c
clean:
	rm -f lexer.o parser.o ast.o symbolTable.o typeChecker.o code.asm code.o code.out o.am out.no stage1exe
