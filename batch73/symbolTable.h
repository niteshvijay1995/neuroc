/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/
#include "symbolTableDef.h"

void push_symbol_list(symbol_list** lis, char* lexeme, char* func_name);
sym_table* push_sym_table(sym_table* head, func_sym_table* data);
func_sym_table* sym_create(int size, char* name);
void copy_details(details* d1, details* d2);
var_entry* var_entry_create(char* key, details* det);
int sym_hash_function(func_sym_table* ht, char* key);
void func_sym_insert(func_sym_table* ht, char* key, details* value);
details* func_sym_get(func_sym_table* ht, char* key);
void createVarEntry(func_sym_table* f, func_sym_table* g, char* lexeme, details* d, symbol_list** lis, int flag, int* error);
void allocate(astTree* temp, int* offset, func_sym_table* f, func_sym_table* g, looktable* lt_rec, symbol_list** lis, int flag, int flag2, int* error);
func_sym_table* createMainFuncEntry(astTree* root, char* name, func_sym_table* g, symbol_list** lis, looktable* lt_rec, int flag, int* error);
sym_table* createSymbolTable(astTree* root, symbol_list** lis, int flag, int* error);
func_sym_table* search_sym_table(sym_table* st, char* name);
void printSymbolTable(sym_table* st, symbol_list* lis);
void iterate_type_def(details** d, astTree* root, func_sym_table* g, int flag, int* error);