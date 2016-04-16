/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/

#define NUM_STATES 60
typedef struct state state;
typedef struct pair Pair;
typedef struct tokenInfo tokenInfo;
typedef struct entry_s entry;
typedef struct hashtable_s hashtable;
struct pair
{
	int next_state;
	int alpha;
	Pair* next;
};
struct state
{	
	int id;
	Pair* trans;
	char* token;
	int isfinal;
	int islookup;
};
struct tokenInfo
{
	char token[25];
	char lexeme[25];
	int lineno;
	int isvalid;
	int errno;
	tokenInfo* next;
};
struct entry_s{
	char* key;
	char* value;
	struct entry_s *next;
};
struct hashtable_s{
	int size;
	entry** table;
};


