#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"
void check_ret_stmt(astTree* root,sym_table* st,char* func_name, int* error)
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
				*error = 1;
			}
			else
			{
				for(i=0;i<func_table->num_output_par;i++)
				{
					if(root->children[1]->children[i]->type == -1)
					{
						compute_type(root->children[1]->children[i],st,func_name, error);
					}
					if(!(root->children[1]->children[i]->type == func_table->output_par_type[i])&& root->children[1]->children[i]->type!=-2)
					{
						printf("ERROR 878: Type mismatched in return statement with output parameters of function at lineno %d\n",root->children[1]->children[i]->lineno);
							*error = 1;
					}
				}
			}
		}
	}
}

void check_read_write_stmt(astTree* root, sym_table* st, char* func_name, int* error)
{
	compute_type(root->children[0],st,func_name, error);
	//printf("\nComputed\n");
}

void check_fun_call(astTree* root,sym_table* st,char* func_name, int* error)
{
	func_sym_table* func_table =  search_sym_table(st, root->children[1]->lexeme);
	if(func_table ==NULL)
	{
		printf("ERROR 405: Function %s not declared",root->children[1]->lexeme);
		*error = 1;
	}
	else if(strcmp(func_name,root->children[1]->lexeme)==0)
	{
		printf("ERROR 222: Recursive call\n");
		*error = 1;
	}
	else if(func_table->lineno > root->children[1]->lineno)
	{
		printf("ERROR 911: Function %s is not declared and called at line number %d\n",root->children[1]->lexeme,root->children[1]->lineno);
		*error = 1;
	}
	else
	{
		if(strcmp(root->children[0]->node_symbol,"<outputParameters>")==0 && strcmp(root->children[2]->node_symbol,"<inputParameters>")==0)
		{
			int i=0;
			if(root->children[0]->size != func_table->num_output_par)
			{
				printf("ERROR 345: Number of output parameter doesn't match at lineno %d\n",root->children[0]->children[0]->lineno);
			*error = 1;
				}
			else if(root->children[2]->size != func_table->num_input_par)
			{
				printf("ERROR 346: Number of input parameter doesn't match at lineno %d\n",root->children[2]->children[0]->lineno);
				*error = 1;
			}
			else
			{
				for(i=0;i<func_table->num_output_par;i++)
				{
					if(root->children[0]->children[i]->type == -1)
					{
						compute_type(root->children[0]->children[i],st,func_name, error);
					}
					if(!(root->children[0]->children[i]->type == func_table->output_par_type[i])&& root->children[0]->children[i]->type!=-2)
					{
						printf("ERROR 879: Type mismatched for output parameters at lineno %d\n",root->children[0]->children[i]->lineno);
							*error = 1;
					}
				}
				for(i=0;i<func_table->num_input_par;i++)
				{
					if(root->children[2]->children[i]->type == -1)
					{
						compute_type(root->children[2]->children[i],st,func_name, error);

					}
					if(!(root->children[2]->children[i]->type == func_table->input_par_type[i])&& root->children[2]->children[i]->type!=-2)
					{
						printf("ERROR 880: Type mismatched for input parameters at lineno %d type = %d\n",root->children[2]->children[i]->lineno,root->children[2]->children[i]->type);
							*error = 1;
					}
				}
			}
		}
	}

}