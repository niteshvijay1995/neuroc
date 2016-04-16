

#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stddef.h>
#include<stdio.h>

struct token{
char* name;
char* follow[20];
char* first[20];
};


struct look{

char* lex;

char* val;

int lno;

struct look *next;

};

typedef struct look LOOK;



struct leaf{

struct look* arg;

struct leaf* parent;

struct leaf* next;

struct leaf* child;

};



typedef struct leaf Leaf;

struct node{

char* str;

struct node *next;

};



/*for(hjk=0;hjk<20;hjk++){
    token->first[hjk] = "#";
    token->follow[hjk] = "#";
}*/
typedef struct token TOKEN;
TOKEN  *tk[50];


void fun(){
printf("ffucktou");
}

char* all_tokens[] = {"<program>","<mainFunction>","<otherFunctions>","<function>","<input_par>","<output_par>","<primitiveDatatype>","<constructedDatatype>","<remaining_list>","<typeDefinitions>","<typeDefinition>","<fieldDefinition>","<moreFields>",
                      "<declarations>","<declaration>","<global_or_not>","<otherStmts>","<SingleOrRecId>","<new_24>","<funCallStmt>","<outputParameters>","<inputParameters>","<iterativeStmt>","<conditionalStmt>","<elsePart>",
                      "<ioStmt>","<allVar>","<expPrime>","<termPrime>","<factor>","<highPrecedenceOperators>","<lowPrecedenceOperators>","<all>","<temp>","<booleanExpression>","<var>","<logicalOp>","<relationalOp>","<returnStmt>",
                      "<optionalReturn>","<idList>","<more_ids>","<dataType>","<fieldDefinitions>","<assignmentStmt>","<stmts>","<stmt>","<term>","<parameterList>","<arithmeticExpression>","TK_ASSIGNOP","TK_COMMENT","TK_FIELDID",
                      "TK_ID","TK_NUM","TK_RNUM","TK_FUNID","TK_RECORDID","TK_WITH","TK_PARAMETERS","TK_END","TK_WHILE","TK_TYPE","TK_MAIN","TK_GLOBAL","TK_PARAMETER","TK_LIST","TK_SQL","TK_SQR",
                      "TK_INPUT","TK_OUTPUT","TK_INT","TK_REAL","TK_SEM","TK_COLON","TK_DOT","TK_ENDWHILE","TK_OP","TK_CL","TK_IF","TK_THEN","TK_ENDIF","TK_READ","TK_WRITE","TK_RETURN","TK_PLUS","TK_MINUS","TK_MUL","TK_DIV","TK_CALL","TK_RECORD","TK_ENDRECORD",
                      "TK_ELSE","TK_AND","TK_OR","TK_NOT","TK_LT","TK_LE","TK_EQ","TK_GT","TK_GE","TK_NE"};

char* gr[89];


int all_not(char* str,char* nt,int** pa){
    int p,q = 0,s = 0,i;
     char* not[10];
    char** wer;
    p = strlen(nt);
    while(str[q] != '<') q++; //printf("\n %d",q);
    q += p;
    //printf("\n%d",q);
    while(str[q] != '=') q++;
    q += 4;
    //printf("\n%d",q);
    while((str[q] != '\0')&&(str[q] != '\r')&&(str[q] != '\n') ){
    while(str[q] == ' ') q++;
    if((str[q] == '\r')||(str[q] == '\0')||(str[q] == '\n')) goto bb;
    //printf("\n%d",q);
    while(1){
        int r = 0;
        while((str[q] != ' ') ){
            if((str[q] == '\0')||(str[q] == '\r')||(str[q] == '\n') )
            break;

            r++;q++;
        }
  //      if((str[q] != '\0')&&(str[q] != '\r')&&(str[q] != '\n') )
//      break;

        //printf("\n%d",q);
        not[s] = (char*) calloc(sizeof(char),r+1);
        int t;
        t = r;
        q = q-r;r = 0;
        //printf("\n%d",q);
        while(r < t){
         not[s][r]= str[q];
        r++;q++;}
        not[s][t]= '\0';
            break;
    }
    s++;
    }
    wer = not;
    int* arr = (int*) malloc(s*sizeof(int));
    for(i=0;i<s;i++){
        arr[i]=getint(not[i]);

    }


//printf("%s",not[0]);
bb:    *pa = arr;
    return s;
}
int getint(char* str){
  int i;
  for(i=0;i<102;i++){
    if(strcmp(str,all_tokens[i]) == 0)
        return i;
  }
    return -1;

}

