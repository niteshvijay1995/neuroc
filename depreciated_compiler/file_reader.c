/*
Author Nitesh Vijay
Date 29-02-16  
*/

#include<stdio.h>
#include<malloc.h>

//Function to get buffer stream from file
FILE* getStream(FILE *fp, char** Buffer, size_t buffersize)
{
	int c;
	*Buffer = (char*)malloc(sizeof(char)*buffersize); 	
	c = fread(*Buffer,1,buffersize,fp); 
	if(c==0)
		return NULL;	
	return fp;
}


void lexime_detected(char* lexime)			//function called whenever a lexime is detected (after detecting delimiter)
{
	printf("Lexime = %s\n",lexime);
	fflush(stdout);
}


void main()
{
	char* Buffer;	
	char ch;			//used when iterating on Buffer
	size_t bsize;
	bsize = 10000;			//Size of buffer
	int i,j=0,temp;			// j -> index for lexime string | i-> index for Buffer string | temp -> index for temporary check
	char lexime[20];		//to store lexime when reading from Buffer
	int comment_flag = 0;		//1 if comment started (when character read is %) and set to 0 when '\n' is read
	int lineno = 1;			//incremented when '\n' is read
	FILE *fp = fopen("./testcases/testcase2.txt","r");
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
						lexime[j] = '\0';
						if(j!=0)		// to check that lexime is not empty					
							lexime_detected(lexime);
						//printf("Lexime = %s",lexime);
						j = 0;				
						break;
					case '%':
						comment_flag = 1;
						printf("Comment detected on line no. %d\n",lineno);
						break;
					case '\n':
						comment_flag = 0;
						lexime[j] = '\0';
						if(j!=0)
							//printf("j = %d",j);
							lexime_detected(lexime);
						j = 0;
						lineno++;
						break;
					case '\r':
						break;
					case ':':
						lexime[j] = '\0';
						if(j!=0)						
							lexime_detected(lexime);
						lexime_detected(":");
						j = 0;
						break;
					case '[':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("[");
						j = 0;
						break;
					case ']':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("]");
						j = 0;
						break;
					case ';':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected(";");
						j = 0;
						break;
					case '+':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("+");
						j = 0;
						break;
					case '-':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("-");
						j = 0;
						break;
					case '*':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("*");
						j = 0;
						break;
					case '/':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("/");
						j = 0;
						break;
					case '=':			//for detecting ==
						temp = i
						if(Buffer[++temp] == '=')
						{
							lexime[j] = '\0';
							if(j!=0)
								lexime_detected(lexime);
							lexime_detected("==");
							j = 0;
							i = temp;
							break;
						}
					case '~':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("~");
						j = 0;
						break;
					case '!':
						temp = i			//for detectinn !=
						if(Buffer[++temp] == '=')
						{
							lexime[j] = '\0';
							if(j!=0)
								lexime_detected(lexime);
							lexime_detected("!=");
							j = 0;
							i = temp;
							break;
						}
						break;
					case '@':
						temp = i			//for detectinn @@@
						if(Buffer[++temp] == '@')
							if(Buffer[++temp] == '@')						
								{
									lexime[j] = '\0';
									if(j!=0)
										lexime_detected(lexime);
									lexime_detected("@@@");
									j = 0;
									i = temp;
									break;
								}
						break;
					case '&':
						temp = i			//for detectinn &&&
						if(Buffer[++temp] == '&')
							if(Buffer[++temp] == '&')						
								{
									lexime[j] = '\0';
									if(j!=0)
										lexime_detected(lexime);
									lexime_detected("&&&");
									j = 0;
									i = temp;
									break;
								}
						break;
					case '.':
						temp = i;
						/*This is to differentiate between a real number(TK_REAL) and Record type definition 
						Here we are checking next character and if it is a digit then it should be a real number and 							hence skipping the lexime_detected call*/ 
						if(isdigit(Buffer[++temp]))
						{	
							lexime[j] = ch;
							j++;
							break;//no need for i = temp as we no not delimiting here (loop will continue as it is)
						}	
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected(".");
						j = 0;
						break;
					case '(':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("(");
						j = 0;
						break;
					case ')':
						lexime[j] = '\0';
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected(")");
						j = 0;
						break;
						break;
					case '\t':
						break;
					case '<':
						temp = i;
						if(Buffer[++temp]=='-')					//check for <-- (Assignment operator)
							if(Buffer[++temp]=='-')
								if(Buffer[++temp]=='-')
									{
										lexime[j] = '\0';
										if(j!=0)
											lexime_detected(lexime);
										lexime_detected("<---");
										i = temp;
										j = 0;
										break;
									}
						else if(Buffer[temp] == '=')				//check for <= (less than or equals to)
						{
							lexime[j] = '\0';
							if(j!=0)
								lexime_detected(lexime);
							lexime_detected("<=");
							i = temp;
							j = 0;
							break;	
						}
						lexime[j] = '\0';					//for < (less than) as delimiter
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected("<");
						j = 0;
						break;
					case '>':
						if(Buffer[++temp] == '=')			//check for >= (greater than or equal to)
						{
							lexime[j] = '\0';
							if(j!=0)
								lexime_detected(lexime);
							lexime_detected(">=");
							i = temp;
							j = 0;
							break;	
						}
						lexime[j] = '\0';				//for >(greater than) as delimiter
						if(j!=0)
							lexime_detected(lexime);
						lexime_detected(">");
						j = 0;
						break;
					default:
						lexime[j] = ch;
						j++;
					//	printf("d exec =  %d |",ch);
					
				}		//end switch
			}			//end if (for comment)
			i++;
		}				//end while for reading char by char
	}					//end  while of file input buffer
}						//end main

