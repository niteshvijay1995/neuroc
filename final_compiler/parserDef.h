#define NON_TERMINAL 51
#define TERMINAL 53
#define NUM_RULES 90

struct list_gram{
	char* value;
	struct list_gram* next;
};
typedef struct list_gram list_gram;

struct first_set{
	char* arr[30];
	int valid;
	int size;
	int eflag;
};
struct follow_set{
	char* arr[30];
	int folof;
	int valid;
	int size;
	int eflag;
};
typedef struct first_set first_set;
typedef struct follow_set follow_set;
struct snode{
	char* value;
	struct snode* next;
};
typedef struct snode snode;

struct ntree{
	int size;
	int consumed;
	int traverse;
	char* lexeme;
	char* node_symbol;
	struct ntree* next[20];
	int is_leaf_node;
	struct ntree* parent;
	int lineno;
};


struct dum_fol{
	char* value;
	struct dum_fol* next;
};

typedef struct dum_fol dum_fol;
typedef struct ntree ntree;

struct lookentry_s{
	char* key;
	int value;
	struct lookentry_s *next;
};
typedef struct lookentry_s lookentry;

struct looktable_s{
	int size;
	lookentry** table;
};
typedef struct looktable_s looktable;