/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/
void check_fun_call(astTree* root,sym_table* st,char* func_name, int* error);
void check_ret_stmt(astTree* root,sym_table* st,char* func_name, int* error);
void check_read_write_stmt(astTree* root, sym_table* st, char* func_name, int* error);