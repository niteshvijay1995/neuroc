#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"

void init_typechecker(astTree* root, sym_table* st, char* func_name)
{
	int i=0,j;
	for(i=0;i<root->size;i++)
	{
		astTree* temp = root->children[i];
		if(strcmp(temp->node_symbol,"TK_FUNID")==0 || strcmp(temp->node_symbol,"TK_MAIN")==0)
			func_name = strdup(temp->lexeme);
		if(strcmp(temp->node_symbol,"TK_FUNID")==0)
		{
			temp = temp->children[2];
		}
		//printf("\nInside : %s\n",temp->node_symbol);
		for(j=0;j<temp->size;j++)
		{
			if(strcmp(temp->children[j]->node_symbol,"TK_ASSIGNOP")==0)
			{
				if(!check_assignop_type(temp->children[j],st,func_name))
				{
					printf("ERROR 440: Type mismatch for %s at line number: %d\n",temp->children[j]->node_symbol,temp->children[j]->lineno);
				}
			}
			//else if(if_bool_exp(temp->children[j]->node_symbol))
			else if(strcmp(temp->children[j]->node_symbol,"<conditionalStmt>")==0 || strcmp(temp->children[j]->node_symbol,"<iterativeStmt>")==0 )
			{
				//astTree* temp2 = temp->children[j];
				//printf("\nConditional Statement %s\n",temp2->children[1]->lexeme);

				if(!check_boolexp_type(temp->children[j]->children[0],st,func_name))
				{
					printf("ERROR 440: %s At line number: %d\n",temp->children[j]->node_symbol,temp->children[j]->children[0]->lineno);
				}
				//printf("\nConditional Statement2\n");				
				init_typechecker(temp->children[j],st,func_name);
			}
			else if(strcmp(temp->children[j]->node_symbol,"<funCallStmt>")==0)
			{
				check_fun_call(temp->children[j],st,func_name);
			}
			else if(strcmp(temp->children[j]->node_symbol,"<returnStmt>")==0 && !(strcmp(func_name,"_main")==0))
			{
				//printf("\nReturn Statement function : %s\n",func_name);
				check_ret_stmt(temp->children[j],st,func_name);
			}
		}
	}
}

void check_ret_stmt(astTree* root,sym_table* st,char* func_name)
{
	int i;
	func_sym_table* func_table =  search_sym_table(st, func_name);
	if(func_table !=NULL)
	{
		if(strcmp(root->children[1]->node_symbol,"<optionalReturn>")==0)
		{
			if(root->children[1]->size != func_table->num_output_par)
			{
				printf("ERROR 344: Number of output parameter doesn't match with return statement at lineno %d\n",root->children[1]->lineno);
			}
			else
			{
				for(i=0;i<func_table->num_output_par;i++)
				{
					if(root->children[1]->children[i]->type == -1)
					{
						compute_type(root->children[1]->children[i],st,func_name);
					}
					if(!(root->children[1]->children[i]->type == func_table->output_par_type[i])&& root->children[1]->children[i]->type!=-2)
					{
						printf("ERROR 878: Type mismatched in return statement with output parameters of function at lineno %d\n",root->children[1]->children[i]->lineno);

					}
				}
			}
		}
	}
}

