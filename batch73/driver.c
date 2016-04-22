/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/

#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"
void printcode(tokenInfo* tokenlist)
{
	int lineno = 0;
    while(tokenlist->next!=NULL)
    {
    	if(lineno != tokenlist->lineno)
    	{
    		lineno = tokenlist->lineno;
    		printf("\n");
    		printf("%d :",lineno);
    	}
    	printf("%s ",tokenlist->lexeme);
    	tokenlist = tokenlist->next;
    }
}

void printtoken(tokenInfo* tokenlist)
{
	while(tokenlist->next!=NULL)
	{
		if(tokenlist->isvalid)
			printf("%d : %s (%s)\n",tokenlist->lineno,tokenlist->token,tokenlist->lexeme);
		else
			printf("%d : Invalid token (%s)\n",tokenlist->lineno,tokenlist->lexeme);
		tokenlist = tokenlist->next;
	}
}

int main(int argc, char** args)
{
	int pflag = 0;
	int o;
	if(argc < 3)
	{
		if(argc<2)
		{
			printf("Need an input program file.\n");
		}
		else
		{
			printf("Need an input program file and an output file.\n");
		}
		exit(0);
	}
	else
	{
		FILE *fp = fopen(args[1],"r");
		if(fp == NULL)
		{
			printf("File not found !!\n");
			exit(0);
		}
		if(argc == 4 && !strcmp(args[3],"-p"))
			pflag = 1;
		
		printf("First and Follow set automated.\n");
		printf("Both lexical and syntax analysis modules implemented.\n");
		printf("Modules work with all the testcases. (Testcase4 prints relevant errors).\n");
		printf("Parse tree is printed to output file given at command line.\n");
		printf("Error recovery in Parsing is implemented.\n\n");
		
		tokenInfo* tokenlist;
		tokenlist = getNextToken(fp);
		ntree* root;
		astTree* astTree_root;
		int error=0;
		parser_init();
		ast_init();
		while(1)
			{	
				printf("\nPress option for the defined task\n 1 : Print the comment free code on the console.\n 2 : Print the token list (on the console) generated by the lexer.\n 3 : Parse to verify the syntactic correctness of the input source code.\n 4 : Create the parse tree\n 5 : Print AST and Symbol Table \n 6 : Exit \n\n");
				printf("Option : ");
				scanf("%d",&o);
				switch(o)
				{
					case 1:
						printcode(tokenlist);
						printf("\n");
						break;
					case 2:
						printtoken(tokenlist);
						break;
					case 3:
						root = parseInputSourceCode(args[1], &error);
						if(error==0) printf("\nCompiled Successfully: Input source code is syntactically correct!!\n\n");
						else printf("\nCompilation Error\n\n");
						break;
					case 4:
						root = parseInputSourceCode(args[1], &error);
						fp = fopen(args[2], "w");
						printParseTree(root, fp);
						if(error==0) printf("\nCompiled Successfully: Parse tree printed in file %s!!\n\n",args[2]);
						else printf("\nCompilation Error: Parse tree printed in file %s!!\n\n",args[2]);
						fclose(fp);
						break;
					case 5:
						modify_grammar("astRules.txt");
						astTree_root =  construct_ast(root);
						//print_ast(astTree_root);
						astTree_root = clean_ast(astTree_root);
						print_ast(astTree_root);
						printf("\n_____________________________\n");
						symbol_list* lis;
						lis = malloc(sizeof(symbol_list*));
						lis->next = NULL;
						sym_table* st = createSymbolTable(astTree_root, &lis);
						printSymbolTable(st, lis);
						//print_ast(astTree_root);
						init_typechecker(astTree_root, st,NULL);
						int parse_tree_size = count_nodes_parser(root);
						int ast_tree_size = count_nodes_ast(astTree_root);
						printf("size of parse tree: %d, size of ast: %d\n", parse_tree_size, ast_tree_size);
						double compression = 100.0 - (double)ast_tree_size/(double)parse_tree_size * 100.0;
						printf("compression percentage: %lf", compression);
						FILE* fp = fopen("code.asm", "w");
						code_gen(astTree_root->children[0],st,lis,fp);
						break;
					default:
						exit(0);
				}
			}
		}
}
