#include"dfa.h"
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