void check_fun_call(astTree* root,sym_table* st,char* func_name)
{
	func_sym_table* func_table =  search_sym_table(st, root->children[1]->lexeme);
	if(func_table ==NULL)
	{
		printf("ERROR 405: Function %s not declared",root->children[1]->lexeme);
	}
	else if(strcmp(func_name,root->children[1]->lexeme)==0)
	{
		printf("ERROR 222: Recursive call\n");
	}
	else if(func_table->lineno > root->children[1]->lineno)
	{
		printf("ERROR 911: Function %s is not declared and called at line number %d\n",root->children[1]->lexeme,root->children[1]->lineno);
	}
	else
	{
		if(strcmp(root->children[0]->node_symbol,"<outputParameters>")==0 && strcmp(root->children[2]->node_symbol,"<inputParameters>")==0)
		{
			int i=0;
			if(root->children[0]->size != func_table->num_output_par)
			{
				printf("ERROR 345: Number of output parameter doesn't match at lineno %d\n",root->children[0]->children[0]->lineno);
			}
			else if(root->children[2]->size != func_table->num_input_par)
			{
				printf("ERROR 346: Number of input parameter doesn't match at lineno %d\n",root->children[2]->children[0]->lineno);
			}
			else
			{
				for(i=0;i<func_table->num_output_par;i++)
				{
					if(root->children[0]->children[i]->type == -1)
					{
						compute_type(root->children[0]->children[i],st,func_name);
					}
					if(!(root->children[0]->children[i]->type == func_table->output_par_type[i])&& root->children[0]->children[i]->type!=-2)
					{
						printf("ERROR 879: Type mismatched for output parameters at lineno %d\n",root->children[0]->children[i]->lineno);

					}
				}
				for(i=0;i<func_table->num_input_par;i++)
				{
					if(root->children[2]->children[i]->type == -1)
					{
						compute_type(root->children[2]->children[i],st,func_name);

					}
					if(!(root->children[2]->children[i]->type == func_table->input_par_type[i])&& root->children[2]->children[i]->type!=-2)
					{
						printf("ERROR 880: Type mismatched for input parameters at lineno %d type = %d\n",root->children[2]->children[i]->lineno,root->children[2]->children[i]->type);

					}
				}
			}
		}
	}

}

int if_bool_exp(char* string)
{
	if(strcmp(string, "TK_AND") == 0 || strcmp(string, "TK_OR") == 0 || strcmp(string, "TK_LT") == 0 || strcmp(string, "TK_LE") == 0 || strcmp(string, "TK_EQ") == 0 || strcmp(string, "TK_GT") == 0 || strcmp(string, "TK_GE") == 0 || strcmp(string, "TK_NE") == 0 || strcmp(string, "TK_NOT") == 0)
		return 1;
	return 0;
}

int check_assignop_type(astTree* root,sym_table* st,char* func_name)
{
	if(root->children[0]->type==-1)
	{
		compute_type(root->children[0],st,func_name);
	}
	if(root->children[1]->type==-1)
	{
		compute_type(root->children[1],st,func_name);
	}
	if(root->children[0]->type == root->children[1]->type)
	{
		root->type = root->children[0]->type;
		return 1;
	}	
	else if(root->children[0]->type == -2)
	{
		root->type = root->children[1]->type;
		return 1;
	}
	else if(root->children[1]->type == -2)
	{
		root->type = root->children[0]->type;
		return 1;
	}
	else
	{
		return 0;
	}
}
void compute_type(astTree* root, sym_table* st,char* func_name)
{
	if(strcmp(root->node_symbol,"TK_PLUS")==0||strcmp(root->node_symbol,"TK_MINUS")==0||strcmp(root->node_symbol,"TK_MUL")==0||strcmp(root->node_symbol,"TK_DIV")==0)
	{
		if(root->children[0]->type==-1)
			compute_type(root->children[0],st,func_name);
		if(root->children[1]->type==-1)
			compute_type(root->children[1],st,func_name);
		if(root->children[0]->type == root->children[1]->type)
		{
			root->type = root->children[0]->type;
			return;
		}
		else if(root->children[0]->type == -2)
		{
			root->type = root->children[1]->type;
			return;
		}
		else if(root->children[1]->type == -2)
		{
			root->type = root->children[0]->type;
			return;
		}
		else
		{
			printf("ERROR_x : Type mismatch at line number: %d (Types - %d,%d)\n",root->lineno,root->children[0]->type,root->children[1]->type);
			root->type = -2;
			return;
		}
	}
	else if(strcmp(root->node_symbol,"TK_NUM")==0)
	{
		root->type = 0;
		return;
	}
	else if(strcmp(root->node_symbol,"TK_RNUM")==0)
	{
		root->type = 1;
		return;
	}
	else if(strcmp(root->node_symbol,"<singleOrRecId>")==0)
	{
		symbol_table_lookup(root, st,func_name);
		//printf("\nSymbol Table Done\n");
		return;
	}
	else if(strcmp(root->node_symbol,"TK_ID")==0)
	{
		id_symbol_table_lookup(root, st,func_name);
	}
	else if(if_bool_exp(root->node_symbol))
	{
		check_boolexp_type(root,st,func_name);
	}
}

