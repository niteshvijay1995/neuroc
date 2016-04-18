struct astTree{
	int size;
	struct astTree* children[20];
	char* node_symbol;
	char* lexeme;
};
typedef struct astTree astTree;

struct intList{
	int val;
	struct intList* next;
};
typedef struct intList intList;

