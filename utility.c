#include "dfa.h"
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

//returns the next state after searching linked list.
int search_linked_list(Pair* p, int alpha){
	Pair* head = p;
	while(head!=NULL){
		if(head->alpha == alpha) return head->next_state;
		head = head->next;
	}
	return -1;
}
char* getNextToken(char* input, state** states){
	int i,next_state_id;
	int curr_state_id = 0;
	char* a = malloc(100* sizeof(char));
	a = "Invalid stream";
	int len = strlen(input);
	for(i=0; i<len; i++){
		int alpha = char2index(input[i]);
		next_state_id =  search_linked_list(states[curr_state_id]->trans, alpha);
		if(next_state_id == -1) return a;
		else curr_state_id = next_state_id;
	}
	if(states[curr_state_id]->isfinal==1) return states[curr_state_id]->token;
	else return a;
}

