#include "lexer.c"
int main(int argc, char** args)
{
	int pflag = 0;
	if(argc < 2)
	{
		printf("No file passed\n");
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
				printf("%s ",temp->token);
			}
			else if(temp->isvalid == 0)
			{
				printf("Error on line %d Error Code : %d Lexeme : %s\n",temp->lineno,temp->errno,temp->lexeme);
			}
			temp = temp->next;
		}
	}
}

