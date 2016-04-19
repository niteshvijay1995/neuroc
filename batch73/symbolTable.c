#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "symbolTable.h"

int global_offset = 0;

void push_symbol_list(symbol_list** lis, char* lexeme, char* func_name){
	symbol_list* new_node = malloc(sizeof(symbol_list));
	new_node->lexeme = strdup(lexeme);
	new_node->func_name = strdup(func_name);
	new_node->next = *lis;
	*lis = new_node;
}

sym_table* push_sym_table(sym_table* head, func_sym_table* data){
	sym_table* new_node = malloc(sizeof(sym_table));
	new_node->f = data;
	new_node->next = head;
	head = new_node;
	return head; 
}

func_sym_table* sym_create(int size, char* name){
	func_sym_table* ht = NULL;

	if(size < 1) return ht;

	if((ht = malloc(sizeof(func_sym_table)))==NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}
	if((ht->table = malloc(sizeof(var_entry*) * size) ) == NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}
	ht->func_name = strdup(name);

	int i;
	for(i=0;i<size;i++)	ht->table[i] = NULL;
	ht->size = size;

	return ht;
}

void copy_details(details* d1, details* d2){
	d1->type = d2->type;
	d1->offset = d2->offset;
	if(d1->type==3)
	{
		d1->rec_name = strdup(d2->rec_name);
		symbol_list* temp = d2->slist;
		d1->f = d2->f;
		d1->slist = d2->slist;
	}
	d1->lineno = d2->lineno;
}

var_entry* var_entry_create(char* key, details* det){
	var_entry* kv; //key value pair.
	if( ( kv = malloc(sizeof(var_entry) ) ) == NULL ){
		printf("cannot allocate memory\n");
		return NULL;
	}
	
	if( ( kv->lexeme = strdup( key ) ) == NULL ) {
		printf("cannot allocate memory\n");
		return NULL;
	}
	kv->det = malloc(sizeof(details));
	copy_details(kv->det,det);
	kv->next = NULL;
	return kv;
}

