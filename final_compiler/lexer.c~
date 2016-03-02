#include "lexer.h"
hashtable* lookup_table;
int num_states = 60;
state* states[60];
int char2index(char ch)
{
	//printf("=%c=",ch);
	if(ch>='b' && ch<='d')
	{
		return 2;
	}
	else if('A'<=ch && ch<='Z')
		return 0;
	else if('0'<=ch && ch<='9')
		return 3;
	else if('a'<=ch && ch<='z')
		return 1;
	else
	{
		switch(ch)
		{
			case '<':
				return 4;
			case '>':
				return 5;
			case '%':
				return 7;
			case '#':
				return 8;
			case '_':
				return 9;
			case '[':
				return 10;
			case ']':
				return 11;
			case ';':
				return 12;
			case ':':
				return 13;
			case '.':
				return 14;
			case '(':
				return 15;
			case ')':
				return 16;
			case '+':
				return 17;
			case '-':
				return 18;
			case '*':
				return 19;
			case '/':
				return 20;
			case '@':
				return 21;
			case '&':
				return 22;
			case '~':
				return 23;
			case '=':
				return 24;
			case '!':
				return 25;

		}
	}

}

void printstate(state** states)
{
	int i=0,j;
	Pair* temp;
	for(i=0;i<60;i++)
	{
		printf("\n%d --> ",i);
		temp = states[i]->trans;
		while(temp != NULL)
		{
			printf("%d %d,",temp->alpha,temp->next_state);
			temp = temp->next;
		}
		if(states[i]->isfinal == 1)
			{
				printf("Final state token = %s",states[i]->token);

			}
	}
}

//returns the next state after searching linked list.
int search_linked_list(Pair* p, int alpha){
	Pair* head = p;
	while(head!=NULL){
		if(head->alpha == alpha) return head->next_state;
		head = head->next;
	}
	return -1;
}

char* getToken(char* lexeme, state** states){
	int i,next_state_id;
	int curr_state_id = 0;
	char* a = malloc(100* sizeof(char));
	a = "Invalid";
	int len = strlen(lexeme);
	for(i=0; i<len; i++){
		int alpha = char2index(lexeme[i]);
		next_state_id =  search_linked_list(states[curr_state_id]->trans, alpha);
		if(next_state_id == -1) return a;
		else curr_state_id = next_state_id;
		//printf("sid = %d\n",curr_state_id);
	}
	if(states[curr_state_id]->islookup==1){
		char* token;
		if((token = ht_get(lookup_table,lexeme)) !=NULL)
			return token;
		}
	if(states[curr_state_id]->isfinal==1) return states[curr_state_id]->token;
	else return a;
}

void read_dfa(char* filename, state** states)
{
	FILE *file = fopen(filename,"r");
	size_t len = 0;
	ssize_t read;
	char* line = NULL;
	char* tok;
	int i;
	for(i=0;i<num_states;i++)
	{
		states[i] = (state*)malloc(sizeof(state));
		states[i]->isfinal = 0;
		states[i]->islookup = 0;
	}
	lookup_table = ht_create(30);
	printf("States initialised\n");
	if(file == 0)
		printf("Problem opening file");
	else
	{
		while((len = getline(&line, &len, file)) != -1)
		{
			if(strcmp(line,"<FinalStates>\n") == 0)
			{
				printf("Reading Final States ...\n");
				int final_state_id;
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</FinalStates>\n"))
				{
					
					tok = strtok(line," ");
					final_state_id = atoi(tok);
					tok = strtok(NULL," \n");
					states[final_state_id]->token = (char*)malloc(sizeof(tok));
					states[final_state_id]->isfinal = 1;
					strcpy(states[final_state_id]->token,tok);
				}
			}
			if(strcmp(line,"<Transition>\n") == 0)
			{
				printf("Reading Transition States ...\n");
				//temp = head;
				int from;
				int to;
				int alpha_index;
				char alpha;
				Pair* temp_pair;		
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</Transition>\n"))
				{
					//printf(".");
					//printf("..%s..",line);
					tok = strtok(line," ");
					from = atoi(tok);						
					//printf("--%s--",tok);
					tok = strtok(NULL," ");
					alpha = tok[0];
					//printf("-------%s------",tok);
					alpha_index = char2index(alpha);
					//printf(".%d.",alpha_index);					
					tok = strtok(NULL," \n");
					//printf("%s",tok);	
					to = atoi(tok);
					temp_pair = states[from]->trans;
					if(temp_pair == NULL)
					{
						states[from]->trans = (Pair*)malloc(sizeof(Pair));
						temp_pair = states[from]->trans;
						//printf("||%d\n",from);
					}
					else
					{
						//printf("Adding more trans\n");
						while(temp_pair->next != NULL)
							temp_pair = temp_pair->next;
						temp_pair->next = (Pair*)malloc(sizeof(Pair));
						temp_pair = temp_pair->next;
					}	
					
					temp_pair->next_state = to;
					temp_pair->alpha = alpha_index;
					temp_pair->next = NULL;
				}
			}
			if(strcmp(line,"<Lookup>\n") == 0)
			{
				printf("Reading Lookup ...\n");
				int state;
				char lexime[20];
				char token[20];		
				while((len = getline(&line, &len, file)) != -1 && strcmp(line,"</Lookup>\n"))
				{
					//printf(".");
					tok = strtok(line," ");
					state = atoi(tok);
					states[state]->isfinal = 1;
					states[state]->islookup = 1;	
					tok = strtok(NULL," ");
					strcpy(lexime,tok);					
					tok = strtok(NULL,"\n");
					strcpy(token,tok);
					ht_insert(lookup_table,lexime,token);
				}
			}
		}
		fclose(file);
	//print(head);
	//printdfa(states);
	//return states;
	}
}

