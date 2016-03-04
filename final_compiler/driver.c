#include "parser.c"
int main(int argc, char** args)
{
	int pflag = 0;
	if(argc < 3)
	{
		printf("Need an input program file and an output file.\n");
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
		if(argc == 3 && !strcmp(args[2],"-p"))
			pflag = 1;
		tokenInfo* tokenlist;
		tokenInfo* temp;
		tokenlist = getNextToken(fp);
		temp = tokenlist;
		int lineno = 1;
		while(temp->next !=NULL)
		{
			if(pflag && temp->isvalid == 1)
			{
				if(temp->lineno != lineno)
				{
					printf("\n");
					lineno = temp->lineno;
				}
				printf("%s(%s) ",temp->token,temp->lexeme);
			}
			else if(temp->isvalid == 0)
			{
				//printf("Error on line %d Error Code : %d Lexeme : %s\n",temp->lineno,temp->errno,temp->lexeme);
			}
			temp = temp->next;
		}
	}
	printf("\nFirst and Follow set automated.\n");
	printf("\nBoth lexical and syntax analysis modules implemented.\n");
	printf("\nModules work with all the testcases. (Testcase4 prints relevant errors).\n");
	printf("\nParse tree is printed to output file given at command line.\n");
	printf("\nError recovery in Parsing is implemented.\n");
	list_gram* rules[num_rules];
	looktable* lt_non_terminal = lt_create(128);
	looktable* lt_terminal = lt_create(128);
	char* nt_strings[50];
	int i = 0,j;
	for(i=0; i<num_rules; i++){
		rules[i] = malloc(sizeof(list_gram));
	}
	for(i=0;i<non_terminals;i++)
	{
		fs[i] = malloc(sizeof(first_set));
		fs[i]->size = 0;
		fs[i]->valid = 0;
		fs[i]->eflag = 0;
		fol[i] = malloc(sizeof(follow_set));
		fol[i]->size = 0;
		fol[i]->valid = 0;
		fol[i]->eflag = 0;
		fol[i]->folof = -1;
	}
	readgrammar("grammarnew.txt", rules);
	//print_grammar(rules);

	FILE *file = fopen("nonterminal.txt","r");
	size_t len = 0;
	size_t nbytes = 1000;
	ssize_t read;
	char* line = malloc(nbytes+1);
	char* tok;
	i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_non_terminal, line, i);
		nt_strings[i] = strdup(line);
		i++;
	}
	fclose(file);

	for(i=0;i<50;i++){
		find_first_set(nt_strings[i], rules, lt_non_terminal);
		//print_fs(lt_non_terminal,nt_strings[i]);
	}
	find_fol(rules, lt_non_terminal);

	file = fopen("terminals.txt", "r");
	i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_terminal, line, i);
		i++;
	}

	for(i=0;i<NON_TERMINAL;i++){
		for(j=0;j<TERMINAL;j++){
			parse_table[i][j] = -1;
		}
	}
	createParseTable(lt_non_terminal, lt_terminal, rules);
	//print_parse_table();
	int error=0;
	ntree* root = parseInputSourceCode(args[1],rules, lt_terminal, lt_non_terminal, &error);
	if(error==0) printf("\nCompiled Successfully: Input source code is syntactically correct!!\n");
	FILE* fp = fopen(args[2], "w");
	printParseTree(root, fp);
	fclose(fp);
}