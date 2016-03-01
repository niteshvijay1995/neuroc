#include "utility.c"
#include <stdlib.h>
#include <stdio.h>
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
//int final[];
int num_states = 60;
typedef struct finalstate finalstate;
struct finalstate
{
	int id;
	char* token;
	finalstate* next;
};

void read_dfa(char* filename, state** states)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	ssize_t read;
	char* line = NULL;
	char* tok;
	int i;
	for(i=0;i<num_states;i++)
	{
		states[i] = (state*)malloc(sizeof(state));
		states[i]->isfinal = 0;
		states[i]->islookup = 0;
	}
	lookup_table = ht_create(30);
	printf("States initialised\n");
	if(file == 0)
		printf("Problem opening file");
	else
	{
		while((len = getline(&line, &len, file)) != -1)
		{
			if(strcmp(line,"<FinalStates>\n") == 0)
			{
				printf("Reading Final States ...\n");
				int final_state_id;
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</FinalStates>\n"))
				{
					
					tok = strtok(line," ");
					final_state_id = atoi(tok);
					tok = strtok(NULL," \n");
					states[final_state_id]->token = (char*)malloc(sizeof(tok));
					states[final_state_id]->isfinal = 1;
					strcpy(states[final_state_id]->token,tok);
				}
			}
			if(strcmp(line,"<Transition>\n") == 0)
			{
				printf("Reading Transition States ...\n");
				//temp = head;
				int from;
				int to;
				int alpha_index;
				char alpha;
				Pair* temp_pair;		
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</Transition>\n"))
				{
					printf(".");
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
						//printf("Adding more trans\n");
						while(temp_pair->next != NULL)
							temp_pair = temp_pair->next;
						temp_pair->next = (Pair*)malloc(sizeof(Pair));
						temp_pair = temp_pair->next;
					}	
					
					temp_pair->next_state = to;
					temp_pair->alpha = alpha_index;
					temp_pair->next = NULL;
				}
			}
			if(strcmp(line,"<Lookup>\n") == 0)
			{
				printf("Reading Lookup ...\n");
				int state;
				char lexime[20];
				char token[20];		
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</Lookup>\n"))
				{
					printf(".");
					tok = strtok(line," ");
					state = atoi(tok);
					states[state]->isfinal = 1;
					states[state]->islookup = 1;	
					tok = strtok(NULL," ");
					strcpy(lexime,tok);					
					tok = strtok(NULL,"\n");
					strcpy(token,tok);
					ht_insert(lookup_table,lexime,token);
				}
			}
		}
		fclose(file);
	//print(head);
	printstate(states);
	//return states;
	}
}
