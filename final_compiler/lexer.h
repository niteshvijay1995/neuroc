#include "lexerDef.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
FILE* getStream(FILE *fp, char** Buffer, size_t buffersize);
//tokenInfo* lexeme_detected(tokenInfo* token_node,int lineno, char* lexeme);
void read_dfa(char* filename, state** states);
char* getToken(char* lexeme, state** states);
int search_linked_list(Pair* p, int alpha);		//returns the next state after searching linked list.
void printdfa(state** states);				//print DFA tree
int char2index(char ch);				//Compute index of input character for array indexing (Hard coded)
hashtable* ht_create(int size);
entry* entry_create(char* key, char* value);
int hash_function(hashtable* ht, char* key);
void ht_insert(hashtable* ht, char* key, char* value);
char* ht_get(hashtable* ht, char* key);
