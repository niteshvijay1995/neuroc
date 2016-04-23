#include "parser.h"
#include "ast.h"
#include "symbolTable.h"
#include "typeChecker.h"
#include "codegen.h"


void code_gen(astTree* root,sym_table* st, symbol_list* lis, FILE* fp){
	fprintf(fp,"global  main\nextern  printf\nextern scanf\nSECTION .data\nformatin: db \"%%d\", 0\nformatout: db \"%%d\", 10, 0\n");
	symbol_list* temp = lis;
	func_sym_table* g_table = search_sym_table(st, "global");
	while(temp!=NULL)
	{
		func_sym_table* f = search_sym_table(st, temp->func_name);
		if(f==NULL){
			temp = temp->next;
			continue;
		}
		details* d = func_sym_get(f, temp->lexeme);		
		if(d->type==0)
		{
			fprintf(fp,"%s: times 4 db 0\n",temp->lexeme);
		}
		if(d->type==2)
		{

			details* d3 = func_sym_get(g_table, d->rec_name);
			symbol_list* temp2 = d3->slist;
			while(temp2!=NULL)
			{
				details* d2 = func_sym_get(d3->f, temp2->lexeme);
				if(d2!=NULL) 
				{
					//printf("\nlexeme: %s, function_name: %s, type: %d, offset: %d\n", temp2->lexeme, temp2->func_name, d2->type, d2->offset);
					if(d2->type == 0)
						fprintf(fp,"%s_%s: times 4 db 0\n",temp->lexeme,temp2->lexeme);
				}
				temp2 = temp2->next;
			}
		}
		temp = temp->next;
	}
	fprintf(fp,"SECTION .text\nmain:\n");
	astTree* temp_tree = root;
	int i;
	fprintf(fp,"push ebx\npush ecx\npush eax\n");
	code_gen1(temp_tree,st,g_table,fp);
	fprintf(fp,"pop ecx\npop ebx\npop eax\nmov eax, 0\nret\n");

}

void code_gen1(astTree* root,sym_table* st,func_sym_table* g_table,FILE* fp)
{
	int i;
	for(i=0;i<root->size;i++)
	{
		if(strcmp(root->children[i]->node_symbol,"TK_ASSIGNOP")==0 && root->children[i]->type ==2)
		{
			evaluate_record(root->children[i],st,g_table, fp);
		}
		else if(strcmp(root->children[i]->node_symbol,"TK_ASSIGNOP")==0)
		{
			//printf("\nAssign op at line %d type %d\n",root->children[i]->lineno,root->children[i]->type);
			evaluate(root->children[i],g_table, fp);
		}
		if(strcmp(root->children[i]->node_symbol,"TK_READ")==0)
		{
			if(root->children[i]->children[0]->size==1)
				fprintf(fp,"push %s\npush formatin\ncall scanf\nadd esp,8\n",root->children[i]->children[0]->children[0]->lexeme);
			else if(root->children[i]->children[0]->size==2)
				fprintf(fp,"push %s_%s\npush formatin\ncall scanf\nadd esp,8\n",root->children[i]->children[0]->children[0]->lexeme,root->children[i]->children[0]->children[1]->lexeme);
		}

		if(strcmp(root->children[i]->node_symbol,"TK_WRITE")==0 && root->children[i]->children[0]->type ==2)
		{
			print_rec(root->children[i],st,g_table, fp);
		}

		else if(strcmp(root->children[i]->node_symbol,"TK_WRITE")==0)
		{
			//printf("\nWrite at lineno %d type %d\n",root->children[i]->lineno,root->children[i]->children[0]->children[0]->type);
			if(root->children[i]->children[0]->size==1)
				fprintf(fp,"mov ebx,[%s]\npush ebx\npush formatout\ncall printf\nadd esp,8\n",root->children[i]->children[0]->children[0]->lexeme);
			else
				fprintf(fp,"mov ebx,[%s_%s]\npush ebx\npush formatout\ncall printf\nadd esp,8\n",root->children[i]->children[0]->children[0]->lexeme,root->children[i]->children[0]->children[1]->lexeme);
		}
		if(strcmp(root->children[i]->node_symbol,"<conditionalStmt>")==0)
		{
			cond_stmt(root->children[i],st,g_table,fp);
		}
		if(strcmp(root->children[i]->node_symbol,"<iterativeStmt>")==0)
		{
			iter_stmt(root->children[i],st,g_table,fp);
		}
	}
}

