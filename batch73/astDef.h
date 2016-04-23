/*
Batch 73
Nitesh Vijay 2013A7PS164P
Nilay Jain 2013A7PS179P
*/
struct astTree{
	int size;
	struct astTree* children[20];
	char* node_symbol;
	char* lexeme;
	int lineno;
	int type;
};
typedef struct astTree astTree;

struct intList{
	int val;
	struct intList* next;
};
typedef struct intList intList;

