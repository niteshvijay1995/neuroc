#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lookuptable.c"
#include "lexer.c"
#define NON_TERMINAL 50
#define TERMINAL 53
struct list_gram{
	char* value;
	struct list_gram* next;
};
typedef struct list_gram list_gram;

struct first_set{
	char* arr[30];
	int valid;
	int size;
	int eflag;
};
typedef struct first_set first_set;

struct snode{
	char* value;
	struct snode* next;
};
typedef struct snode snode;

struct ntree{
	int size;
	int consumed;
	char* lexeme;
	char* node_symbol;
	struct ntree* next[20];
	int is_leaf_node;
	struct ntree* parent;
	int lineno;
};


typedef struct ntree ntree;


int num_rules = 89;
int non_terminals = 50;
int num_terminals = 53; 
first_set* fs[50];
first_set* fol[50];
int parse_table[NON_TERMINAL][TERMINAL];

void st_push(snode** head, char* value){
	snode* newp = malloc(sizeof(snode));
	newp->value = strdup(value);
	newp->next = *head;
	*head = newp;
}

void s_node_push(ntree* root, char* value){
	ntree* temp = malloc(sizeof(ntree));
	temp->size = 0;
	temp->consumed = 0;
	//if(value[0]=='T') {temp->lexeme = tok->lexeme;}
	temp->lexeme = NULL;
	temp->lineno = 0;
	temp->node_symbol = strdup(value);
	if(value[0]=='T' || value[0] == 'e') temp->is_leaf_node = 1;
	else temp->is_leaf_node = 0;
	temp->parent = root;
	root->next[root->size] = temp;
	root->size++;
	printf("\nNode %s inserted in %s at %d\n",temp->node_symbol,root->node_symbol,(root->size-1));
}
char* st_pop(snode** head){
	char* value;
	value = strdup((*head)->value);
	//printf("Value popping = %s..\n",value);
	*head = (*head)->next;
	return value;
}

void push_rule(snode** head ,ntree* s_node, list_gram* rule ){
	list_gram* temp = rule;
	//temp = temp->next;
	s_node_push(s_node, temp->value);
	if(temp->next!=NULL){
		push_rule(head, s_node, temp->next);
	}
	//printf("\n%s\n", temp->value);
	st_push(head, temp->value);
}

void st_print(snode* head){
	while(head->next!=NULL){
		printf("%s-->", head->value);
		head = head->next;
	}
}

void list_gram_insert_end(list_gram** head, char* value){
	list_gram* newp = malloc(sizeof(list_gram));
	newp->value = malloc(sizeof(value));
	strcpy(newp->value, value);
	newp->next = NULL;
	//printf("reached here");
	list_gram* temp = *head;
	if((*head)==NULL){
		*head = newp;
		return;
	}
	else{
		while(temp->next!=NULL){
			temp = temp->next;
		}
	}
	temp->next = newp;
	return;
}

void list_gram_merge(list_gram** h1, list_gram** h2){
	list_gram* temp1 = *h1;
	while(temp1->next!=NULL){
		temp1 = temp1->next;
	}
	temp1->next = *h2;
	return;
}



void readgrammar(char* filename, list_gram** rules){
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
			tok = strtok(line," ");
			rules[i]->value = malloc(sizeof(tok));
			strcpy(rules[i]->value, tok);
			tok = strtok(NULL, " \n");
			while(tok!=NULL){
				if(strcmp(tok,"===>")!=0){
					//if(strcmp(tok,"<highPrecedenceOperator>")==0) printf("%s ", tok);
					list_gram_insert_end(&rules[i],tok);
				}
				tok = strtok(NULL," \n");	
			}
			i++;
		}
	}
}

void print_grammar(list_gram** rules){
	int i =0;
	for(i=0; i<num_rules; i++){
		list_gram* temp = rules[i];
		while(temp!=NULL){
			printf("%s ", temp->value);
			temp = temp->next;
		}
		printf("\n");
	}
}

