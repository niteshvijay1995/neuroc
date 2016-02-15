#include "dfa.c"
#include<stdlib.h>
#include<stdio.h>
/*
[A-Z] -> 0
[a U e-z] -> 1
[b-d] -> 2
[0-9] -> 3
< -> 4
> -> 5
% -> 7
# -> 8 
_ -> 9
[ -> 10
] -> 11
; -> 12
: -> 13
. -> 14
( -> 15
) -> 16
+ -> 17
- -> 18
* -> 19
/ -> 20
@ -> 21
& -> 22
~ -> 23
= -> 24
! -> 25

*/
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
typedef struct finalstate finalstate;
struct finalstate
{
	int id;
	char* token;
	finalstate* next;
};
void print(finalstate* head)
{
	finalstate* temp;
	temp = head;
	while(temp->next != NULL)
	{
		printf("%d %s\n",temp->id,temp->token);
		temp = temp->next;
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
			printf("%d %d,",temp->alpha,temp->state);
			temp = temp->next;
		}
	}
}
void read_dfa(char* filename)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	ssize_t read;
	char* line = NULL;
	char* tok;
	finalstate* head;
	finalstate* temp; 
	head = (finalstate*)malloc(sizeof(finalstate));
	state* states[11];
	int i;
	for(i=0;i<11;i++)
	{
		states[i] = (state*)malloc(sizeof(state));
	}
	if(file == 0)
		printf("Problem opening file");
	else
	{
		while((len = getline(&line, &len, file)) != -1)
		{
			if(strcmp(line,"<FinalStates>\n") == 0)
			{
				temp = head;	
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</FinalStates>\n"))
				{
					
					tok = strtok(line," ");
					temp->id = atoi(tok);						
					//printf("%s",tok);
					tok = strtok(NULL," \n");
					temp->token = (char*)malloc(sizeof(tok));
					strcpy(temp->token,tok);
					//printf("%s",tok);
					temp->next = (finalstate*)malloc(sizeof(finalstate));
					temp = temp->next;
				}
				free(temp);
			}
			if(strcmp(line,"<Transition>\n") == 0)
			{
				temp = head;
				int from;
				int to;
				int alpha_index;
				char alpha;
				Pair* temp_pair;		
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</Transition>\n"))
				{
					//printf("..%s..",line);
					tok = strtok(line," ");
					from = atoi(tok);						
					//printf("--%s--",tok);
					tok = strtok(NULL," ");
					alpha = tok[0];
					//printf("-------%s------",tok);
					alpha_index = char2index(alpha);
					//printf(".%d.",alpha_index);					
					tok = strtok(NULL," \n");
					//printf("%s",tok);	
					to = atoi(tok);
					temp_pair = states[from]->trans;
					if(temp_pair == NULL)
					{
						states[from]->trans = (Pair*)malloc(sizeof(Pair));
						temp_pair = states[from]->trans;
						//printf("||%d\n",from);
					}
					else
					{
						while(temp_pair->next != NULL)
							temp_pair = temp_pair->next;
						temp_pair->next = (Pair*)malloc(sizeof(Pair));
						temp_pair = temp_pair->next;
					}	
					temp_pair->state = to;
					temp_pair->alpha = alpha_index;
				}
			}
		}
		fclose(file);
	print(head);
	printstate(states);
	}
}
int main()
{
	read_dfa("dfa.txt");
}