void id_symbol_table_lookup(astTree* root,sym_table* st,char* func_name)
{
	//printf("\n Looking up symbol table\n");
	func_sym_table* func_table =  search_sym_table(st, func_name);
	details* d =  func_sym_get(func_table, root->lexeme);
	if(d==NULL)
	{
		func_sym_table* g_table = search_sym_table(st, "global");
		d =  func_sym_get(g_table, root->lexeme);
	}
	if(d!=NULL)
	{
		//if (root->lineno > d->lineno)
		root->type = d->type;
		return;
	}
	else
	{
		printf("ERROR 555: Undeclared variable %s , node_symbol: %s, used at line number: %d\n",root->lexeme, root->node_symbol,root->lineno);
		root->type = -2;
		return;
	}
}

void symbol_table_lookup(astTree* root,sym_table* st,char* func_name)
{
	//printf("\n Looking up symbol table of func : %s\n",func_name);
	func_sym_table* func_table =  search_sym_table(st, func_name);
	details* d =  func_sym_get(func_table, root->children[0]->lexeme);
	if(d==NULL)
	{
		func_sym_table* g_table = search_sym_table(st, "global");
		d =  func_sym_get(g_table, root->children[0]->lexeme);
	}
	if(d!=NULL)
	{
		root->children[0]->type = d->type;
		if(d->type == 2 && root->children[1] != NULL)
		{
			//	printf("\nREC_ID detected %s\n",d->rec_name);
			details* d2 =  func_sym_get(func_table, d->rec_name);
			//printf("\nREC_ID detected\n");
			details* d3 =  func_sym_get(d2->f, root->children[1]->lexeme);
			if(d3 !=NULL)
			{
				root->type = d3->type;
				return;
			}
			else
			{
				printf("ERROR 88: Undeclared record variable %s used at line number: %d\n",root->lexeme,root->lineno);
				root->type = -2;
				return;
			}
		}
		else
		{
			root->type = d->type;
			return;
		}
	}
	else
	{
		printf("ERROR: Undeclared variable %s used at line number: %d\n",root->children[0]->lexeme,root->children[0]->lineno);
		root->type = -2;
		return;
	}
}
int check_boolexp_type(astTree* root,sym_table* st,char* func_name)
{
	//printf("\nNode Symbol = %s\n",root->node_symbol);
	if(root->children[0]->type == -1)
		compute_type(root->children[0],st,func_name);
	if(root->children[1]->type == -1)
		compute_type(root->children[1],st,func_name);
	if(strcmp(root->node_symbol,"TK_AND")==0 || strcmp(root->node_symbol,"TK_OR")==0)
	{
		if((root->children[0]->type == 5 || root->children[0]->type == -2) && (root->children[1]->type == 5 || root->children[1]->type == -2))
		{
			root->type =5;
			return 1;
		}
		else
		{
			root->type = -2;
			printf("ERROR 520: Cannot perform Boolean operation on non Boolean types.\n ");
			return 1;
		}
	}
	else
	{
		if(root->children[0]->type == root->children[1]->type && (root->children[0]->type == 0 || root->children[0]->type == 1))
		{
			root->type =5;
			return 1;
		}
		else if(root->children[0]->type == -2)
		{
			root->type = root->children[1]->type;
			return 1;
		}
		else if(root->children[1]->type == -2)
		{
			root->type = root->children[0]->type;
			return 1;
		}
		else
		{
			root->type = -2;
			printf("ERROR 123: Type Mismatch for Comparision statement at line number %d (%d, %d)",root->lineno,root->children[0]->type,root->children[1]->type);
			return 1;
		}
	}
}

