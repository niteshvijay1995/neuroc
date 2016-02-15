#include "dfa.h"
#include<stdlib.h>
#include<stdio.h>
typedef struct finalstate finalstate;
struct finalstate
{
	int id;
	char* token;
	finalstate* next;
};
void read_dfa(char* filename)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	ssize_t read;
	char* line = NULL;
	char* tok;
	finalstate* head; 
	if(file == 0)
		printf("Problem opening file");
	else
	{
		while((len = getline(&line, &len, file)) != -1)
		{
			if(strcmp(line,"<FinalStates>\n") == 0)
			{
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</FinalStates>\n"))
				{
					tok = strtok(line," ");
					printf("%s",tok);
					tok = strtok(NULL," ");
					printf("%s",tok);
				}
			}
			printf("Retrieved line of length %zu :\n", read);
        		printf("%s", line);
		}
		fclose(file);

	}
}
int main()
{
	read_dfa("dfa.txt");
}
