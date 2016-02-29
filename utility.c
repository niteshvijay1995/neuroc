#include "dfa.c"
#include <stdio.h>
int char2index(char ch)
{
	//printf("=%c=",ch);
	if(ch>='b' && ch<='d')
	{
		return 2;
	}
	else if('A'<=ch && ch<='Z')
		return 0;
	else if('0'<=ch && ch<='9')
		return 3;
	else if('a'<=ch && ch<='z')
		return 1;
	else
	{
		switch(ch)
		{
			case '<':
				return 4;
			case '>':
				return 5;
			case '%':
				return 7;
			case '#':
				return 8;
			case '_':
				return 9;
			case '[':
				return 10;
			case ']':
				return 11;
			case ';':
				return 12;
			case ':':
				return 13;
			case '.':
				return 14;
			case '(':
				return 15;
			case ')':
				return 16;
			case '+':
				return 17;
			case '-':
				return 18;
			case '*':
				return 19;
			case '/':
				return 20;
			case '@':
				return 21;
			case '&':
				return 22;
			case '~':
				return 23;
			case '=':
				return 24;
			case '!':
				return 25;

		}
	}

}

void printstate(state** states)
{
	int i=0,j;
	Pair* temp;
	for(i=0;i<10;i++)
	{
		printf("\n%d --> ",i);
		temp = states[i]->trans;
		while(temp != NULL)
		{
			printf("%d %d,",temp->alpha,temp->next_state);
			temp = temp->next;
		}
		if(states[i]->isfinal == 1)
			{
				printf("Final state token = %s",states[i]->token);

			}
	}
}