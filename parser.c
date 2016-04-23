/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/

#include "lexer.h"
#include "parser.h"

char* synset[] = {"TK_SEM","TK_END","TK_RETURN","TK_ENDIF","TK_COLON"};
int synsetsize = 5;

void parser_init()
{
	 	lt_non_terminal	= lt_create(128);
	 	lt_terminal = lt_create(128);
		int i = 0,j;
		for(i=0; i<NUM_RULES; i++){
			rules[i] = malloc(sizeof(list_gram));
		}
		for(i=0;i<NON_TERMINAL;i++)
		{
			fs[i] = malloc(sizeof(first_set));
			fs[i]->size = 0;
			fs[i]->valid = 0;
			fs[i]->eflag = 0;
			fol[i] = malloc(sizeof(follow_set));
			fol[i]->size = 0;
			fol[i]->valid = 0;
			fol[i]->eflag = 0;
			fol[i]->folof = -1;
		}

		readgrammar("grammarnew.txt", rules);
		//modify_grammar("astRules.txt");
		//print_grammar(rules);
		readnonterminal("nonterminal.txt",lt_non_terminal,nt_strings);
		readterminal("terminals.txt",lt_terminal);
		for(i=0;i<NON_TERMINAL;i++){
			find_first_set(nt_strings[i], rules, lt_non_terminal);
		//	print_fs(lt_non_terminal,nt_strings[i]);
		}

		
		find_fol(rules, lt_non_terminal);

		for(i=0;i<NON_TERMINAL;i++){
			for(j=0;j<TERMINAL;j++){
				parse_table[i][j] = -1;
			}
		}
		/*for(i=0;i<NON_TERMINAL;i++)
		{
			int arr[100];
			dum_fol* temp = compute_fol(i,i, arr, 0);
			printf("printing fol set for %s\n", nt_strings[i]);
			while(temp!=NULL)
			{
				printf("%s ", temp->value);
				temp = temp->next;
			}
			printf("\n");
			//print_fol(lt_non_terminal, nt_strings[i]);
		}*/

		createParseTable(lt_non_terminal, lt_terminal, rules);
		//print_parse_table();
				
}


void st_push(snode** head, char* value){
	snode* newp = malloc(sizeof(snode));
	newp->value = strdup(value);
	newp->next = *head;
	*head = newp;
}

void s_node_push(ntree* root, list_gram* rule){				//Formally - void s_node_push(ntree* root,char*value)  || Changed by Nitesh Vijay on 15/04/16
	ntree* temp = malloc(sizeof(ntree));
	temp->size = 0;
	temp->consumed = 0;
	//if(value[0]=='T') {temp->lexeme = tok->lexeme;}
	temp->lexeme = NULL;
	temp->lineno = 0;
	temp->ast_value = rule->ast_value;
	temp->node_symbol = strdup(rule->value);
	if(rule->value[0]=='T' || rule->value[0] == 'e') temp->is_leaf_node = 1;	//value[0] changed to rule->value[0] by Nitesh Vijay
	else temp->is_leaf_node = 0;
	temp->parent = root;
	root->next[root->size] = temp;
	root->size++;
	//printf("\nNode %s inserted in %s at %d\n",temp->node_symbol,root->node_symbol,(root->size-1));
}
char* st_pop(snode** head){
	char* value;
	value = strdup((*head)->value);
	//printf("Value popping = %s..\n",value);
	*head = (*head)->next;
	return value;
}
char* st_front(snode* head){
	if(head==NULL) return NULL;
	else return head->value;
}

void push_rule(snode** head ,ntree* s_node, list_gram* rule ){
	list_gram* temp = rule;
	//temp = temp->next;
	s_node_push(s_node, temp);		//formally - s_node_push(s_node, temp->value);  || Chnaged by Nitesh Vijay on 15/04/16
	if(temp->next!=NULL){
		push_rule(head, s_node, temp->next);
	}
	//printf("\n%s\n", temp->value);
	st_push(head, temp->value);
}

void st_print(snode* head){
	printf("Stack = ");
	while(head->next!=NULL){
		printf("%s-->", head->value);
		head = head->next;
	}
	printf("\n");
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

void readnonterminal(char* filename,looktable* lt_non_terminal,char** nt_strings)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	size_t nbytes = 1000;
	ssize_t read;
	char* line = malloc(nbytes+1);
	char* tok;
	int i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_non_terminal, line, i);
		nt_strings[i] = strdup(line);
		i++;
	}
	fclose(file);
}