int sym_hash_function(func_sym_table* ht, char* key){
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

void func_sym_insert(func_sym_table* ht, char* key, details* value){
	var_entry* newp = NULL, *next = NULL, *last = NULL;
	int hashval = sym_hash_function(ht,key);
	next = ht->table[hashval];
	
	while(next!=NULL && next->lexeme!=NULL && strcmp(key, next->lexeme) > 0 ){
		last = next;
		next = next->next;
	}
	if(next!=NULL && next->lexeme != NULL && strcmp(key, next->lexeme) == 0 ){
		
		free( next->det );
		next->det = malloc(sizeof(details));
		copy_details(next->det, value);
	
	} else {
		newp = var_entry_create(key, value);

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

details* func_sym_get(func_sym_table* ht, char* key){
	int hashval = sym_hash_function(ht, key);
	var_entry* newp = ht->table[hashval];
	while(newp!=NULL && newp->lexeme!=NULL && strcmp( key, newp->lexeme ) > 0 ){
		newp = newp->next;
	}
	if(newp == NULL || newp->lexeme == NULL || strcmp(key,newp->lexeme) != 0 ){
		return NULL;
	}
	else{

		return newp->det;
	}
}

void createVarEntry(func_sym_table* f, char* lexeme, details* d, symbol_list** lis){
	
	if(func_sym_get(f, lexeme)!=NULL){
		printf("error. variable %s is redeclared", lexeme);
	}
	func_sym_insert(f, lexeme, d);
	//printf("a variable entry is created\n");
	push_symbol_list(lis, lexeme, f->func_name);
	
}

void allocate(astTree* temp, int* offset, func_sym_table* f, func_sym_table* g, looktable* lt_rec, symbol_list** lis){
	int j = 0;
	int off = 0;
	for(j=0; j<=temp->size-2; j+=2){
		details* d = malloc(sizeof(details));
		if(strcmp(temp->children[j]->node_symbol, "TK_INT")==0){
			temp->children[j]->type = 0;
			temp->children[j+1]->type = 0;
			d->type = 0;
			d->lineno = temp->children[j]->lineno;
			off = 2;
		}
		else if(strcmp(temp->children[j]->node_symbol, "TK_REAL")==0){
			temp->children[j]->type = 1;
			temp->children[j+1]->type = 1;
			d->type = 1;
			d->lineno = temp->children[j]->lineno;
			off = 2; 
		}
		else if(strcmp(temp->children[j]->node_symbol, "TK_RECORDID")==0){
			temp->children[j]->type = 2;
			temp->children[j+1]->type = 2;
			d->type = 2;
			d->lineno = temp->children[j]->lineno;
			off = lt_get(lt_rec, temp->children[j]->lexeme);
			//printf("\noff is: %d, lexeme searched was: %s\n", off, temp->children[j]->lexeme);
		}
		d->offset = *offset + global_offset;

		if(temp->size == 3){
			global_offset += off;
			createVarEntry(g, temp->children[j+1]->lexeme, d, lis);
		}
		else{
			(*offset) = *offset + off;
			createVarEntry(f,temp->children[j+1]->lexeme,d, lis);
			//printf("\n ***lexeme = %s\n",temp->children[j+1]->lexeme);
		}	
	}

}

iterate_type_def(details** d, astTree* root){
	int i;
	int offset = 0;
	symbol_list* temp = malloc(sizeof(symbol_list));
	for(i=0; i<root->size; i++)
	{
		allocate(root->children[i], &offset, (*d)->f, NULL, NULL, &temp);
	}
	(*d)->slist = temp;
}

func_sym_table* createMainFuncEntry(astTree* root, char* name, func_sym_table* g, symbol_list** lis){
	func_sym_table* f = sym_create(1024, name);
	looktable* lt_rec = lt_create(1024);
	int i;
	int offset = 0;
	for(i=0; i<root->size; i++){
		if(strcmp(root->children[i]->node_symbol, "<typeDefinition>")==0)
		{
			astTree* temp = root->children[i];
			int value = temp->children[1]->size * 2;		//considering int and real of size 2 
		//	printf("\nlexeme inserted is: %s\n", temp->children[0]->lexeme);
			details* d2 = malloc(sizeof(details));
			d2->type = 3;
			d2->offset = offset;
			d2->lineno = temp->children[0]->lineno;  
			offset = offset+=value;
			d2->f = sym_create(32, temp->children[0]->lexeme);
			temp->type = 3;
			temp->children[0]->type = 3;
			iterate_type_def(&d2, temp->children[1]);
			//if(d2->slist == NULL) printf("nulla ho gaya\n");
			createVarEntry(f,temp->children[0]->lexeme,d2, lis);
			

			//func_sym_insert(f, )

			lt_insert(lt_rec, temp->children[0]->lexeme, value);
			
		}
		if(strcmp(root->node_symbol, "TK_MAIN")==0){
			if(strcmp(root->children[i]->node_symbol, "<declaration>")==0)
			{	
				astTree* temp = root->children[i];
				allocate(temp,&offset,f, g, lt_rec, lis);
			}
		}
		else if(strcmp(root->node_symbol,"TK_FUNID")==0)
		{
			astTree* temp1;
			temp1 = root->children[i];

			if(i<2){
				allocate(temp1, &offset,f, g, lt_rec, lis);
			}
			else if(i==2){
				int j;
				for(j=0; j<temp1->size; j++){
				
					if(strcmp(temp1->children[j]->node_symbol, "<declaration>")==0)
					{	
						astTree* temp3 = temp1->children[j];
						allocate(temp3,&offset,f, g, lt_rec, lis);
					}			
				}
			}
		}
	}
	return f;
}

sym_table* createSymbolTable(astTree* root, symbol_list** lis){
	sym_table* st;
	int i;
	func_sym_table* g = malloc(sizeof(func_sym_table));

	g = sym_create(1024, "global");

	for(i=0; i<root->size; i++)
	{
		func_sym_table* f = createMainFuncEntry(root->children[i], root->children[i]->lexeme, g, lis);
		//printf("\none entry created\n");
		st = push_sym_table(st,f);
	}
	st = push_sym_table(st, g);
	return st;
}

func_sym_table* search_sym_table(sym_table* st, char* name){
	sym_table* temp = st;
	while(temp!=NULL){
		if(strcmp(temp->f->func_name, name)==0)
			return temp->f;
		temp = temp->next;
	}
	return NULL;
}

void printSymbolTable(sym_table* st, symbol_list* lis){
	symbol_list* temp = lis;
	while(temp->next!=NULL)
	{
		//printf("\nin printSymbolTable func name = %s\n",temp->func_name);
		func_sym_table* f = search_sym_table(st, temp->func_name);
		//printf("in printSymbolTable\n");
		if(f==NULL){
			temp = temp->next;
			continue;
		}
		//printf("in printSymbolTable\n");
		details* d = func_sym_get(f, temp->lexeme);
		///printf("in printSymbolTable\n");

		printf("\nlexeme: %s, function_name: %s, type: %d, offset: %d, lineno: %d\n", temp->lexeme, temp->func_name, d->type, d->offset, d->lineno);
		if(d->type==3)
		{
			symbol_list* temp2 = d->slist;

			while(temp2!=NULL)
			{
				details* d2 = func_sym_get(d->f, temp2->lexeme);
				if(d2!=NULL) 
				printf("\nlexeme: %s, function_name: %s, type: %d, offset: %d\n", temp2->lexeme, temp2->func_name, d2->type, d2->offset, d2->lineno);
				temp2 = temp2->next;
			}
		}
		temp = temp->next;
	}
}

/*
void init_typechecker(astTree* root, sym_table* st)
{
	int i=0,j;
	for(i=0;i<root->size;i++)
	{

		astTree* temp = root->children[i];
		for(j=0;j<temp->size;j++)
		{
			if(strcmp(temp->children[j]->node_symbol,"TK_ASSIGNOP")==0)
			{
				check_assignop_type(temp->children[j]);	
			}
		}
		if(strcmp(root->children[i]->node_symbol,"TK_MAIN")==0)
		{

		}
	}
}
/*
int check_assignop_type(astTree* root)
{
	root->
}*/