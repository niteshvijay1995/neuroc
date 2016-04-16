/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/

#include "parserDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


first_set* fs[NON_TERMINAL];
follow_set* fol[NON_TERMINAL];
looktable* lt_non_terminal;
looktable* lt_terminal;
list_gram* rules[NUM_RULES];
char* nt_strings[NON_TERMINAL];
int parse_table[NON_TERMINAL][TERMINAL];
int found[NON_TERMINAL];


void parser_init();
void st_push(snode** head, char* value);
void s_node_push(ntree* root, list_gram* rule);
char* st_pop(snode** head);
char* st_front(snode* head);
void push_rule(snode** head ,ntree* s_node, list_gram* rule );
void st_print(snode* head);
void list_gram_insert_end(list_gram** head, char* value);
void list_gram_merge(list_gram** h1, list_gram** h2);
void readnonterminal(char* filename,looktable* lt_non_terminal,char** nt_strings);
void readterminal(char* filename,looktable* lt_terminal);
void readgrammar(char* filename, list_gram** rules);
void print_grammar(list_gram** rules);
int find_first_set(char* non_terminal, list_gram** rules, looktable* lt_non_terminal);
int find_fol(list_gram** rules, looktable* lt_non_terminal);
int ifexists(int idx, char* ter);
int ifexists2(int idx, char* ter);
dum_fol* compute_fol(int idx,int start, int arr[], int size);
int fol_add(int idx, first_set* nw);
int fs_add(int idx, first_set* nw);
void print_fs(looktable* lt_non_terminal, char* nt_string);
void print_fol(looktable* lt_non_terminal, char* nt_string);
int if_exists_in_rule(list_gram* temp, char* ter);
void createParseTable(looktable* lt_non_terminal, looktable* lt_terminal, list_gram** rules);
void print_parse_table();
ntree* parseInputSourceCode(char* test_case_file, int* error);
void printNode(ntree* node, FILE* fp);
void printParseTree(ntree* root, FILE* fp);
int lt_get(looktable* ht, char* key);
void lt_insert(looktable* ht, char* key, int value);
int lt_hash_function(looktable* ht, char* key);
lookentry* lookentry_create(char* key, int value);
looktable* lt_create(int size);
