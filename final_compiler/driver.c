#include "lexer1.c"
int main()
{
	FILE *fp = fopen("./testcases/testcase2.txt","r");
	tokenInfo* tokenlist;
	tokenlist = getNextToken(fp);
	while(tokenlist->next !=NULL)
	{
		if(tokenlist->isvalid == 1)
		{
			//printf("%s\n",tokenlist->token);
		}
		else
		{
			printf("Error on line %d Error Code : %d Lexeme : %s\n",tokenlist->lineno,tokenlist->errno,tokenlist->lexeme);
		}
		tokenlist = tokenlist->next;
	}
}