void code_gen(astTree* root,sym_table* st, symbol_list* lis){
	printf("global  main\nextern  printf\nextern scanf\nSECTION .data\nformatin: db \"%%d\", 0\nformatout: db \"%%d\", 10, 0\n");
	symbol_list* temp = lis;
	while(temp->next!=NULL)
	{
		func_sym_table* f = search_sym_table(st, temp->func_name);
		if(f==NULL){
			temp = temp->next;
			continue;
		}
		details* d = func_sym_get(f, temp->lexeme);		
	//	printf("\nlexeme: %s, function_name: %s, type: %d, offset: %d, lineno: %d \n", temp->lexeme, temp->func_name, d->type, d->offset, d->lineno);
		
		if(d->type==0)
		{
			printf("%s: times 4 db 0\n",temp->lexeme);
		}

	/*	if(d->type==3)
		{
			symbol_list* temp2 = d->slist;

			while(temp2!=NULL)
			{
				details* d2 = func_sym_get(d->f, temp2->lexeme);
				if(d2!=NULL) 
				printf("\nlexeme: %s, function_name: %s, type: %d, offset: %d\n", temp2->lexeme, temp2->func_name, d2->type, d2->offset);
				temp2 = temp2->next;
			}
		}*/
		temp = temp->next;
	}
	printf("SECTION .text\nmain:\n");
	astTree* temp_tree = root->children[0];		//_main function
	int i;
	printf("push ebx\npush ecx\n");
	for(i=0;i<temp_tree->size;i++)
	{
		if(strcmp(temp_tree->children[i]->node_symbol,"TK_ASSIGNOP")==0)
		{
			evaluate(temp_tree->children[i]);
		}
		if(strcmp(temp_tree->children[i]->node_symbol,"TK_READ")==0)
		{
			printf("push %s\npush formatin\ncall scanf\nadd esp,8\n",temp_tree->children[i]->children[0]->lexeme);
		}
		if(strcmp(temp_tree->children[i]->node_symbol,"TK_WRITE")==0)
		{
			printf("mov ebx,[%s]\npush ebx\npush formatout\ncall printf\nadd esp,8\n",temp_tree->children[i]->children[0]->lexeme);
		}
	}
	printf("pop ecx\npop ebx\nmov eax, 0\nret\n");

}

void evaluate(astTree* root)
{
	astTree* temp = root->children[1]; 		//right side of assignop
	if(strcmp(temp->node_symbol,"TK_NUM")==0)
	{
		printf("mov ebx,%s\n",temp->lexeme);
	}
	else if(strcmp(temp->node_symbol,"TK_ID")==0)
	{
		printf("mov ebx,[%s]\n",temp->lexeme);
	}
	else if(strcmp(temp->node_symbol,"TK_PLUS")==0)
	{
		eval(temp,"ebx");
	}
	printf("mov [%s],ebx\n",root->children[0]->children[0]->lexeme);
}
void eval(astTree* root,char* reg)
{
	if(strcmp(root->node_symbol,"TK_PLUS")==0)
	{
		eval(root->children[1],reg2(reg));
		eval(root->children[0],reg);
		printf("add %s,%s\n",reg,reg2(reg));
		return;
	}
	else if(strcmp(root->node_symbol,"TK_MINUS")==0)
	{
		eval(root->children[1],reg2(reg));
		eval(root->children[0],reg);
		printf("sub %s,%s\n",reg,reg2(reg));
		return;
	}
	else if(strcmp(root->node_symbol,"TK_ID")==0)
	{
		printf("mov %s,[%s]\n",reg,root->lexeme);
		return;
	}
}
char* reg2(char* reg)
{
	if(strcmp(reg,"ebx")==0)
	{
		return "ecx";
	}
	else
	{
		return "ebx";
	}
}