int find_first_set(char* non_terminal, list_gram** rules, looktable* lt_non_terminal){
	int i=0;
	int k = lt_get(lt_non_terminal, non_terminal);
	int eflag = 1;
	int iseps = 0;
	int nt_index;
	for(i=0;i<num_rules; i++){
		eflag = 1;
		if(strcmp(non_terminal,rules[i]->value)==0){
			list_gram* temp = rules[i];
			temp = temp-> next;
			while(temp!=NULL && eflag == 1){
			if(temp->value[0]=='T'){
				if(!ifexists(k, temp->value)){
					fs[k]->arr[fs[k]->size] = strdup(temp->value);
					fs[k]->size++;
					fs[k]->valid = 1;
				}
				eflag = 0;
			}else if(temp->value[0]=='e'){
				if(!ifexists(k, temp->value)){
					fs[k]->arr[fs[k]->size] = strdup(temp->value);
					fs[k]->eflag = 1;
					fs[k]->size++;
					iseps = 1;
					fs[k]->valid = 1;
				}
				eflag = 0;
			}else if(temp->value[0] == '<'){
					nt_index = lt_get(lt_non_terminal, temp->value);
					if(fs[nt_index]->valid)
					{
						eflag = fs_add(k,fs[nt_index]);
					}
					else
					{
						eflag = find_first_set(temp->value, rules, lt_non_terminal);
						eflag = fs_add(k,fs[nt_index]);
					}
					temp=temp->next;
				}	
			}
		}
	}
	return iseps;
}

int find_follow_set(list_gram** rules, looktable* lt_non_terminal){
	int i=0;
	int eflag = 1;
	fol[0]->arr[fol[0]->size] = strdup("$");
	fol[0]->valid = 1;
	fol[0]->size++;
	list_gram* temp2;
	for(i=0; i<num_rules; i++){
		list_gram* temp = rules[i];
		temp = temp->next;
		if(temp->value[0] == 'e') continue;
		while(temp!=NULL){
			eflag = 1;
			printf("\ntemp = %s\n",temp->value);
			if(temp->value[0] == 'T') {temp = temp->next; continue;}
			int idx = lt_get(lt_non_terminal, temp->value);

			if(temp->next==NULL){
				first_set* nw = fol[lt_get(lt_non_terminal, rules[i]->value)];
				printf("\nhere\n");
				fol_add(idx, nw);
				//follow = first(rules[i])
			}
			else{
				if(temp->next->value[0] == 'T'){
					//follow = "TK_..."	
						if(!ifexists2(idx, temp->next->value)){
							printf("%s ",temp->next->value);
							fol[idx]->arr[fol[idx]->size] = strdup(temp->next->value);
							fol[idx]->valid = 1;
							fol[idx]->size++;
						}
						temp=temp->next;
				}else if(temp->next->value[0] == '<'){
					temp2 = temp->next; 
				//follow = first(temp->next->value)
					while(eflag && temp2!= NULL)
					{
						if(temp2->value[0]=='T'){
							if(!ifexists2(idx, temp2->value)){
								printf("%s ",temp2->value);
								fol[idx]->arr[fol[idx]->size] = strdup(temp2->value);
								fol[idx]->valid = 1;
								fol[idx]->size++;
							}
							break;
						}
						first_set* nw = fs[lt_get(lt_non_terminal, temp2->value)];
						eflag = fol_add(idx, nw);
						printf("eflag = %d\n",eflag);
						temp2 = temp2->next;
					}
				}	
			}
			temp = temp->next;
		}
	}
}
int ifexists(int idx, char* ter){
	int i;
	for(i=0; i< fs[idx]->size; i++){
		if(strcmp(fs[idx]->arr[i], ter)==0){
			return 1;
		}
	}
	return 0;
}

int ifexists2(int idx, char* ter){
	int i;
	for(i=0; i< fol[idx]->size; i++){
		if(strcmp(fol[idx]->arr[i], ter)==0){
			return 1;
		}
	}
	return 0;
}
int fol_add(int idx, first_set* nw){
	int fill = nw->size;
	int i;
	int j;
	j = fol[idx]->size;
	for(i=0;i<fill;i++)
	{
		if(nw->arr[i][0]!='e' && !ifexists2(idx, nw->arr[i])){
			fol[idx]->arr[j] = strdup(nw->arr[i]);
			//printf("%s ", nw->arr[i]);
			j++;
		}
	}
	fol[idx]->size = j;
	return nw->eflag;	
}
int fs_add(int idx, first_set* nw)
{
	//printf("Adding in = %d",idx);
	int fill = nw->size;
	int i;
	int j;
	j = fs[idx]->size;
	for(i=0;i<fill;i++)
	{
		if(nw->arr[i][0]!='e' && !ifexists(idx, nw->arr[i])){
			fs[idx]->arr[j] = strdup(nw->arr[i]);
			j++;
		}
	}
	fs[idx]->size = j;
	return nw->eflag;
}

