#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

int find_idents() {
	int i,leng;
	int counter=0;
	char buffor[MAX_LINE];
	char indbuff[MAX_LINE];
	int blockcom =0;
	int quote = 0;
	int ind=0;
	while(fgets(buffor, MAX_LINE, stdin) != NULL){
		i=0;
		leng =0;
		while(buffor[i]){
			if(!blockcom && !quote && buffor[i] == '#') break;
			if(blockcom && !quote && i+1 <MAX_LINE && buffor[i] == '*' && buffor[i+1] == '/'){
				blockcom = 0;
				i+=2;
			}
			else if(is_alpha(buffor[i]) && !blockcom && !quote){
				indbuff[leng] = buffor[i];
				i++;
				leng++;
				while(is_alpha(buffor[i]) || (buffor[i] >= 48 && buffor[i] <=57) ){
					indbuff[leng]=buffor[i];
					i++;
					leng++;
				}
				int flag = 1;
				indbuff[leng] = '\0';
				for(int j=0; j<32; j++){
					if(strlen(keywords[j]) == leng){
						if(!strcmp(indbuff,keywords[j])){
							flag=0;
							break;
						}
					}
				}
				if(flag){
					for(int k=0 ; k<ind+1; k++){
						if(strlen(identifiers[k]) == leng){
						// printf("%lld %d \n", strlen(identifiers[k]), leng);
							if(!strcmp(indbuff,identifiers[k])){
								flag =0;
								break;
							}
						}
					}
				}
				if(flag){
					for(int j=0; j<leng; j++){
						identifiers[ind][j] = indbuff[j];
					}
					ind++;
					counter++;
					// printf("%s %d %d \n", indbuff, leng, ind);
				}
				leng =0;
				// printf("\n");
			}
			else if(i+1 < MAX_LINE && buffor[i] == '/' && buffor[i+1] == '/' && !blockcom && !quote) break;
			else if(buffor[i] == '"' && !blockcom){
				if(quote){
					quote = 0;
				}
				else{
					quote = 1;
				}
				i++;
			}
			else if(!blockcom && !quote && i+1 < MAX_LINE && buffor[i] == '/' && buffor[i+1] == '*'){
				blockcom = 1;
				i+=2;
			}
			else i++;
		}	
	}
	for(int i=0; i<ind; i++){
		printf("%s \n", identifiers[i]);
	}
	return counter;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	// printf("%d",is_alpha('-'));
	printf("%d\n", find_idents());
	return 0;
}