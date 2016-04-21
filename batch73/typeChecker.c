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
		//printf("\nInside : %s\n",temp->node_symbol);
		for(j=0;j<temp->size;j++)
		{
			if(strcmp(temp->children[j]->node_symbol,"TK_ASSIGNOP")==0)
			{
				if(!check_assignop_type(temp->children[j],st,func_name))
				{
					printf("\nERROR 440: %s At line number: %d\n",temp->children[j]->node_symbol,temp->children[j]->lineno);
				}
			}
			//else if(if_bool_exp(temp->children[j]->node_symbol))
			else if(strcmp(temp->children[j]->node_symbol,"<conditionalStmt>")==0 || strcmp(temp->children[j]->node_symbol,"<iterativeStmt>")==0 )
			{
				//astTree* temp2 = temp->children[j];
				//printf("\nConditional Statement %s\n",temp2->children[1]->lexeme);

				if(!check_boolexp_type(temp->children[j]->children[0],st,func_name))
				{
					printf("\nERROR 440: %s At line number: %d\n",temp->children[j]->node_symbol,temp->children[j]->children[0]->lineno);
				}
				//printf("\nConditional Statement2\n");				
				init_typechecker(temp->children[j],st,func_name);
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
	if(d!=NULL)
	{
		root->type = d->type;
		return;
	}
	else
	{
		printf("ERROR: Undeclared variable %s used at line number: %d\n",root->lexeme,root->lineno);
		root->type = -2;
		return;
	}
}

void symbol_table_lookup(astTree* root,sym_table* st,char* func_name)
{
	//printf("\n Looking up symbol table of func : %s\n",func_name);
	func_sym_table* func_table =  search_sym_table(st, func_name);
	details* d =  func_sym_get(func_table, root->children[0]->lexeme);
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
		printf("ERROR: Undeclared variable %s used at line number: %d\n",root->lexeme,root->lineno);
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