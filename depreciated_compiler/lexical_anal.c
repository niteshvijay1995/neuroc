#include<stdio.h>
int main(int argc, char *argv[])
{
	if(argc < 2)
		printf("You must pass a file\n");
	else if(argc >2)
		printf("Too many files passed\n");
	else
	{
		FILE *prog = fopen(argv[1], "r");
		if(prog == 0)
		{
			printf("There is some problem opening the file \n");
			exit(0); 
		}
		else
		{
			
		}
	}
	printf("Hello World\n");
}