void print_rec(astTree* root,sym_table* st,func_sym_table* g_table, FILE* fp)
{
	printf("\nPrinting Record %s\n",root->children[0]->node_symbol);
	func_sym_table* f_table = search_sym_table(st, "_main");
	details* d3 = func_sym_get(f_table, root->children[0]->children[0]->lexeme);
	printf("\nPrinting Record\n");
	d3 = func_sym_get(g_table,d3->rec_name);
	symbol_list* temp2 = d3->slist;
	while(temp2!=NULL)
		{
			details* d2 = func_sym_get(d3->f, temp2->lexeme);
			if(d2!=NULL) 
			{
				if(d2->type == 0)
					fprintf(fp,"mov ebx,[%s_%s]\npush ebx\npush formatout\ncall printf\nadd esp,8\n",root->children[0]->children[0]->lexeme,temp2->lexeme);
				//	fprintf(fp, "mov [%s_%s],ebx\n", root->children[0]->children[0]->lexeme,temp2->lexeme);
			}
			temp2 = temp2->next;
		}
}

void eval_rec(astTree* root,char* fid,FILE* fp)
{
	if(strcmp(root->node_symbol,"TK_PLUS")==0)
	{
		eval_rec(root->children[1],fid,fp);
		fprintf(fp,"PUSH ebx\n");
		eval_rec(root->children[0],fid, fp);
		fprintf(fp,"POP ecx\n");
		fprintf(fp,"add ebx,ecx\n");
		return;
	}
	else if(strcmp(root->node_symbol,"TK_MINUS")==0)
	{
		eval_rec(root->children[1],fid, fp);
		fprintf(fp,"PUSH ebx\n");
		eval_rec(root->children[0],fid, fp);
		fprintf(fp,"POP ecx\n");
		fprintf(fp,"sub ebx,ecx\n");
		return;
	}
	if(strcmp(root->node_symbol,"TK_MUL")==0)
	{
		eval_rec(root->children[1],fid,fp);
		fprintf(fp,"PUSH ebx\n");
		eval_rec(root->children[0],fid,fp);
		fprintf(fp,"POP eax\n");
		fprintf(fp,"imul ebx\n");
		fprintf(fp,"mov ebx,eax\n");
		return;
	}
	if(strcmp(root->node_symbol,"TK_DIV")==0)
	{
		eval_rec(root->children[1],fid,fp);
		fprintf(fp,"PUSH ebx\n");
		eval_rec(root->children[0],fid,fp);
		fprintf(fp,"mov eax,ebx\n");
		fprintf(fp,"POP ebx\n");
		fprintf(fp,"idiv ebx\n");
		fprintf(fp,"mov ebx,eax\n");
		return;
	}
	else if(strcmp(root->node_symbol,"<singleOrRecId>")==0)
	{
		if(root->size==1)
		{
			fprintf(fp,"mov ebx,[%s_%s]\n",root->children[0]->lexeme,fid);
			return;
		}
	}
}

void evaluate_record(astTree* root,sym_table* st,func_sym_table* g_table, FILE* fp)
{
	//printf("\nEvaluating record %s lineno : \n",root->children[0]->children[0]->lexeme);
	func_sym_table* f_table = search_sym_table(st, "_main");
	if(root->children[0]->type==2)		//record
	{
		details* d3 = func_sym_get(f_table, root->children[0]->children[0]->lexeme);
		d3 = func_sym_get(g_table,d3->rec_name);
		symbol_list* temp2 = d3->slist;
		while(temp2!=NULL)
		{
			details* d2 = func_sym_get(d3->f, temp2->lexeme);
			if(d2!=NULL) 
			{
				if(d2->type == 0)
					eval_rec(root->children[1],temp2->lexeme, fp);
					//fprintf(fp, "mov ebx,[%s_%s]\n", root->children[0]->children[0]->lexeme,temp2->lexeme );
					fprintf(fp, "mov [%s_%s],ebx\n", root->children[0]->children[0]->lexeme,temp2->lexeme);
			}
			temp2 = temp2->next;
		}
		return;
	}

}

