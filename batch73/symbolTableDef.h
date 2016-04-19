typedef struct func_sym_table func_sym_table;
typedef struct details details;
typedef struct var_entry var_entry;
typedef struct sym_table sym_table;
typedef struct symbol_list symbol_list;

struct details{
	int type; // 0 = int, 1 = real, 2 = record.
	int offset;
	char* rec_name;
	func_sym_table* f;
	symbol_list* slist;
};

struct var_entry{
	char* lexeme;
	details* det;
	struct var_entry *next;
};

struct func_sym_table{
	char* func_name;
	int size;
	var_entry** table;
};

struct sym_table{
	func_sym_table* f;
	struct sym_table* next;
};

struct symbol_list{
	char* lexeme;
	char* func_name;
	struct symbol_list* next;
};