void print_fs(looktable* lt_non_terminal, char* nt_string){
	int idx = lt_get(lt_non_terminal, nt_string);
	printf("\n\nprinting fs for %s\n",nt_string);
	int i;
	//printf("\nsize=%d\n",fs[idx]->size);
	for(i=0; i< fs[idx]->size; i++){
		printf("\n%s\n ", fs[idx]->arr[i]);
	}
}

void print_fol(looktable* lt_non_terminal, char* nt_string){
	int idx = lt_get(lt_non_terminal, nt_string);
	printf("\n\nprinting fol for %s\n",nt_string);
	int i;
	//printf("\nsize=%d\n",fs[idx]->size);
	for(i=0; i< fol[idx]->size; i++){
		printf("%s ", fol[idx]->arr[i]);
	}
}

int if_exists_in_rule(list_gram* temp, char* ter){
	while(temp!=NULL){
		if(temp->value[0]=='e'){
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void createParseTable(looktable* lt_non_terminal, looktable* lt_terminal, list_gram** rules){
	int i,j,k,m,p,eflag=1;
	for(i=0; i<num_rules; i++){
		list_gram* temp = rules[i];
		temp = temp->next;
		eflag = 1;
		while(temp!=NULL && eflag==1){
		if(strcmp(temp->value,"eps")==0){
			//follow
			k = lt_get(lt_non_terminal, rules[i]->value);
			for(j=0; j<fol[k]->size; j++){
				m = lt_get(lt_terminal, fol[k]->arr[j]);
				if(m!=-1) parse_table[k][m] = i;
			}
			eflag = 0;
		}
		else if(temp->value[0]=='T'){
			k = lt_get(lt_non_terminal, rules[i]->value);
			m = lt_get(lt_terminal, temp->value);
			if(m!=-1) parse_table[k][m] = i;
			eflag = 0;
		}
		else{
			//first
			k = lt_get(lt_non_terminal, rules[i]->value);
			p = lt_get(lt_non_terminal, temp->value);
			//	printf("hello %s\n", rules[i]->value);
				for(j=0; j<fs[p]->size; j++){
			//		printf("rule: %s -> %s", rules[i]->value,fs[k]->arr[j]);
					m = lt_get(lt_terminal, fs[p]->arr[j]);
			//		printf("m is %d k is %d j is %d\n", m,k,j);
					if(m!=-1) parse_table[k][m] = i;
				}
				eflag = fs[p]->eflag;
				temp = temp->next;	
			}
			
		}
	}
}

ntree* parseInputSourceCode(char* test_case_file, list_gram** rules, looktable* lt_terminal, looktable* lt_non_terminal){
	FILE *fp = fopen(test_case_file,"r");
	tokenInfo* tokenlist;
	tokenInfo* temp;
	tokenlist = getNextToken(fp);
	temp = tokenlist;
	snode* stack = malloc(sizeof(snode));
	//st_boot(&stack);
	ntree* root = malloc(sizeof(ntree));
	ntree* currNode;
	root->parent = NULL;
	root->node_symbol = strdup("<program>");
	root->consumed = 0;
	push_rule(&stack,root,rules[0]->next);
	st_print(stack);
	char* t_rec;
	int nt_i;
	int t_i;
	int ruleno;
	int a,b;
	currNode = root;
	while(stack->next!=NULL && temp!=NULL)
	{
		for(a=0;a<32;a++)
			for(b=0;b<32;b++){}
		t_rec = st_pop(&stack);
		if(temp->isvalid == 0)
		{
			if(temp->errno == 23)
				printf("Error in line no. %d:TK_ID lenght execceded\n",temp->lineno);
			else
				printf("Error in line no. %d:%s \n",temp->lineno,temp->token);
			temp = temp->next;
		}
		//printf("\nPopped = %s Token = %s\n",t_rec,temp->token);
		//st_print(stack);
		else if(strcmp(t_rec,temp->token)==0)
		{
			printf("\n%s consumed %s\n",temp->token,temp->lexeme); 
			temp = temp->next;
			printf("\nParse tree consumed %s\n",currNode->next[currNode->consumed]->node_symbol);
			currNode->consumed++;
			printf("Consumed = %d Size = %d",currNode->consumed,currNode->size);
			while(currNode->consumed>=currNode->size)
						{
							printf("\nupar gaya\n");
							currNode = currNode->parent;
						}
		}
		else if(t_rec[0] == '<')
			{
				nt_i = lt_get(lt_non_terminal,t_rec);
				t_i = lt_get(lt_terminal,temp->token);
				ruleno = parse_table[nt_i][t_i];
				if(ruleno != -1)
				{
					while(currNode->consumed>=currNode->size)
						currNode = currNode->parent;

						push_rule(&stack, currNode->next[currNode->consumed], rules[ruleno]->next);
						currNode->consumed++;
						currNode = currNode->next[(currNode->consumed)-1];
					printf("\nRule %d pushed\n",ruleno);
				}
				else
				{
					printf("Error : %s\n",temp->token);
				}
			}
		else
		{
			currNode->consumed++;
			while(currNode->consumed>=currNode->size)
							currNode = currNode->parent;
			}
	}
}

int main(){

	list_gram* rules[num_rules];
	looktable* lt_non_terminal = lt_create(128);
	looktable* lt_terminal = lt_create(128);
	char* nt_strings[50];
	int i = 0,j;
	for(i=0; i<num_rules; i++){
		rules[i] = malloc(sizeof(list_gram));
	}
	for(i=0;i<non_terminals;i++)
	{
		fs[i] = malloc(sizeof(first_set));
		fs[i]->size = 0;
		fs[i]->valid = 0;
		fs[i]->eflag = 0;
		fol[i] = malloc(sizeof(first_set));
		fol[i]->size = 0;
		fol[i]->valid = 0;
		fol[i]->eflag = 0;
	}
	readgrammar("grammarnew.txt", rules);
	//print_grammar(rules);

	FILE *file = fopen("nonterminal.txt","r");
	size_t len = 0;
	size_t nbytes = 1000;
	ssize_t read;
	char* line = malloc(nbytes+1);
	char* tok;
	i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_non_terminal, line, i);
		nt_strings[i] = strdup(line);
		i++;
	}
	fclose(file);

	for(i=0;i<50;i++){
		find_first_set(nt_strings[i], rules, lt_non_terminal);
		//print_fs(lt_non_terminal,nt_strings[i]);
	}
	//printf("\nreached here\n");
	find_follow_set(rules, lt_non_terminal);
	printf("\nreached here\n");
	for(i=0; i<50; i++) print_fol(lt_non_terminal, nt_strings[i]);
	file = fopen("terminals.txt", "r");
	i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_terminal, line, i);
		i++;
	}

	for(i=0;i<NON_TERMINAL;i++){
		for(j=0;j<TERMINAL;j++){
			parse_table[i][j] = -1;
		}
	}
	createParseTable(lt_non_terminal, lt_terminal, rules);
	parseInputSourceCode("testcase1.txt",rules, lt_terminal, lt_non_terminal);
/*	
	for(i=0;i<NON_TERMINAL;i++){
		for(j=0;j<TERMINAL;j++){
			if(parse_table[i][j] != -1)
			
			printf("%d -> %d ",i, parse_table[i][j]);
		}
		printf("\n");
	}*/
	//printf("\nParse Table value checking = %d\n",parse_table[lt_get(lt_non_terminal,"<stmts>")][lt_get(lt_terminal,"TK_READ")]);
	//printf("\n Rule = %s %s",rules[parse_table[lt_get(lt_non_terminal,"<stmts>")][lt_get(lt_terminal,"TK_READ")]]->value,rules[parse_table[lt_get(lt_non_terminal,"<dataType>")][lt_get(lt_terminal,"TK_INT")]]->next->value);
	
	
}

