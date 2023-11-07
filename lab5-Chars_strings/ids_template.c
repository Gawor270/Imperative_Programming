#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define OUTSIDE 0
#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8


#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define MAX_LINE 128

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int is_alpha(char c){
	if((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 95) return 1;
	return 0;
}

int notcontains(char* string, int ind,int leng){
	int flag = 1;
	string[leng] = '\0';
	for(int j=0; j<32; j++){
		if(strlen(keywords[j]) == leng){
			if(!strcmp(string,keywords[j])){
				flag=0;
				break;
			}
		}
	}
	if(flag){
		for(int k=0 ; k<ind+1; k++){
			if(strlen(identifiers[k]) == leng){
			// printf("%lld %d \n", strlen(identifiers[k]), leng);
				if(!strcmp(string,identifiers[k])){
					flag =0;
					break;
				}
			}
		}
	}
	return flag;
}

int find_idents() {
	int i,leng;
	char buffor[MAX_LINE];
	char indbuff[MAX_LINE];
	int state = OUTSIDE;
	int ind=0;
	int counter=0;
	while(fgets(buffor, MAX_LINE, stdin) != NULL){
		i=0;
		leng =0;
		while(buffor[i]){
			switch (state)
			{
			case OUTSIDE:
				if(buffor[i]==39)i+=3;
				else if(is_alpha(buffor[i]) /*&& (buffor[i-1] == ' ' || buffor[i-1] == '	' || buffor[i-1] == '(' || buffor[i-1]==',' || buffor[i-1] == '*')*/){
					indbuff[leng] = buffor[i];
					i++;
					leng++;
					state = IN_ID;
				}
				else if(buffor[i] == '"'){
					state = IN_STRING;
					i++;
				}
				else if(i+1 < MAX_LINE && buffor[i] == '/' && buffor[i+1] == '/'){
					state = IN_LINE_COMMENT;
					i+=2;
				}
				else if(i+1 < MAX_LINE && buffor[i] == '/' && buffor[i+1] == '*'){
					state = IN_BLOCK_COMMENT;
					i+=2;
				}
				else i++;
				break;
			case IN_BLOCK_COMMENT:
				if(i+1 < MAX_LINE && buffor[i] == '*' && buffor[i+1] == '/'){
					state = 0;
					i += 2;
				}
				else{
					i++;
				}
				break;
			
			case IN_STRING:
				if(buffor[i-1] != 92 && buffor[i] == '"'){
					state=0;
				}
				i++;
				break;
			
			case IN_ID:
				if(is_alpha(buffor[i]) || (buffor[i] >= 48 && buffor[i] <=57)){
					indbuff[leng]=buffor[i];
					leng++;
					i++;
				}
				else if(notcontains(indbuff ,ind ,leng )){
					for(int j=0; j<leng; j++){
						identifiers[ind][j] = indbuff[j];
					}
					identifiers[ind][leng] = '\0';
					ind++;
					counter++;			
					leng =0;
					state =0;
				}
				else{
					leng =0;
					state=0;
				}
				break;
			case IN_LINE_COMMENT:
				i = MAX_LINE-1;
				state =0;
				break;
			default:
				break;
			}
		}
	}	
	return counter;
}


int main(void) {
	// printf("%d",is_alpha('-'));
	printf("%d\n", find_idents());
	return 0;
}