FILE* getStream(FILE *fp, char** Buffer, size_t buffersize)
{
	int c;
	*Buffer = (char*)malloc(sizeof(char)*buffersize); 	
	c = fread(*Buffer,1,buffersize,fp); 
	if(c==0)
		return NULL;	
	return fp;
}

//function called whenever a lexeme is detected (after detecting delimiter)
tokenInfo* lexeme_detected(tokenInfo* token_node,int lineno, char* lexeme)
{	
	char token[25];
	strcpy(token,getToken(lexeme,states));
	if(strcmp(token,"TK_ID")==0 && strlen(lexeme)>20)
	{
		token_node->isvalid = 0;
		token_node->errno = 23;			
		//printf("Identifier length exceeded (Limit : 20)\n"); 
	}
	else
	{
		if(strcmp(token,"Invalid")==0)
		{
			strcpy(token_node->lexeme,lexeme);
			token_node->isvalid = 0;
			token_node->errno = 45;
		}
		else
		{
			token_node->isvalid = 1;
			token_node->errno = 0;
			strcpy(token_node->lexeme,lexeme);
			strcpy(token_node->token,token);
			//printf("Lexeme = %s ",lexeme);
			//printf("Token = %s\n",token);
		}
	}
	token_node->lineno = lineno;
	token_node->next = (tokenInfo*)malloc(sizeof(tokenInfo));
	token_node = token_node->next;	
	return token_node;
}
tokenInfo* getNextToken(FILE *fp)
{
	read_dfa("dfa.dat",states);
	char* Buffer;	
	char ch;			//used when iterating on Buffer
	size_t bsize;
	bsize = 10000;			//Size of buffer
	int i,j=0,temp;			// j -> index for lexeme string | i-> index for Buffer string | temp -> index for temporary check
	char lexeme[60];		//to store lexeme when reading from Buffer
	int comment_flag = 0;		//1 if comment started (when character read is %) and set to 0 when '\n' is read
	int lineno = 1;			//incremented when '\n' is read
	tokenInfo* head_tok;
	head_tok = (tokenInfo*)malloc(sizeof(tokenInfo));
	tokenInfo* temp_tok;
	temp_tok = head_tok;
	while((fp=getStream(fp,&Buffer,bsize))!=NULL)
	{
		i =0;
		//printf("%s\n",Buffer);
		while((ch=Buffer[i])!='\0')
		{
			
			if(comment_flag == 0 || ch == '\n')
			{
				switch(ch)
				{
					case ' ':
						lexeme[j] = '\0';
						if(j!=0)		// to check that lexeme is not empty					
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						//printf("lexeme = %s",lexeme);
						j = 0;				
						break;
					case '%':
						comment_flag = 1;
						//printf("Comment detected on line no. %d\n",lineno);
						break;
					case '\n':
						comment_flag = 0;
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						j = 0;
						lineno++;
						break;
					case '\r':
						break;
					case ':':
						lexeme[j] = '\0';
						if(j!=0)						
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,":");
						j = 0;
						break;
					case '[':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"[");
						j = 0;
						break;
					case ']':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"]");
						j = 0;
						break;
					case ';':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,";");
						j = 0;
						break;
					case '+':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"+");
						j = 0;
						break;
					case '-':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"-");
						j = 0;
						break;
					case '*':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"*");
						j = 0;
						break;
					case '/':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"/");
						j = 0;
						break;
					case ',':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,",");
						j = 0;
						break;
					case '=':			//for detecting ==
						temp = i;
						if(Buffer[++temp] == '=')
						{
							lexeme[j] = '\0';
							if(j!=0)
								temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
							temp_tok = lexeme_detected(temp_tok,lineno,"==");
							j = 0;
							i = temp;
							break;
						}
						lexeme[j] = ch;
						temp_tok = lexeme_detected(temp_tok,lineno,"=");
						j = 0;
						break;
					case '~':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"~");
						j = 0;
						break;
					case '!':
						temp = i;			//for detectinn !=
						if(Buffer[++temp] == '=')
						{
							lexeme[j] = '\0';
							if(j!=0)
								temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
							temp_tok = lexeme_detected(temp_tok,lineno,"!=");
							j = 0;
							i = temp;
							break;
						}
						temp_tok = lexeme_detected(temp_tok,lineno,"!");
						j = 0;
						break;
					case '@':
						temp = i;			//for detectinn @@@
						if(Buffer[++temp] == '@')
							if(Buffer[++temp] == '@')						
								{
									lexeme[j] = '\0';
									if(j!=0)
										temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
									temp_tok = lexeme_detected(temp_tok,lineno,"@@@");
									j = 0;
									i = temp;
									break;
								}
						lexeme[j] = ch;
						break;
					case '&':
						temp = i;			//for detectinn &&&
						if(Buffer[++temp] == '&')
							if(Buffer[++temp] == '&')						
								{
									lexeme[j] = '\0';
									if(j!=0)
										temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
									temp_tok = lexeme_detected(temp_tok,lineno,"&&&");
									j = 0;
									i = temp;
									break;
								}
						lexeme[j] = ch;
						break;
					case '.':
						temp = i;
						/*This is to differentiate between a real number(TK_REAL) and Record type definition 
						Here we are checking next character and if it is a digit then it should be a real number and 							hence skipping the lexeme_detected call*/ 
						if(isdigit(Buffer[++temp]))
						{	
							lexeme[j] = ch;
							j++;
							break;//no need for i = temp as we no not delimiting here (loop will continue as it is)
						}	
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,".");
						j = 0;
						break;
					case '(':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,"(");
						j = 0;
						break;
					case ')':
						lexeme[j] = '\0';
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,")");
						j = 0;
						break;
						break;
					case '\t':
						break;
					case '<':
						if(Buffer[i+1]=='-')			 //check for <-- (Assignment operator)
						{	if(Buffer[i+2]=='-')
								if(Buffer[i+3]=='-')
									{
										lexeme[j] = '\0';
										if(j!=0)
											temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
										temp_tok = lexeme_detected(temp_tok,lineno,"<---");
										i = i+3;
										j = 0;
										break;
									}}
						else if(Buffer[i+1]=='=')			//check for <= (less than or equals to)
						{	
							lexeme[j] = '\0';
							if(j!=0)
								temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
							temp_tok = lexeme_detected(temp_tok,lineno,"<=");
							i = i+1;
							j = 0;
							break;	
						}
						else
						{
							lexeme[j] = '\0';			//for < (less than) as delimiter
							if(j!=0)
								temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
							temp_tok = lexeme_detected(temp_tok,lineno,"<");
							j = 0;
							break;
						}
						break;
					case '>':
						if(Buffer[++temp] == '=')			//check for >= (greater than or equal to)
						{
							lexeme[j] = '\0';
							if(j!=0)
								temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
							temp_tok = lexeme_detected(temp_tok,lineno,">=");
							i = temp;
							j = 0;
							break;	
						}
						lexeme[j] = '\0';				//for >(greater than) as delimiter
						if(j!=0)
							temp_tok = lexeme_detected(temp_tok,lineno,lexeme);
						temp_tok = lexeme_detected(temp_tok,lineno,">");
						j = 0;
						break;
					default:
						lexeme[j] = ch;
						if(j<60)
							j++;
					
				}		//end switch
			}			//end if (for comment)
			i++;
		}				//end while for reading char by char
	}					//end  while of file input buffer
	return head_tok;
}