bool has_null(int sdf){
    int k,i=0;
    k = sdf;
    while(strcmp(tk[k]->first[i],"#") != 0){
    if(strcmp(tk[k]->first[i],"") == 0)
        return 1;
    i++;
    }
    return 0;
}
void createGramTab(){
int i =0;
size_t p =0;
FILE *fp;
fp = fopen("grammar.txt","r");
while(i<89){gr[0] = (char*) malloc(89*sizeof(char));i++;}
i=0;
if (fp == NULL)printf("qedw");
else{
while(i<89){
    getline(&gr[i],&p,fp);

//printf("%s %c",gr[i],gr[i][10]);    
i++;
}

}
}



int main(){
    int x_fir;
    int yui,hjk;
int k = 0;
while(k<50){
tk[k] = (TOKEN*) malloc(sizeof(TOKEN));
k++;
}


tk[0]->name = "<program>";tk[0]->first[0] = "TK_FUNID";tk[0]->first[1] = "TK_MAIN";tk[0]->first[2] = "";tk[0]->first[3] = "#";
tk[1]->name = "<mainFunction>";tk[1]->first[0] = "TK_MAIN";tk[1]->first[1] = "#";
tk[2]->name = "<otherFunctions>";tk[2]->first[0] = "TK_FUNID";tk[2]->first[1] = "";tk[2]->first[2] = "#";
tk[3]->name = "<function>";tk[3]->first[0] = "TK_FUNID";tk[3]->first[1] = "#";
tk[4]->name = "<input_par>";tk[4]->first[0] = "TK_INPUT";tk[4]->first[1] = "#";
tk[5]->name = "<output_par>";tk[5]->first[0] = "TK_OUTPUT";tk[5]->first[1] = "";tk[5]->first[2] = "#";
tk[6]->name = "<primitiveDatatype>";tk[6]->first[0] = "TK_INT";tk[6]->first[1] = "TK_REAL";tk[6]->first[2] = "#";
tk[7]->name = "<constructedDatatype>";tk[7]->first[0] = "TK_RECORD";tk[7]->first[1] = "#";
tk[8]->name = "<remaining_list>";tk[8]->first[0] = "TK_COMMA";tk[8]->first[1] = "";tk[8]->first[2] = "#";
tk[9]->name = "<typeDefinitions>";tk[9]->first[0] = "TK_RECORD";tk[9]->first[1] = "";tk[9]->first[2] = "#";
tk[10]->name = "<typeDefinition>";tk[10]->first[0] = "TK_RECORD";tk[10]->first[1] = "#";
tk[11]->name = "<fieldDefinition>";tk[11]->first[0] = "TK_TYPE";tk[11]->first[1] = "#";
tk[12]->name = "<moreFields>";tk[12]->first[0] = "TK_TYPE";tk[12]->first[1] = "";tk[12]->first[2] = "#";
tk[13]->name = "<declarations>";tk[13]->first[0] = "TK_TYPE";tk[13]->first[1] = "";tk[13]->first[2] = "#";
tk[14]->name = "<declaration>";tk[14]->first[0] = "TK_TYPE";tk[14]->first[1] = "#";tk[14]->first[2] = "#";
tk[15]->name = "<global_or_not>";tk[15]->first[0] = "TK_COLON";tk[15]->first[1] = "";tk[15]->first[2] = "#";
tk[16]->name = "<otherStmts>";tk[16]->first[0] = "TK_ID";tk[16]->first[1] = "TK_IF";tk[16]->first[2] = "TK_CALL";tk[16]->first[3] = "TK_SQL";tk[16]->first[4] = "TK_WHILE";tk[16]->first[5] = "TK_READ";tk[16]->first[6] = "TK_WRITE";tk[16]->first[7] = "#";
tk[17]->name = "<singleOrRecId>";tk[17]->first[0] = "TK_ID";tk[17]->first[1] = "#";
tk[18]->name = "<new_24>";tk[18]->first[0] = "TK_DOT";tk[18]->first[1] = "";tk[18]->first[2] = "#";
tk[19]->name = "<funCallStmt>";tk[19]->first[0] = "TK_CALL";tk[19]->first[1] = "TK_SQL";tk[19]->first[2] = "";tk[19]->first[3] = "#";
tk[20]->name = "<outputParameters>";tk[20]->first[0] = "TK_SQL";tk[20]->first[1] = "";tk[20]->first[2] = "#";
tk[21]->name = "<inputParameters>";tk[21]->first[0] = "TK_SQL";tk[21]->first[1] = "#";
tk[22]->name = "<iterativeStmt>";tk[22]->first[0] = "TK_WHILE";tk[22]->first[1] = "#";
tk[23]->name = "<conditionalStmt>";tk[23]->first[0] = "TK_IF";tk[23]->first[1] = "#";
tk[24]->name = "<elsePart>";tk[24]->first[0] = "TK_ELSE";tk[24]->first[1] = "TK_ENDIF";tk[24]->first[2] = "#";
tk[25]->name = "<ioStmt>";tk[25]->first[0] = "TK_READ";tk[25]->first[1] = "TK_WRITE";tk[25]->first[2] = "#";
tk[26]->name = "<allVar>";tk[26]->first[0] = "TK_RECORDID";tk[26]->first[1] = "TK_ID";tk[26]->first[2] = "TK_NUM";tk[26]->first[3] = "TK_RNUM";tk[26]->first[4] = "#";
tk[27]->name = "<expPrime>";tk[27]->first[0] = "TK_PLUS";tk[27]->first[1] = "TK_MINUS";tk[27]->first[2] = "";tk[27]->first[3] = "#";
tk[28]->name = "<termPrime>";tk[28]->first[0] = "TK_MUL";tk[28]->first[1] = "TK_DIV";tk[28]->first[2] = "";tk[28]->first[3] = "#";
tk[29]->name = "<factor>";tk[29]->first[0] = "TK_OP";tk[29]->first[1] = "TK_ID";tk[29]->first[2] = "TK_NUM";tk[29]->first[3] = "TK_RNUM";tk[29]->first[4] = "TK_RECORDID";tk[29]->first[5] = "#";
tk[30]->name = "<highPrecedenceOperators>";tk[30]->first[0] = "TK_MUL";tk[30]->first[1] = "TK_DIV";tk[30]->first[2] = "#";
tk[31]->name = "<lowPrecedenceOperators>";tk[31]->first[0] = "TK_PLUS";tk[31]->first[1] = "TK_MINUS";tk[31]->first[2] = "#";
tk[32]->name = "<all>";tk[32]->first[0] = "TK_ID";tk[32]->first[1] = "TK_NUM";tk[32]->first[2] = "TK_RNUM";tk[32]->first[3] = "TK_RECORDID";tk[32]->first[4] = "#";
tk[33]->name = "<temp>";tk[33]->first[0] = "TK_DOT";tk[33]->first[1] = "";tk[33]->first[2] = "#";
tk[34]->name = "<booleanExpression>";tk[34]->first[0] = "TK_OP";tk[34]->first[1] = "TK_NOT";tk[34]->first[2] = "TK_ID";tk[34]->first[3] = "TK_NUM";tk[34]->first[4] = "TK_RNUM";tk[34]->first[5] = "#";
tk[35]->name = "<var>";tk[35]->first[0] = "TK_ID";tk[35]->first[1] = "TK_NUM";tk[35]->first[2] = "TK_RNUM";tk[35]->first[3] = "#";
tk[36]->name = "<logicalOp>";tk[36]->first[0] = "TK_AND";tk[36]->first[1] = "TK_OR";tk[36]->first[2] = "#";
tk[37]->name = "<relationalOp>";tk[37]->first[0] = "TK_LT";tk[37]->first[1] = "TK_LE";tk[37]->first[2] = "TK_EQ";tk[37]->first[3] = "TK_GT";tk[37]->first[4] = "TK_GE";tk[37]->first[5] = "TK_NE";
tk[37]->first[6] = "#";
tk[38]->name = "<returnStmt>";tk[38]->first[0] = "TK_RETURN";tk[38]->first[1] = "#";
tk[39]->name = "<optionalReturn>";tk[39]->first[0] = "TK_SQL";tk[39]->first[1] = "";tk[39]->first[2] = "#";
tk[40]->name = "<idList>";tk[40]->first[0] = "TK_ID";tk[40]->first[1] = "#";
tk[41]->name = "<more_ids>";tk[41]->first[0] = "TK_COMMA";tk[41]->first[1] = "";tk[41]->first[2] = "#";
tk[42]->name = "<dataType>";tk[42]->first[0] = "TK_INT";tk[42]->first[1] = "TK_REAL";tk[42]->first[2] = "TK_RECORD";tk[42]->first[3] = "#";
tk[43]->name = "<fieldDefinitions>";tk[43]->first[0] = "TK_TYPE";tk[43]->first[1] = "#";
tk[44]->name = "<assignmentStmt>";tk[44]->first[0] = "TK_ID";tk[44]->first[1] = "#";
tk[45]->name = "<stmts>";tk[45]->first[0] = "TK_RECORD";tk[45]->first[1] = "TK_TYPE";tk[45]->first[2] = "TK_RETURN";tk[45]->first[3] = "TK_ID";tk[45]->first[4] = "TK_IF";tk[45]->first[5] = "TK_CALL";tk[45]->first[6] = "TK_SQL";tk[45]->first[7] = "TK_WHILE";tk[45]->first[8] = "TK_READ";tk[45]->first[9] = "TK_WRITE";tk[45]->first[10] = "";tk[45]->first[11] = "#";
tk[46]->name = "<stmt>";tk[46]->first[0] = "TK_ID";tk[46]->first[1] = "TK_IF";tk[46]->first[2] = "TK_CALL";tk[46]->first[3] = "TK_SQL";tk[46]->first[4] = "TK_WHILE";tk[46]->first[5] = "TK_READ";tk[46]->first[6] = "TK_WRITE";tk[46]->first[7] = "";tk[46]->first[8] = "#";
tk[47]->name = "<term>";tk[47]->first[0] = "TK_OP";tk[47]->first[1] = "TK_ID";tk[47]->first[2] = "TK_NUM";tk[47]->first[3] = "TK_RNUM";tk[47]->first[4] = "TK_RECORDID";tk[47]->first[5] = "#";
tk[48]->name = "<parameter_list>";tk[48]->first[0] = "TK_INT";tk[48]->first[1] = "TK_REAL";tk[48]->first[2] = "TK_RECORD";tk[48]->first[3] = "#";
tk[49]->name = "<arithmeticExpression>";tk[49]->first[0] = "TK_OP";tk[49]->first[1] = "TK_ID";tk[49]->first[2] = "TK_NUM";tk[49]->first[3] = "TK_RNUM";tk[49]->first[4] = "TK_RECORDID";
tk[49]->first[5] = "#";


tk[0]->follow[0] = "$";tk[0]->follow[1] = "#";
tk[1]->follow[0] = "$";tk[1]->follow[1] = "#";
tk[2]->follow[0] = "TK_MAIN";tk[2]->follow[1] = "#";
tk[3]->follow[0] = "TK_FUNID";tk[3]->follow[1] = "TK_MAIN";tk[3]->follow[2] = "#";
tk[4]->follow[0] = "TK_OUTPUT";tk[4]->follow[1] = "#";
tk[5]->follow[0] = "TK_SEM";tk[5]->follow[1] = "#";
tk[6]->follow[0] = "TK_COLON";tk[6]->follow[1] = "TK_ID";tk[6]->follow[2] = "#";
tk[7]->follow[0] = "TK_COLON";tk[7]->follow[1] = "TK_ID";tk[7]->follow[2] = "#";
tk[8]->follow[0] = "TK_SQR";tk[8]->follow[1] = "#";
tk[9]->follow[0] = "TK_ID";tk[9]->follow[1] = "TK_IF";tk[9]->follow[2] = "TK_CALL";tk[9]->follow[3] = "TK_SQL";tk[9]->follow[4] = "TK_WHILE";tk[9]->follow[5] = "TK_READ";tk[9]->follow[6] = "TK_WRITE";tk[9]->follow[7] = "TK_RETURN";tk[9]->follow[8] = "TK_TYPE";tk[9]->follow[9] = "#";
tk[10]->follow[0] = "TK_RECORD";tk[10]->follow[1] = "TK_ID";tk[10]->follow[2] = "TK_IF";tk[10]->follow[3] = "TK_CALL";tk[10]->follow[4] = "TK_SQL";tk[10]->follow[5] = "TK_WHILE";tk[10]->follow[6] = "TK_READ";tk[10]->follow[7] = "TK_WRITE";tk[10]->follow[8] = "TK_RETURN";tk[10]->follow[9] = "TK_TYPE";tk[10]->follow[10] = "#";
tk[11]->follow[0] = "TK_TYPE";tk[11]->follow[1] = "TK_ENDRECORD";tk[11]->follow[2] = "#";
tk[12]->follow[0] = "TK_ENDRECORD";tk[12]->follow[1] = "#";
tk[13]->follow[0] = "TK_ID";tk[13]->follow[1] = "TK_IF";tk[13]->follow[2] = "TK_CALL";tk[13]->follow[3] = "TK_SQL";tk[13]->follow[4] = "TK_WHILE";tk[13]->follow[5] = "TK_READ";tk[13]->follow[6] = "TK_WRITE";tk[13]->follow[7] = "#";
tk[14]->follow[0] = "TK_TYPE";tk[14]->follow[1] = "TK_ID";tk[14]->follow[2] = "TK_IF";tk[14]->follow[3] = "TK_CALL";tk[14]->follow[4] = "TK_SQL";tk[14]->follow[5] = "TK_WHILE";tk[14]->follow[6] = "TK_READ";tk[14]->follow[7] = "TK_WRITE";tk[14]->follow[8] = "#";
tk[15]->follow[0] = "TK_SEM";tk[15]->follow[1] = "#";
tk[16]->follow[0] = "TK_ENDIF";tk[16]->follow[1] = "TK_ELSE";tk[16]->follow[2] = "TK_ENDWHILE";tk[16]->follow[3] = "TK_RETURN";tk[16]->follow[4] = "#";
tk[17]->follow[0] = "TK_CL";tk[17]->follow[1] = "TK_ASSIGNOP";tk[17]->follow[2] = "#";
tk[18]->follow[0] = "TK_CL";tk[18]->follow[1] = "TK_ASSIGNOP";tk[18]->follow[2] = "#";
tk[19]->follow[0] = "TK_ID";tk[19]->follow[1] = "TK_IF";tk[19]->follow[2] = "TK_CALL";tk[19]->follow[3] = "TK_SQL";tk[19]->follow[4] = "TK_WHILE";tk[19]->follow[5] = "TK_READ";tk[19]->follow[6] = "TK_WRITE";tk[19]->follow[7] = "TK_OP";tk[19]->follow[8] = "TK_NOT";tk[19]->follow[9] = "TK_NUM";tk[19]->follow[10] = "RNUM";tk[19]->follow[11] = "TK_ENDIF";tk[19]->follow[12] = "TK_ELSE";tk[19]->follow[13] = "ENDWHILE";tk[19]->follow[14] = "TK_RETURN";tk[19]->follow[15] = "#";
tk[20]->follow[0] = "TK_CALL";tk[20]->follow[1] = "#";
tk[21]->follow[0] = "TK_SEM";tk[21]->follow[1] = "#";
tk[22]->follow[0] = "TK_ID";tk[22]->follow[1] = "TK_IF";tk[22]->follow[2] = "TK_CALL";tk[22]->follow[3] = "TK_SQL";tk[22]->follow[4] = "TK_WHILE";tk[22]->follow[5] = "TK_READ";tk[22]->follow[6] = "TK_WRITE";tk[22]->follow[7] = "TK_OP";tk[22]->follow[8] = "TK_NOT";tk[22]->follow[9] = "TK_NUM";tk[22]->follow[10] = "RNUM";tk[22]->follow[11] = "TK_ENDIF";tk[22]->follow[12] = "TK_ELSE";tk[22]->follow[13] = "ENDWHILE";tk[22]->follow[14] = "TK_RETURN";tk[22]->follow[15] = "#";
tk[23]->follow[0] = "TK_ID";tk[23]->follow[1] = "TK_IF";tk[23]->follow[2] = "TK_CALL";tk[23]->follow[3] = "TK_SQL";tk[23]->follow[4] = "TK_WHILE";tk[23]->follow[5] = "TK_READ";tk[23]->follow[6] = "TK_WRITE";tk[23]->follow[7] = "TK_OP";tk[23]->follow[8] = "TK_NOT";tk[23]->follow[9] = "TK_NUM";tk[23]->follow[10] = "RNUM";tk[23]->follow[11] = "TK_ENDIF";tk[23]->follow[12] = "TK_ELSE";tk[23]->follow[13] = "ENDWHILE";tk[23]->follow[14] = "TK_RETURN";tk[23]->follow[15] = "#";
tk[24]->follow[0] = "TK_ID";tk[24]->follow[1] = "TK_IF";tk[24]->follow[2] = "TK_CALL";tk[24]->follow[3] = "TK_SQL";tk[24]->follow[4] = "TK_WHILE";tk[24]->follow[5] = "TK_READ";tk[24]->follow[6] = "TK_WRITE";tk[24]->follow[7] = "TK_OP";tk[24]->follow[8] = "TK_NOT";tk[24]->follow[9] = "TK_NUM";tk[24]->follow[10] = "RNUM";tk[24]->follow[11] = "TK_ENDIF";tk[24]->follow[12] = "TK_ELSE";tk[24]->follow[13] = "ENDWHILE";tk[24]->follow[14] = "TK_RETURN";tk[24]->follow[15] = "#";
tk[25]->follow[0] = "TK_ID";tk[25]->follow[1] = "TK_IF";tk[25]->follow[2] = "TK_CALL";tk[25]->follow[3] = "TK_SQL";tk[25]->follow[4] = "TK_WHILE";tk[25]->follow[5] = "TK_READ";tk[25]->follow[6] = "TK_WRITE";tk[25]->follow[7] = "TK_OP";tk[25]->follow[8] = "TK_NOT";tk[25]->follow[9] = "TK_NUM";tk[25]->follow[10] = "RNUM";tk[25]->follow[11] = "TK_ENDIF";tk[25]->follow[12] = "TK_ELSE";tk[25]->follow[13] = "ENDWHILE";tk[25]->follow[14] = "TK_RETURN";tk[25]->follow[15] = "#";
tk[26]->follow[0] = "TK_CL";tk[26]->follow[1] = "#";
tk[27]->follow[0] = "TK_CL";tk[27]->follow[1] = "TK_SEM";tk[27]->follow[2] = "#";
tk[28]->follow[0] = "TK_PLUS";tk[28]->follow[1] = "TK_MINUS";tk[28]->follow[2] = "TK_CL";tk[28]->follow[2] = "TK_SEM";tk[28]->follow[3] = "#";
tk[29]->follow[0] = "TK_MUL";tk[29]->follow[1] = "TK_DIV";tk[29]->follow[2] = "TK_PLUS";tk[29]->follow[3] = "TK_MINUS";tk[29]->follow[4] = "TK_CL";tk[29]->follow[5] = "TK_SEM";tk[29]->follow[6] = "#";
tk[30]->follow[0] = "TK_OP";tk[30]->follow[1] = "TK_ID";tk[30]->follow[2] = "TK_NUM";tk[30]->follow[3] = "TK_RNUM";tk[30]->follow[4] = "TK_RECORDID";tk[30]->follow[5] = "#";
tk[31]->follow[0] = "TK_OP";tk[31]->follow[1] = "TK_ID";tk[31]->follow[2] = "TK_NUM";tk[31]->follow[3] = "TK_RNUM";tk[31]->follow[4] = "TK_RECORDID";tk[31]->follow[5] = "#";
tk[32]->follow[0] = "TK_MUL";tk[32]->follow[1] = "TK_DIV";tk[32]->follow[2] = "TK_PLUS";tk[32]->follow[3] = "TK_MINUS";tk[32]->follow[4] = "TK_CL";tk[32]->follow[5] = "TK_SEM";tk[32]->follow[6] = "#";
tk[33]->follow[0] = "TK_MUL";tk[33]->follow[1] = "TK_DIV";tk[33]->follow[2] = "TK_PLUS";tk[33]->follow[3] = "TK_MINUS";tk[33]->follow[4] = "TK_CL";tk[33]->follow[5] = "TK_SEM";tk[33]->follow[6] = "#";
tk[34]->follow[0] = "TK_CL";tk[34]->follow[1] = "#";
tk[35]->follow[0] = "TK_LT";tk[35]->follow[1] = "TK_LE";tk[35]->follow[2] = "TK_EQ";tk[35]->follow[3] = "TK_GT";tk[35]->follow[4] = "TK_GE";tk[35]->follow[5] = "TK_NE";tk[35]->follow[6] = "TK_CL";
tk[35]->follow[7] = "#";
tk[36]->follow[0] = "TK_OP";tk[36]->follow[1] = "#";
tk[37]->follow[0] = "TK_ID";tk[37]->follow[1] = "TK_NUM";tk[37]->follow[2] = "TK_RNUM";tk[37]->follow[3] = "#";
tk[38]->follow[0] = "TK_END";tk[38]->follow[1] = "#";
tk[39]->follow[0] = "TK_SEM";tk[39]->follow[1] = "#";
tk[40]->follow[0] = "TK_SQR";tk[40]->follow[1] = "#";
tk[41]->follow[0] = "TK_SQR";tk[41]->follow[1] = "#";
tk[42]->follow[0] = "TK_COLON";tk[42]->follow[1] = "TK_ID";tk[42]->follow[2] = "#";
tk[43]->follow[0] = "TK_ENDRECORD";tk[43]->follow[1] = "#";
tk[44]->follow[0] = "TK_ID";tk[44]->follow[1] = "TK_IF";tk[44]->follow[2] = "TK_CALL";tk[44]->follow[3] = "TK_SQL";tk[44]->follow[4] = "TK_WHILE";tk[44]->follow[5] = "TK_READ";tk[44]->follow[6] = "TK_WRITE";tk[44]->follow[7] = "TK_OP";tk[44]->follow[8] = "TK_NOT";tk[44]->follow[9] = "TK_NUM";tk[44]->follow[10] = "TK_RNUM";tk[44]->follow[11] = "TK_ENDIF";tk[44]->follow[12] = "TK_ELSE";tk[44]->follow[13] = "TK_ENDWHILE";tk[44]->follow[14] = "TK_RETURN";tk[44]->follow[15] = "#";
tk[45]->follow[0] = "TK_END";tk[45]->follow[1] = "#";
tk[46]->follow[0] = "TK_ID";tk[46]->follow[1] = "TK_IF";tk[46]->follow[2] = "TK_CALL";tk[46]->follow[3] = "TK_SQL";tk[46]->follow[4] = "TK_WHILE";tk[46]->follow[5] = "TK_READ";tk[46]->follow[6] = "TK_WRITE";tk[46]->follow[7] = "TK_OP";tk[46]->follow[8] = "TK_NOT";tk[46]->follow[9] = "TK_NUM";tk[46]->follow[10] = "TK_RNUM";tk[46]->follow[11] = "TK_ENDIF";tk[46]->follow[12] = "TK_ELSE";tk[46]->follow[13] = "TK_ENDWHILE";tk[46]->follow[14] = "TK_RETURN";tk[46]->follow[15] = "#";
tk[47]->follow[0] = "TK_PLUS";tk[47]->follow[1] = "TK_MINUS";tk[47]->follow[2] = "TK_CL";tk[47]->follow[3] = "TK_SEM";tk[47]->follow[4] = "#";
tk[48]->follow[0] = "TK_SQR";tk[48]->follow[1] = "#";
tk[49]->follow[0] = "TK_CL";tk[49]->follow[1] = "TK_SEM";tk[49]->follow[2] = "#";
int i,j;

int *arr[50];
for(i=0;i<50;i++){
arr[i] = (int*) malloc(sizeof(int)*52*50);
 }


 for(yui=0;yui<50;yui++){
    for(hjk=0;hjk<52;hjk++){
       arr[yui][hjk] = 100;
}
}
createGramTab();
for(i=0;i<89;i++){
//printf("%d",i);
    int j = 0,k =0,a,n,p;
    int* pa;
    char rule_str[15];
    while(gr[i][j] != '<')j++;
    while((gr[i][j] != ' ') && (gr[i][j] != '=')){
       rule_str[k] = gr[i][j];
        k++;j++;
    }
    rule_str[k] = '\0';
   char* s  = rule_str;
printf("%s \n",s);
    a = getint(s);
  /*while((gr[i][j] == ' ') || (gr[i][j] == '=') || (gr[i][j] == '='))j++;
    if(gr[i][j] == 'e'){
      int ag =0;
                while(strcmp(tk[a]->follow[ag],"#") != 0){
                    int y;
                    y = getint(tk[a]->follow[ag]) -50;
                    arr[a][y] = i;
                     ag++;
}
//i++;
 continue;
}*/
if((i == 3)||(i == 7)||(i == 15)||(i == 18)||(i == 23)||(i == 25)||(i == 28)||(i == 30)||(i == 39)||(i == 42)||(i == 54)||(i == 57)||(i == 68)||(i == 85)||(i == 87)) {

int ag =0;
                while(strcmp(tk[a]->follow[ag],"#") != 0){
                    int y;
                    y = getint(tk[a]->follow[ag]) -50;
                    arr[a][y] = i;
                     ag++;
}
continue;
}
    n = all_not(gr[i],s,&pa);
    for(p=0;p<n;p++){
        if(all_tokens[pa[p]][0] == 'T'){
            int tid;
            tid = pa[p] - 50;
            arr[a][tid] = i;
            break;
        }
        else{
            int ntid;
            ntid = pa[p];
            if(has_null(pa[p]) == 0){
                int qw = 0;
                while(strcmp(tk[ntid]->first[qw],"#") != 0){
                    int x;
                    x = getint(tk[ntid]->first[qw]) -50;
                    arr[a][x] = i;
                    qw++;
                }
                break;
            }
            if(has_null(pa[p]) == 1){
                int qw = 0;
                while((strcmp(tk[ntid]->first[qw],"#") != 0) && (strcmp(tk[ntid]->first[qw],"")!= 0)){
                    int x;
                    x = getint(tk[ntid]->first[qw]) -50;
                    arr[a][x] = i;
                    qw++;
            }
           if(p == n-1){
               int ab =0;
                while(strcmp(tk[a]->follow[ab],"#") != 0){
                     int y;
                    y = getint(tk[a]->follow[ab]) -50;
                    arr[a][y] = i;
                    ab++;

                }
            }
            }


        }

    }





}

//createParseTable(arr);
printf("aaa");

for(i=0;i<50;i++){
for(j=0;j<52;j++){
  printf("  %d",arr[i][j]);
}
}
printf("jsdfkhlsadfjhjadsfkl");
fun();
return 0;
}

