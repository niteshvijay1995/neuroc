#include "lookuptable.c"
#include "lexer.c"
#include "parser.h"
#define NON_TERMINAL 51
#define TERMINAL 53
char* synset[] = {"TK_SEM","TK_END","TK_RETURN","TK_ENDIF","TK_COLON"};
int synsetsize = 5;
int num_rules = 90;
int non_terminals = 51;
int num_terminals = 53; 
first_set* fs[50];
follow_set* fol[50];
int parse_table[NON_TERMINAL][TERMINAL+1];
int found[NON_TERMINAL];

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
	temp->traverse = 0;
	//if(value[0]=='T') {temp->lexeme = tok->lexeme;}
	temp->lexeme = NULL;
	temp->lineno = 0;
	temp->node_symbol = strdup(value);
	if(value[0]=='T' || value[0] == 'e') temp->is_leaf_node = 1;
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
	s_node_push(s_node, temp->value);
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


int find_fol(list_gram** rules, looktable* lt_non_terminal){
	int i=0;
	int eflag = 1;
	fol[0]->arr[fol[0]->size] = strdup("$");
	fol[0]->valid = 1;
	fol[0]->size++;
	list_gram* temp2;
	list_gram* temp;
	for(i=0; i<num_rules; i++){
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
dum_fol* compute_fol(int idx,int start)
{
//	char* fol_set[20];
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
	if(fol[idx]->folof != -1 && fol[idx]->folof != start && idx!= fol[idx]->folof)
	{
		//printf("Comuting for %d\n",fol[idx]->folof);
		df->next = compute_fol(fol[idx]->folof,start);
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
			dum_fol* dtemp;
			dtemp = compute_fol(k,k);
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
						//printf("\nsome error occured\n");
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

ntree* parseInputSourceCode(char* test_case_file, list_gram** rules, looktable* lt_terminal, looktable* lt_non_terminal, int* error){
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
	root->traverse = 0;
	push_rule(&stack,root,rules[0]->next);
	//st_print(stack);
	char* t_rec;
	int nt_i;
	int t_i;
	int ruleno;
	int lineno;
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
			if(temp->errno == 23)
				printf("ERROR_1: Identifier at line %d is longer than the prescribed length of 20 characters.\n",temp->lineno);
			else
			{
				if(t_rec[0] == '<')
					ntremflag = 1;
				printf("ERROR_3: Unknown pattern %s at line number %d\n",temp->lexeme,temp->lineno);
			}
			(*error)++;
			temp = temp->next;
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
					printf("Error_5: The token %s for lexeme %s does not match at lineNo. %d The expected token is %s\n",temp->token,temp->lexeme,temp->lineno,t_rec);
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
					printf("ERROR_5: The token of type %s for lexeme %s does not match at line number %d. The expected token is of type %s\n",temp->token,temp->lexeme,temp->lineno,t_rec);
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
					dum_fol* fols = compute_fol(nt_i,nt_i);
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
							temp = temp->next;
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
		fprintf(fp, "lexemeCurrentNode: ----,\tlineno: , token: ,"); 
		fprintf(fp, "valueIfNumber: , parentNodeSymbol: root, isLeafNode: no , NodeSymbol:%s \n", node->node_symbol);
	}
	else if(strcmp(node->node_symbol, "TK_NUM")==0 || strcmp(node->node_symbol, "TK_RNUM")==0){
		fprintf(fp,"lexemeCurrentNode: %s,\tlineno: %d, token: %s,", node->lexeme, node->lineno, node->node_symbol); 
		fprintf(fp,"valueIfNumber: %s, parentNodeSymbol: %s, isLeafNode: yes , NodeSymbol: \n", node->lexeme,node->parent->node_symbol);
	}else if((node->node_symbol)[0]=='T'){
		fprintf(fp,"lexemeCurrentNode: %s,\tlineno: %d, token: %s,", node->lexeme, node->lineno, node->node_symbol); 
		fprintf(fp,"valueIfNumber: , parentNodeSymbol: %s, isLeafNode: yes , NodeSymbol: \n",node->parent->node_symbol);
	}else if((node->node_symbol)[0]=='e'){
		fprintf(fp,"lexemeCurrentNode: ----,\tlineno: , token: ,"); 
		fprintf(fp,"valueIfNumber: , parentNodeSymbol: %s, isLeafNode: yes , NodeSymbol: %s\n", node->parent->node_symbol,node->node_symbol);
	}
	else{
		fprintf(fp,"lexemeCurrentNode: ----,\tlineno: , token: ,"); 
		fprintf(fp,"valueIfNumber: , parentNodeSymbol: %s, isLeafNode: no , NodeSymbol: %s\n", node->parent->node_symbol,node->node_symbol);
	}
}
void printParseTree(ntree* root, FILE* fp){
	if(root==NULL) return;
	if(root->size==0){
		root->is_leaf_node=1;
		printNode(root, fp);	
	} 
	if(root->traverse >=root->size) return;
	//root->traverse++;
	int i;
	for(i=0; i<root->size; i++){
		printParseTree(root->next[root->traverse++], fp);
	}
	printNode(root, fp);
}