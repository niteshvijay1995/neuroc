#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

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

looktable* lt_create(int size){
	looktable* ht = NULL;
	if(size < 1) return ht;
	if((ht = malloc(sizeof(looktable)))==NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}
	if((ht->table = malloc(sizeof(lookentry*) * size) ) == NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}

	int i;
	for(i=0;i<size;i++)	ht->table[i] = NULL;
	ht->size = size;

	return ht;
}

lookentry* lookentry_create(char* key, int value){
	lookentry* kv; //key value pair.
	if( ( kv = malloc(sizeof(lookentry) ) ) == NULL ){
		printf("cannot allocate memory\n");
		return NULL;
	}
	
	if( ( kv->key = strdup( key ) ) == NULL ) {
		printf("cannot allocate memory\n");
		return NULL;
	}

	kv->value = value;

	kv->next = NULL;
	return kv;
}

int lt_hash_function(looktable* ht, char* key){
	unsigned long int hashval=0; //?
	int i = 0;

	/* Convert our string to an integer */
	while( hashval < ULONG_MAX && i < strlen( key ) ) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}
	
	return hashval % ht->size;
}

void lt_insert(looktable* ht, char* key, int value){
	lookentry* newp = NULL, *next = NULL, *last = NULL;
	int hashval = lt_hash_function(ht,key);
	next = ht->table[hashval];
	
	while(next!=NULL && next->key!=NULL && strcmp(key, next->key) > 0 ){
		last = next;
		next = next->next;
	}
	if(next!=NULL && next->key != NULL && strcmp(key, next->key) == 0 ){
		
		next->value = value;
	
	} else {
		newp = lookentry_create(key, value);

		if(next == ht->table[hashval]){
			newp->next = next;
			ht->table[hashval] = newp;
		
		} else if (next == NULL){
			last->next = newp;
		
		} else{
			newp->next = next;
			last->next = newp;
		}
	}

	
}

int lt_get(looktable* ht, char* key){
	int hashval = lt_hash_function(ht, key);
	lookentry* newp = ht->table[hashval];
	while(newp!=NULL && newp->key!=NULL && strcmp( key, newp->key ) > 0 ){
		newp = newp->next;
	}
	if(newp == NULL || newp->key == NULL || strcmp(key,newp->key) != 0 ){
		return -1;
	}
	else{

		return newp->value;
	}
}
/*

int main( int argc, char **argv ) {

	looktable* ht = lt_create( 65536 );

	lt_insert( ht, "key1", 1 );
	lt_insert( ht, "key2", 2 );
	lt_insert( ht, "key3", 3 );
	lt_insert( ht, "key4", 4 );

	printf( "%d\n", lt_get( ht, "key1" ) );
	printf( "%d\n", lt_get( ht, "key2" ) );
	printf( "%d\n", lt_get( ht, "key3" ) );
	printf( "%d\n", lt_get( ht, "key4" ) );

	return 0;
}
*/