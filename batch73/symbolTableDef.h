typedef struct func_sym_table func_sym_table;
typedef struct details details;
typedef struct var_entry var_entry;
typedef struct sym_table sym_table;
typedef struct symbol_list symbol_list;
//typedef struct error_list error_list;
struct details{
	int type; // 0 = int, 1 = real, 2 = record, 3 = record_details.
	int offset;
	char* rec_name;
	func_sym_table* f;
	symbol_list* slist;
	int lineno;
};

struct var_entry{
	char* lexeme;
	details* det;
	struct var_entry *next;
};

struct func_sym_table{
	char* func_name;
	int size;
	int input_par_type[20];
	int num_input_par;
	int num_output_par;
	int output_par_type[20];
	var_entry** table;
	int lineno;
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

// struct error_list{
// 	char* error_name;
// 	struct error_list* next;
// };