void readterminal(char* filename,looktable* lt_terminal)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	size_t nbytes = 1000;
	ssize_t read;
	char* line = malloc(nbytes+1);
	char* tok;
	int i=0;
	while((nbytes = getline(&line, &len, file)) != -1){
		int x = strlen(line);
		if(line[x-1]=='\n') line[x-1] = 0;
		//printf("iter i: %d length %d: %s ",i, x, line);
		lt_insert(lt_terminal, line, i);
		i++;
	}
	fclose(file);
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
	for(i=0; i<NUM_RULES; i++){
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
	for(i=0;i<NUM_RULES; i++){
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


int find_fol(list_gram** rules, looktable* lt_non_terminal){
	int i=0;
	int eflag = 1;
	fol[0]->arr[fol[0]->size] = strdup("$");
	fol[0]->valid = 1;
	fol[0]->size++;
	list_gram* temp2;
	list_gram* temp;
	for(i=0; i<NUM_RULES; i++){
		temp = rules[i];
		temp = temp->next;
		if(temp->value[0] == 'e') continue;
		while(temp!=NULL){
			eflag = 1;
			//printf("Rule = %s fol_set of = %s \n",rules[i]->value,temp->value);
			//printf("\ntemp = %s\n",temp->value);
			if(temp->value[0] == 'T') {temp = temp->next; continue;}
			int idx = lt_get(lt_non_terminal, temp->value);

			if(temp->next==NULL){
				//first_set* nw = fol[lt_get(lt_non_terminal, rules[i]->value)];
				//printf("\nhere\n");
				//fol_add(idx, nw);
				if(idx != lt_get(lt_non_terminal, rules[i]->value))
					fol[idx]->folof = lt_get(lt_non_terminal, rules[i]->value);
				//follow = first(rules[i])
			}
			else{
				if(temp->next->value[0] == 'T'){
					//follow = "TK_..."	
						if(!ifexists2(idx, temp->next->value)){
							//printf("%s ",temp->next->value);
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
								//printf("%s ",temp2->value);
								fol[idx]->arr[fol[idx]->size] = strdup(temp2->value);
								fol[idx]->valid = 1;
								fol[idx]->size++;
							}
							break;
						}
						//printf("Rule = %s fol_set of = %s Non T = %s\n",rules[i]->value,temp->value,temp2->value);
						first_set* nw = fs[lt_get(lt_non_terminal, temp2->value)];
						eflag = fol_add(idx, nw);
						//printf("eflag = %d\n",eflag);
						temp2 = temp2->next;
					}
					if(eflag && temp2==NULL){
						//follow = follow(rules[i])
						//first_set* nw = fol[lt_get(lt_non_terminal, rules[i]->value)];
						//printf("\nhere\n");
						//fol_add(idx, nw);
						if(idx != lt_get(lt_non_terminal, rules[i]->value))
							fol[idx]->folof = lt_get(lt_non_terminal, rules[i]->value);
						//follow = first(rules[i])
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
int inarr(int* arr, int value, int size){
	int i;
	for(i=0; i<size; i++){
		if(arr[i]==value) return 1;
	}
	return 0;
}

dum_fol* compute_fol(int idx,int start, int* arr, int size)
{
//	char* fol_set[20];
	//printf("infinte loop idx = %d, start = %d\n", idx, start);
	dum_fol* df;
	dum_fol* head;
	df = malloc(sizeof(dum_fol));
	head = df;
	int i;
	for(i=0;i<fol[idx]->size;i++)
	{
		df->next = malloc(sizeof(dum_fol));	
		df = df->next;
		df-> value = strdup(fol[idx]->arr[i]);
		df->next = NULL;
	}
	if(fol[idx]->folof != -1 && !inarr(arr, fol[idx]->folof, size) && idx!= fol[idx]->folof)
	{
		//printf("***********************Comuting for %d\n",fol[idx]->folof);
		//printf("contents of arr\n[");
		//int j;
		//for(j=0; j<size; j++) printf("%d-%d ",j, arr[i]);
		//printf("]\n");
		//if(fol[idx]->folof < 0) printf("kuch galat hai\n");
		arr[size++] = fol[idx]->folof;
		df->next = compute_fol(fol[idx]->folof,start, arr, size);
	}
	return head->next;
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
	dum_fol* temp;
	//printf("\nsize=%d\n",fs[idx]->size);
	for(i=0; i< fol[idx]->size; i++){
		//temp = compute_fol(idx,idx);
		while(temp!=NULL)
		{
			printf("%s ", temp->value);
		}
		printf("\n");
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
	for(i=0; i<NUM_RULES; i++){
		list_gram* temp = rules[i];
		temp = temp->next;
		eflag = 1;
		while(temp!=NULL && eflag==1){
		if(strcmp(temp->value,"eps")==0){
			//follow
			k = lt_get(lt_non_terminal, rules[i]->value);
			dum_fol* dtemp;
			int arr[100];
			dtemp = compute_fol(k,k, arr, 0);
			while(dtemp!=NULL)
			{
			//for(j=0; j<fol[k]->size; j++){
				m = lt_get(lt_terminal, dtemp->value);
				if(m!=-1) parse_table[k][m] = i;
				dtemp = dtemp->next;
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
					if(m!=-1) {
						parse_table[k][m] = i;

					}else{
						//printf("\nsome error occured in rule %s , %s\n", rules[i]->value, rules[i]->next->value);
					}
				}
				eflag = fs[p]->eflag;
				temp = temp->next;	
			}
			
		}
	}
}

void print_parse_table(){
	int i,j;
	for(i=0;i<NON_TERMINAL;i++){
		for(j=0;j<TERMINAL;j++){
			if(parse_table[i][j] != -1)
			{
				printf("(%d,%d) -> %d ",i+1,j+1, parse_table[i][j]);
			}
		}
		printf("\n");
	}
}

ntree* parseInputSourceCode(char* test_case_file, int* error){
	FILE *fp = fopen(test_case_file,"r");
	tokenInfo* tokenlist;
	tokenInfo* temp;
	tokenlist = getNextToken(fp);
	temp = tokenlist;
	snode* stack = malloc(sizeof(snode));
	ntree* root = malloc(sizeof(ntree));
	ntree* currNode;
	root->parent = NULL;
	root->node_symbol = strdup("<program>");
	root->consumed = 0;
	root->ast_value = 2;
	push_rule(&stack,root,rules[0]->next);
	//st_print(stack);
	char* t_rec;
	int nt_i;
	int t_i;
	int ruleno;
	int lineno;
	int vrflag = 1;
	int ntremflag = 0;
	currNode = root;
	while(stack->next!=NULL && temp->next!=NULL)
	{
		//printf("\nToken-%s\n",temp->token);
		//st_print(stack);
		t_rec = st_pop(&stack);
		lineno = temp->lineno;
		if(temp->isvalid == 0)
		{
			if(temp->errno == 1)
				printf("ERROR_1: Identifier at line %d is longer than the prescribed length of 20 characters.\n",temp->lineno);
			else if(temp->errno == 3)
			{
				if(t_rec[0] == '<')
					ntremflag = 1;
				printf("ERROR_3: Unknown pattern '%s' ascii value %d at line number %d\n",temp->lexeme,temp->lexeme[0],temp->lineno);
			}
			else if(temp->errno == 2)
			{
				printf("ERROR_2: Unknown Symbol %s at line %d\n",temp->lexeme,temp->lineno);
				st_push(&stack,t_rec);
			}
			(*error)++;
			temp = temp->next;
			vrflag = 1;
			char ch;
			//scanf(" %c ",&ch);
			//for(a=0;a<56787;a++)
			//for(b=0;b<56787;b++){}
		}
		//printf("\nPopped = %s Token = %s\n",t_rec,temp->token);
		//st_print(stack);
		else if (t_rec[0] == 'T')
			{
				if(strcmp(t_rec,temp->token)==0)
				{
					//currNode->lexeme = temp->lexeme;
					//printf("\n%s consumed %s\n",temp->token,temp->lexeme); 
					currNode->next[currNode->consumed]->lexeme = temp->lexeme;
					currNode->next[currNode->consumed]->lineno = temp->lineno;
					temp = temp->next;
					vrflag = 1;
				//	printf("\nParse tree consumed %s\n",currNode->next[currNode->consumed]->node_symbol);
					//printf("\nlexeme is %s\n", temp->lexeme);
					
					currNode->consumed++;
				//	printf("Consumed = %d Size = %d",currNode->consumed,currNode->size);
					while(currNode->consumed>=currNode->size && currNode->parent !=NULL)
								{
								//	printf("\nupar gaya\n");
									currNode = currNode->parent;
								}
				}
				else
				{
					if(ntremflag)
					{
						ntremflag = 0;
						continue;
					}
					if(vrflag)
						printf("Error_5: The token %s for lexeme %s does not match at lineNo. %d The expected token is %s\n",temp->token,temp->lexeme,temp->lineno,t_rec);
					vrflag = 0;
					(*error)++;
					char ch;
					//scanf(" %c ",&ch);

				}
			}
		else if(t_rec[0] == '<')
			{
				//printf("%s\n", );
				nt_i = lt_get(lt_non_terminal,t_rec);
				t_i = lt_get(lt_terminal,temp->token);
				ruleno = parse_table[nt_i][t_i];
				if(ruleno != -1)
				{
					while(currNode->consumed>=currNode->size)
						currNode = currNode->parent;

						push_rule(&stack, currNode->next[currNode->consumed], rules[ruleno]->next);
						//st_print(stack);
						currNode->consumed++;
						currNode = currNode->next[(currNode->consumed)-1];
					//printf("\nRule %d pushed\n",ruleno);
				}
				else
				{
					(*error)++;
					//if(vrflag)
						//printf("ERROR_5: The token of type %s for lexeme %s does not match at line number %d. The expected token is of type %s\n",temp->token,temp->lexeme,temp->lineno,t_rec);
					int s;
					int bflag = 1;
				//	printf("\nE: t_rec = %s, token = %s lineNo. %d\n", t_rec, temp->token,temp->lineno);
					for(s=0;s<synsetsize;s++)
					{
						//printf("\nentered with\n");
						t_i = lt_get(lt_terminal,synset[s]);
						ruleno = parse_table[nt_i][t_i];
						if(ruleno != -1)
						{
							if(vrflag)
								printf("ERROR_5: The token of type %s for lexeme %s does not match at line number %d. The expected token is of type %s\n",temp->token,temp->lexeme,temp->lineno,synset[s]);
							vrflag = 0;
							while(currNode->consumed>=currNode->size)
								currNode = currNode->parent;
								push_rule(&stack, currNode->next[currNode->consumed], rules[ruleno]->next);
								//st_print(stack);
								currNode->consumed++;
								currNode = currNode->next[(currNode->consumed)-1];
								bflag = 0;
						}
					}
					//t_rec = st_front(stack);
					//nt_i = lt_get(lt_non_terminal,t_rec);
					int arr[100];
					dum_fol* fols = compute_fol(nt_i,nt_i, arr, 0);
					first_set* firs = fs[nt_i];
					//printf("%s Error in line no. %d\n",t_rec,temp->lineno);
					dum_fol* temp2 = fols;
					while(temp2!=NULL)
						{
							//printf("in fol set: %s ",temp2->value);
							temp2 = temp2->next;
						}
						int i;
					tokenInfo* tok_temp2;
					tok_temp2 = temp;
					while(tok_temp2!=NULL && bflag)
					{
						while(fols!=NULL && bflag)
						{
							//printf("\nChecking\n");
							if(strcmp(fols->value,tok_temp2->token)==0)
							{
								//printf("\n\nREACHED HERE\n\n");
								temp = tok_temp2; //ask?
								vrflag = 1;
								if(vrflag)
									printf("ERROR_5: The token of type %s for lexeme %s does not match at line number %d. The expected token is of type %s\n",temp->token,temp->lexeme,temp->lineno,tok_temp2->token);
								vrflag = 0;
								//t_i = lt_get(lt_terminal,tok_temp2->token);
								//ruleno = parse_table[nt_i][t_i];
								bflag = 0;
							}
							fols = fols->next;
						}
						tok_temp2 = tok_temp2->next;
					}
					if(bflag)
						{
						//	printf(" Token skipped = %s\n",temp->token);
							printf("ERROR_5: The token of type %s for lexeme %s does not match at line number %d. The expected token is of type NOT FOUND\n",temp->token,temp->lexeme,temp->lineno);
							temp = temp->next;
							vrflag = 1;
						}
				//	printf("Error :%s - %s\n",t_rec,temp->token);
				//	char ch;
					//scanf(" %c ",&ch);
				}
			}
		else
		{
			//printf("\n %s eps\n",t_rec);
			currNode->consumed++;
			while(currNode->consumed>=currNode->size)
							currNode = currNode->parent;
			}
	}
	if(stack->next!=NULL)
	{
		(*error)++;
		printf("Error_4: Input is consumed while it is expected to have token %s at line number %d\n",st_pop(&stack),lineno+1);
	}
	return root;
}

void printNode(ntree* node, FILE* fp){
	
	if(strcmp(node->node_symbol, "<program>")==0){
		fprintf(fp, "lexemeCurrentNode: %-20s, lineno:   , token: %-20s,","----",""); 
		fprintf(fp, "valueIfNumber: %-5s, parentNodeSymbol: %-20s, isLeafNode: no  , NodeSymbol:%s \n","","root", node->node_symbol);
	}
	else if(strcmp(node->node_symbol, "TK_NUM")==0 || strcmp(node->node_symbol, "TK_RNUM")==0){
		fprintf(fp,"lexemeCurrentNode: %-20s, lineno: %-2d, token: %-20s,", node->lexeme, node->lineno, node->node_symbol); 
		fprintf(fp,"valueIfNumber: %-5s, parentNodeSymbol: %-20s, isLeafNode: yes , NodeSymbol: \n", node->lexeme,node->parent->node_symbol);
	}else if((node->node_symbol)[0]=='T'){
		fprintf(fp,"lexemeCurrentNode: %-20s, lineno: %-2d, token: %-20s,", node->lexeme, node->lineno, node->node_symbol); 
		fprintf(fp,"valueIfNumber: %-5s, parentNodeSymbol: %-20s, isLeafNode: yes , NodeSymbol: \n","",node->parent->node_symbol);
	}else if((node->node_symbol)[0]=='e'){
		fprintf(fp,"lexemeCurrentNode: %-20s, lineno:   , token: %-20s,","----",""); 
		fprintf(fp,"valueIfNumber: %-5s, parentNodeSymbol: %-20s, isLeafNode: yes , NodeSymbol: %s\n", "",node->parent->node_symbol,node->node_symbol);
	}
	else{
		fprintf(fp,"lexemeCurrentNode: %-20s, lineno:   , token: %-20s,","----",""); 
		fprintf(fp,"valueIfNumber: %-5s, parentNodeSymbol: %-20s, isLeafNode: no  , NodeSymbol: %s\n", "",node->parent->node_symbol,node->node_symbol);
	}
}
void printParseTree(ntree* root, FILE* fp){
	if(root==NULL) return;
	if(root->size==0){
		root->is_leaf_node=1;
		printNode(root, fp);	
	} 
	//if(root->traverse >=root->size) return;
	//root->traverse++;
	int i;
	for(i=0; i<root->size; i++){
		printParseTree(root->next[i], fp);
	}
	printNode(root, fp);
}

looktable* lt_create(int size){
	looktable* ht = NULL;
	if(size < 1) return ht;
	if((ht = malloc(sizeof(looktable)))==NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}
	if((ht->table = malloc(sizeof(lookentry*) * size) ) == NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}

	int i;
	for(i=0;i<size;i++)	ht->table[i] = NULL;
	ht->size = size;

	return ht;
}

lookentry* lookentry_create(char* key, int value){
	lookentry* kv; //key value pair.
	if( ( kv = malloc(sizeof(lookentry) ) ) == NULL ){
		printf("cannot allocate memory\n");
		return NULL;
	}
	
	if( ( kv->key = strdup( key ) ) == NULL ) {
		printf("cannot allocate memory\n");
		return NULL;
	}

	kv->value = value;

	kv->next = NULL;
	return kv;
}

int lt_hash_function(looktable* ht, char* key){
	unsigned long int hashval=0; //?
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}
	
	return hashval % ht->size;
}

void lt_insert(looktable* ht, char* key, int value){
	lookentry* newp = NULL, *next = NULL, *last = NULL;
	int hashval = lt_hash_function(ht,key);
	next = ht->table[hashval];
	
	while(next!=NULL && next->key!=NULL && strcmp(key, next->key) > 0 ){
		last = next;
		next = next->next;
	}
	if(next!=NULL && next->key != NULL && strcmp(key, next->key) == 0 ){
		
		next->value = value;
	
	} else {
		newp = lookentry_create(key, value);

		if(next == ht->table[hashval]){
			newp->next = next;
			ht->table[hashval] = newp;
		
		} else if (next == NULL){
			last->next = newp;
		
		} else{
			newp->next = next;
			last->next = newp;
		}
	}

	
}

int lt_get(looktable* ht, char* key){
	int hashval = lt_hash_function(ht, key);
	lookentry* newp = ht->table[hashval];
	while(newp!=NULL && newp->key!=NULL && strcmp( key, newp->key ) > 0 ){
		newp = newp->next;
	}
	if(newp == NULL || newp->key == NULL || strcmp(key,newp->key) != 0 ){
		return -1;
	}
	else{

		return newp->value;
	}
}