hashtable* ht_create(int size){
	hashtable* ht = NULL;
	if(size < 1) return ht;
	if((ht = malloc(sizeof(hashtable)))==NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}
	if((ht->table = malloc(sizeof(entry*) * size) ) == NULL){
		printf("cannot allocate memory\n");
		return NULL;
	}

	int i;
	for(i=0;i<size;i++)	ht->table[i] = NULL;
	ht->size = size;

	return ht;
}

entry* entry_create(char* key, char* value){
	entry* kv; //key value pair.
	if( ( kv = malloc(sizeof(entry) ) ) == NULL ){
		printf("cannot allocate memory\n");
		return NULL;
	}
	
	if( ( kv->key = strdup( key ) ) == NULL ) {
		printf("cannot allocate memory\n");
		return NULL;
	}

	if( ( kv->value = strdup( value ) ) == NULL ) {
		printf("cannot allocate memory\n");
		return NULL;
	}

	kv->next = NULL;
	return kv;
}

int hash_function(hashtable* ht, char* key){
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

void ht_insert(hashtable* ht, char* key, char* value){
	entry* newp = NULL, *next = NULL, *last = NULL;
	int hashval = hash_function(ht,key);
	next = ht->table[hashval];
	
	while(next!=NULL && next->key!=NULL && strcmp(key, next->key) > 0 ){
		last = next;
		next = next->next;
	}
	if(next!=NULL && next->key != NULL && strcmp(key, next->key) == 0 ){
		
		free( next->value );
		next->value = strdup(value);
	
	} else {
		newp = entry_create(key, value);

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

char* ht_get(hashtable* ht, char* key){
	int hashval = hash_function(ht, key);
	entry* newp = ht->table[hashval];
	while(newp!=NULL && newp->key!=NULL && strcmp( key, newp->key ) > 0 ){
		newp = newp->next;
	}
	if(newp == NULL || newp->key == NULL || strcmp(key,newp->key) != 0 ){
		return NULL;
	}
	else{

		return newp->value;
	}
}
						//end 
