program: lexer.o parser.o driver.c
	gcc -o stage1exe driver.c lexer.o parser.o 
lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c -I.
parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c -I.
