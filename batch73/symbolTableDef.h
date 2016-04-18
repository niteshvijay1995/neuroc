struct details{
	int type; // 0 = int, 1 = real, 2 = record.
	int offset;
};
typedef struct details details;

struct var_entry{
	char* lexeme;
	details* det;
	struct var_entry *next;
};
typedef struct var_entry var_entry;

struct func_sym_table{
	char* func_name;
	int size;
	var_entry** table;
};
typedef struct func_sym_table func_sym_table;

struct sym_table{
	func_sym_table* f;
	struct sym_table* next;
};
typedef struct sym_table sym_table;

struct symbol_list{
	char* lexeme;
	char* func_name;
	struct symbol_list* next;
};
typedef struct symbol_list symbol_list;
