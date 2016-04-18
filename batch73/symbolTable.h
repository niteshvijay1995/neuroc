#include "symbolTableDef.h"

void push_symbol_list(char* lexeme, char* func_name);
sym_table* push_sym_table(sym_table* head, func_sym_table* data);
func_sym_table* sym_create(int size, char* name);
void copy_details(details* d1, details* d2);
var_entry* var_entry_create(char* key, details* det);
int sym_hash_function(func_sym_table* ht, char* key);
void func_sym_insert(func_sym_table* ht, char* key, details* value);
details* func_sym_get(func_sym_table* ht, char* key);
void createVarEntry(func_sym_table* f, char* lexeme, details* d);
void allocate(astTree* temp, int* offset, func_sym_table* f, func_sym_table* g, looktable* lt_rec);
func_sym_table* createMainFuncEntry(astTree* root, char* name, func_sym_table* g);
sym_table* createSymbolTable(astTree* root);
func_sym_table* search_sym_table(sym_table* st, char* name);
void printSymbolTable(sym_table* st);
