/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/
void init_typechecker(astTree* root, sym_table* st, char* func_name, int* error);
int if_bool_exp(char* string);
int check_assignop_type(astTree* root,sym_table* st,char* func_name, int* error);
void compute_type(astTree* root, sym_table* st,char* func_name, int* error);
void symbol_table_lookup(astTree* root,sym_table* st,char* func_name, int* error);
int check_boolexp_type(astTree* root,sym_table* st,char* func_name, int* error);
void id_symbol_table_lookup(astTree* root,sym_table* st,char* func_name, int* error);
