void init_typechecker(astTree* root, sym_table* st, char* func_name);
int if_bool_exp(char* string);
int check_assignop_type(astTree* root,sym_table* st,char* func_name);
void compute_type(astTree* root, sym_table* st,char* func_name);
void symbol_table_lookup(astTree* root,sym_table* st,char* func_name);
int check_boolexp_type(astTree* root,sym_table* st,char* func_name);
void id_symbol_table_lookup(astTree* root,sym_table* st,char* func_name);
void check_fun_call(astTree* root,sym_table* st,char* func_name);
void check_ret_stmt(astTree* root,sym_table* st,char* func_name);
void evaluate(astTree* root, FILE* fp);
void eval(astTree* root,char* reg,FILE* fp);
void code_gen(astTree* root,sym_table* st, symbol_list* lis, FILE* fp);
char* reg2(char* reg);