void iter_stmt(astTree* root,sym_table *st,func_sym_table* g_table,FILE *fp)
{
	fprintf(fp, "label_%d:\n",root->children[0]->lineno );
	cond_eval(root->children[0],fp);
	fprintf(fp, "cmp ebx,1\n");
	fprintf(fp, "jne label_out_%d\n",root->children[0]->lineno);
	code_gen1(root->children[1],st,g_table,fp);
	fprintf(fp, "jmp label_%d\n",root->children[0]->lineno);
	fprintf(fp, "label_out_%d:\n",root->children[0]->lineno);
}

void cond_stmt(astTree* root,sym_table* st,func_sym_table* g_table,FILE* fp)
{
	int r = rand()%100;
	cond_eval(root->children[0],fp);
	fprintf(fp, "cmp ebx,1\n");
	fprintf(fp, "jne else_%d_%d\n",root->lineno,r);
	fprintf(fp, "jmp then_%d_%d\n",root->lineno,r);
	fprintf(fp, "then_%d_%d:\n",root->lineno,r );
	//fprintf(fp,"%s\n",toStr(A->next[3]->t->s));
	code_gen1(root->children[1],st,g_table,fp);
	fprintf(fp, "jmp cont_%d_%d\n",root->lineno,r );
	fprintf(fp, "else_%d_%d:\n",root->lineno,r);
	code_gen1(root->children[2],st,g_table,fp);
	fprintf(fp, "cont_%d_%d:\n",root->lineno,r );

}

