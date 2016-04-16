#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void func(char* filename){
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	ssize_t read;
	char* line = NULL;
	char* tok;

	while((len = getline(&line, &len, file)) != -1){
		
		tok = strtok(line," ");
		tok = strtok(NULL," \n");
		//tok = strtok(NULL," \n");
		printf("%s\n", tok);
	}
}

int main(){
	func("grammarshivin.txt");
	return 0;
}