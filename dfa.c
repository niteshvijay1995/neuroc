#include"dfa.h"
struct pair
{
	int state;
	int alpha;
	Pair* next;
};
struct state
{	
	int id;
	Pair* trans;
	char* token;
};