void cond_eval(astTree* root,FILE* fp)
{
	if(strcmp(root->node_symbol,"TK_AND")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp,"and ebx,ecx\n");
	}
	else if(strcmp(root->node_symbol,"TK_OR")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp,"or ebx,ecx\n");
	}
	else if(strcmp(root->node_symbol,"TK_GT")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ecx,ebx\n");
		int r =  rand()%10000;
		fprintf(fp, "jg true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );


	}
	else if(strcmp(root->node_symbol,"TK_GE")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ecx,ebx\n");
		int r =  rand()%10000;
		fprintf(fp, "jge true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );
	}
	else if(strcmp(root->node_symbol,"TK_EQ")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ebx,ecx\n");
		int r =  rand()%10000;
		fprintf(fp, "je true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );
	}
	else if(strcmp(root->node_symbol,"TK_NE")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ebx,ecx\n");
		int r =  rand()%10000;
		fprintf(fp, "jne true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );
	}
	else if(strcmp(root->node_symbol,"TK_LE")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ecx,ebx\n");
		int r =  rand()%10000;
		fprintf(fp, "jle true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );
	}
	else if(strcmp(root->node_symbol,"TK_LT")==0)
	{
		cond_eval(root->children[0],fp);
		fprintf(fp, "push ebx\n");
		cond_eval(root->children[1],fp);
		fprintf(fp, "pop ecx\n");
		fprintf(fp, "cmp ecx,ebx\n");
		int r =  rand()%10000;
		fprintf(fp, "jl true_%d_%d\n",root->lineno,r);
		fprintf(fp, "jmp false_%d_%d\n",root->lineno,r);
		fprintf(fp, "true_%d_%d:\nmov ebx, 1\njmp resume_%d_%d\n",root->lineno,r,root->lineno,r );
		fprintf(fp, "false_%d_%d:\nmov ebx, 0\nresume_%d_%d:\n",root->lineno,r,root->lineno,r );

	}
	else if(strcmp(root->node_symbol,"TK_ID")==0)
	{
		fprintf(fp, "mov ebx,[%s]\n",root->lexeme);
	}
	else if(strcmp(root->node_symbol,"TK_NUM")==0)
	{
		fprintf(fp, "mov ebx,%s\n",root->lexeme );
	}
}

void evaluate(astTree* root,func_sym_table* g_table, FILE* fp)
{
	astTree* temp = root->children[1]; 		//right side of assignop
	if(strcmp(temp->node_symbol,"TK_NUM")==0)
	{
		fprintf(fp,"mov ebx,%s\n",temp->lexeme);
	}
	else if(strcmp(temp->node_symbol,"<singleOrRecId>")==0)
	{
		if(temp->size==1)
		{
			if(temp->children[0]->type==0)
				fprintf(fp,"mov ebx,[%s]\n",temp->children[0]->lexeme);
		}
		else
		{
			fprintf(fp,"mov ebx,[%s_%s]\n",temp->children[0]->lexeme,temp->children[1]->lexeme);
		}
	}
	/*else if(strcmp(temp->node_symbol,"TK_ID")==0)			//depreciated
	{
		fprintf(fp,"mov ebx,[%s]\n",temp->lexeme);
	}*/
	else if(strcmp(temp->node_symbol,"TK_PLUS")==0)
	{
		eval(temp, fp);
	}
	else if(strcmp(temp->node_symbol,"TK_MINUS")==0)
	{
		eval(temp, fp);
	}
	else if(strcmp(temp->node_symbol,"TK_MUL")==0)
	{
		eval(temp, fp);
	}
	else if(strcmp(temp->node_symbol,"TK_DIV")==0)
	{
		eval(temp, fp);
	}
	if(root->children[0]->size==1)
		fprintf(fp,"mov [%s],ebx\n",root->children[0]->children[0]->lexeme);
	else if(root->children[0]->size==2)
		fprintf(fp,"mov [%s_%s],ebx\n",root->children[0]->children[0]->lexeme,root->children[0]->children[1]->lexeme);
}
void eval(astTree* root, FILE* fp)
{
	if(strcmp(root->node_symbol,"TK_PLUS")==0)
	{
		eval(root->children[1],fp);
		fprintf(fp,"PUSH ebx\n");
		eval(root->children[0], fp);
		fprintf(fp,"POP ecx\n");
		fprintf(fp,"add ebx,ecx\n");
		return;
	}
	else if(strcmp(root->node_symbol,"TK_MINUS")==0)
	{
		eval(root->children[1], fp);
		fprintf(fp,"PUSH ebx\n");
		eval(root->children[0], fp);
		fprintf(fp,"POP ecx\n");
		fprintf(fp,"sub ebx,ecx\n");
		return;
	}
	if(strcmp(root->node_symbol,"TK_MUL")==0)
	{
		eval(root->children[1],fp);
		fprintf(fp,"PUSH ebx\n");
		eval(root->children[0], fp);
		fprintf(fp,"POP eax\n");
		fprintf(fp,"imul ebx\n");
		fprintf(fp,"mov ebx,eax\n");
		return;
	}
	if(strcmp(root->node_symbol,"TK_DIV")==0)
	{
		eval(root->children[1],fp);
		fprintf(fp,"PUSH ebx\n");
		eval(root->children[0], fp);
		fprintf(fp,"mov eax,ebx\n");
		fprintf(fp,"POP ebx\n");
		fprintf(fp,"idiv ebx\n");
		fprintf(fp,"mov ebx,eax\n");
		return;
	}
	else if(strcmp(root->node_symbol,"<singleOrRecId>")==0)
	{
		if(root->size==1)
		{
			if(root->children[0]->type==0)
				fprintf(fp,"mov ebx,[%s]\n",root->children[0]->lexeme);
			return;
		}
		else
		{
			fprintf(fp,"mov ebx,[%s_%s]\n",root->children[0]->lexeme,root->children[1]->lexeme);
			return;
		}
	}
	/*else if(strcmp(root->node_symbol,"TK_ID")==0)			//depreciated
	{
		fprintf(fp,"mov ebx,[%s]\n",root->lexeme);
		return;
	}*/
	else if(strcmp(root->node_symbol, "TK_NUM")==0)
	{
		fprintf(fp, "mov ebx,%s\n",root->lexeme);
		return;
	}
}