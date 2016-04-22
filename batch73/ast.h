#include "astDef.h"

void ast_init();
void print_grammar_for_ast(list_gram** rules);
void modify_grammar(char* filename);
astTree* construct_ast(ntree *root);
void copy_ast_node(astTree* r1, astTree* r2);
astTree* clean_ast(astTree* root);
void print_ast(astTree* root);
int check_pull_up(char* string);
int cannotBeDeleted(char* string);
int count_nodes_parser(ntree* root);
int count_nodes_ast(astTree* root);