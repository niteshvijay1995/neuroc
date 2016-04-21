#include "parser.h"
#include "lexer.h"
#include "ast.h"

void ast_init()
{
	modify_grammar("astRules.txt");
}

void print_grammar_for_ast(list_gram** rules){
	int i =0;
	for(i=0; i<NUM_RULES; i++){
		list_gram* temp = rules[i];
		while(temp!=NULL){
			printf("%s (%d)", temp->value,temp->ast_value);
			temp = temp->next;
		}
		printf("\n");
	}
}

void modify_grammar(char* filename)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	size_t nbytes = 1000;
	ssize_t read;
	char* line = malloc(nbytes+1);
	char* tok;
	
	int i;
	if(file == 0)
		printf("Problem opening file");
	else{
		i=0;
		while((len = getline(&line, &nbytes, file)) != -1){
			if(rules[i] == NULL)
			{
				printf("Error - Problem in reading grammer\n");
			}
			else
			{
				list_gram* temp;
				temp = rules[i]->next;
				rules[i]->ast_value = -1;
				tok = strtok(line," ");
				while(tok!=NULL && temp!=NULL){
					temp->ast_value = atoi(tok);
					tok = strtok(NULL," \n");
					temp = temp->next;
				}	
			}
			i++;
		}
	}
	//print_grammar_for_ast(rules);		
}

astTree* ast_new_node(ntree *root)
{
	astTree* new_node;
	new_node = (astTree*)malloc(sizeof(astTree));
	new_node->size = 0;
	new_node->node_symbol = root->node_symbol;
	new_node->lexeme = root->lexeme;
	new_node->lineno = root->lineno;
	new_node->type = -1;
	return new_node;
}

astTree* construct_ast(ntree *root)
{
	//printf("\nCalled\n");
	int i=0,j;
	astTree* new_node = ast_new_node(root);
	astTree* temp;
	//printf("\ninit complete\n");
	for(i=0;i<root->size;i++)
	{
		if(root->next[i]->ast_value == 0)	//removed
			continue;	
		else if(root->next[i]->ast_value == 2)	
		{
			new_node->children[new_node->size++] = construct_ast(root->next[i]);
		}
		else if(root->next[i]->ast_value == 1)
		{
			temp = construct_ast(root->next[i]);
			for(j=0;j<temp->size;j++)
			{
				new_node->children[new_node->size++] = temp->children[j];
			}
		}
	}
	return new_node;
}

void copy_ast_node(astTree* r1, astTree* r2){
	r1->node_symbol = r2->node_symbol;
	r1->lexeme = r2->lexeme;
	r1->lineno = r2->lineno;
}

astTree* clean_ast(astTree* root){
	int i;
	for(i=0; i<root->size; i++){
		while(root->children[i]->size==1 && root->children[i]->node_symbol[0]=='<' && !cannotBeDeleted(root->children[i]->node_symbol)){
			astTree* temp = root->children[i];
			root->children[i] = root->children[i]->children[0];
			free(temp);
		}
		if(root->children[i]->size ==0 && root->children[i]->node_symbol[0]=='<')
		{
			int j;
			root->size--;
			for(j=i; j<root->size;j++)
			{
				root->children[j] = root->children[j+1];
			}
			i--;
		}
		else if(check_pull_up(root->children[i]->node_symbol)&&!cannotBeDeleted(root->node_symbol)){
			astTree* temp = root->children[i];
			copy_ast_node(root, root->children[i]);
			root->size--;
			free(temp);
			int j;
			for(j=i; j<root->size;j++)
			{
				root->children[j] = root->children[j+1];
			}
			i--;	
		}
		else
		{
			root->children[i] = clean_ast(root->children[i]);
		}
	}
	return root;
}
void print_ast(astTree* root)
{
	if(root == NULL)
		return;
	int i;
	if(root->size != 0)
		printf("\nparent - %s children - ",root-> node_symbol);
	for(i=0;i<root->size;i++)
	{
		printf(" %s ( %s )[ %d] ",root->children[i]->node_symbol, root->children[i]->lexeme, root->children[i]->type);
	}	
	for(i=0;i<root->size;i++)
	{
		print_ast(root->children[i]);
	}
}

int check_pull_up(char* string)
{
	if (strcmp(string, "TK_PLUS") == 0 || strcmp(string, "TK_MINUS") == 0 || strcmp(string, "TK_DIV") == 0 || strcmp(string, "TK_MUL") == 0 || strcmp(string, "TK_ASSIGNOP") == 0
		|| strcmp(string, "TK_WRITE") == 0 || strcmp(string, "TK_READ") == 0 || strcmp(string, "TK_AND") == 0 || strcmp(string, "TK_OR") == 0 || strcmp(string, "TK_LT") == 0
		|| strcmp(string, "TK_LE") == 0 || strcmp(string, "TK_EQ") == 0 || strcmp(string, "TK_GT") == 0 || strcmp(string, "TK_GE") == 0 || strcmp(string, "TK_NE") == 0 || strcmp(string, "TK_NOT") == 0
		|| strcmp(string, "TK_FUNID") == 0 || strcmp(string, "TK_MAIN") == 0)
		return 1;
	else return 0;
}

int cannotBeDeleted(char* string)
{
	return strcmp(string, "<whileBody>") == 0 || strcmp(string, "<singleOrRecId>") == 0/*|| strcmp(string, "<booleanExpression>") == 0*/ || strcmp(string, "<funCallStmt>") == 0
		|| strcmp(string, "<parameter_list>") == 0 || strcmp(string, "<ifBody>") == 0 || strcmp(string, "<elsePart>") == 0 || strcmp(string, "<outputParameters>") == 0 || strcmp(string, "<inputParameters>") == 0 || strcmp(string, "<optionalReturn>") == 0;
}
