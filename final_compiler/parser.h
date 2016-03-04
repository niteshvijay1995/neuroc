#include "parserDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
void st_push(snode** head, char* value);
void s_node_push(ntree* root, char* value);
char* st_pop(snode** head);
char* st_front(snode* head);
void push_rule(snode** head ,ntree* s_node, list_gram* rule );
void st_print(snode* head);
void list_gram_insert_end(list_gram** head, char* value);
void list_gram_merge(list_gram** h1, list_gram** h2);
void readgrammar(char* filename, list_gram** rules);
void print_grammar(list_gram** rules);
int find_first_set(char* non_terminal, list_gram** rules, looktable* lt_non_terminal);
int find_fol(list_gram** rules, looktable* lt_non_terminal);
int ifexists(int idx, char* ter);
int ifexists2(int idx, char* ter);
dum_fol* compute_fol(int idx,int start);
int fol_add(int idx, first_set* nw);
int fs_add(int idx, first_set* nw);
void print_fs(looktable* lt_non_terminal, char* nt_string);
void print_fol(looktable* lt_non_terminal, char* nt_string);
int if_exists_in_rule(list_gram* temp, char* ter);
void createParseTable(looktable* lt_non_terminal, looktable* lt_terminal, list_gram** rules);
void print_parse_table();
ntree* parseInputSourceCode(char* test_case_file, list_gram** rules, looktable* lt_terminal, looktable* lt_non_terminal, int* error);
void printNode(ntree* node, FILE* fp);
void printParseTree(ntree* root, FILE* fp);
