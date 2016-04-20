void init_typechecker(astTree* root, sym_table* st, char* func_name);
int if_bool_exp(char* string);
int check_assignop_type(astTree* root,sym_table* st,char* func_name);
void compute_type(astTree* root, sym_table* st,char* func_name);
void symbol_table_lookup(astTree* root,sym_table* st,char* func_name);
int check_boolexp_type(astTree* root,sym_table* st,char* func_name);
