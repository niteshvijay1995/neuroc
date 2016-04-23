/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/
void evaluate(astTree* root,func_sym_table* g_table, FILE* fp);
void eval(astTree* root,FILE* fp);
void code_gen(astTree* root,sym_table* st, symbol_list* lis, FILE* fp);
char* reg2(char* reg);
void cond_eval(astTree* root,FILE* fp);
void cond_evaluate(astTree* root,FILE* fp);
void cond_stmt(astTree* root,sym_table* st,func_sym_table* g_table,FILE* fp);
void code_gen1(astTree* root,sym_table* st,func_sym_table* g_table,FILE* fp);
void iter_stmt(astTree* root,sym_table* st ,func_sym_table* g_table,FILE *fp);
void evaluate_record(astTree* root,sym_table* st,func_sym_table* g_table, FILE* fp);
void eval_rec(astTree* root,char* fid,FILE* fp);
void print_rec(astTree* root,sym_table* st,func_sym_table* g_table, FILE* fp);