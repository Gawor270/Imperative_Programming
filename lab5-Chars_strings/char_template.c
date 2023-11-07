#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS 94
#define MAX_BIGRAMS 8836

#define NEWLINE '\n'
#define IN_WORD 1
#define MAX_LINE 128

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS];
int charcount[MAX_CHARS];
char input[MAX_LINE];


// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if(charcount[va] == charcount[vb]) return va - vb;
	return charcount[vb] - charcount[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}


// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
	int i,len;
	*nl=0;
	*nw=0;
	*nc=0;
	while(fgets(input, MAX_LINE, stdin) != NULL){
		/*i=0;
		while(input[i] != '\0'){
			printf("%d ", input[i]);
			i++;
		}
		printf("\n");*/
		//if(input[0] == 10 && input[1] == '\0') break;	
		*nl = *nl+1;
		i=0;
		len =0;
		while(input[i] != 10){
			*nc = *nc +1;
			if(!(input[i] == 32 || input[i] == 9 ))len++;
			else if( len>=1 && i!=0 && (input[i] == 32 || input[i] == 9)){
				*nw = *nw+1;
				len=0;
			}
			else len=0;
			i++;
		}
		*nc = *nc +1;
		if(len>=1) *nw = *nw+1;
	}
}


void char_count(int char_no, int *n_char, int *cnt) {
	int i;
	while(fgets(input, MAX_LINE, stdin) != NULL){
		i=0;
		while(input[i]){
			if((input[i]>=FIRST_CHAR && input[i]<LAST_CHAR))
				charcount[input[i]-FIRST_CHAR]++;
			i++;
		}	
	}
	int permtab[MAX_CHARS];
	for(i=0; i<MAX_CHARS; i++){
		permtab[i] = i;
	}
	qsort(permtab,MAX_CHARS,sizeof(permtab[0]),cmp);
	//quicksort(cpy, permtab,0,MAX_CHARS);
	*n_char = permtab[char_no-1]+FIRST_CHAR;
	*cnt =  charcount[permtab[char_no-1]];
}

void bigram_count(int bigram_no, int bigram[]) {
	int i;
	while(fgets(input, MAX_LINE, stdin)){
		i=0;
		while(input[i+1]){
			if((input[i]>=FIRST_CHAR && input[i]<LAST_CHAR) && (input[i+1]>=FIRST_CHAR && input[i+1]<LAST_CHAR)){
				count[(int)input[i+1] + MAX_CHARS*((int)input[i]) -(FIRST_CHAR*(1+MAX_CHARS))]++;
			}
			i++;
		}
	}
	int permtabd[MAX_BIGRAMS];
	for(int i=0; i<MAX_BIGRAMS; i++){
		permtabd[i] = i;
	}
	qsort(permtabd,MAX_BIGRAMS,sizeof(permtabd[0]),cmp_di);
	/*for(int i=0; i<MAX_BIGRAMS; i++){
		int firstch = (i+(FIRST_CHAR*MAX_CHARS))%MAX_CHARS +33;
		int secondch = (i+(FIRST_CHAR*MAX_CHARS))/MAX_CHARS;
		int sum = i+(FIRST_CHAR*(1+MAX_CHARS));
		if(count[i]>0) printf("%c%c %d : %d %d | %d \n",firstch,secondch,sum,firstch,secondch, count[i]);
	}
	for(int i=0; i<MAX_BIGRAMS; i++){
		printf("%c%c ", (permtabd[i] + (FIRST_CHAR*MAX_CHARS))%MAX_CHARS +33 , (permtabd[i]+ (FIRST_CHAR*MAX_CHARS))/MAX_CHARS);
	}*/
	bigram[1] = (permtabd[bigram_no-1] + (FIRST_CHAR*MAX_CHARS))%MAX_CHARS +33;
	bigram[0] = (permtabd[bigram_no-1]+ (FIRST_CHAR*MAX_CHARS))/MAX_CHARS;
	bigram[2] = count[permtabd[bigram_no-1]];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
			*line_comment_counter = 0;
			*block_comment_counter = 0;
			int i;
			int state = 0;
			while(fgets(input, MAX_LINE, stdin) != NULL){
				i=0;
				while(input[i]){
					if(state == IN_BLOCK_COMMENT && input[i] == '*' && input[i+1] == '/'){
						state = 0;
						i+=2;
					} 
					if(state != IN_BLOCK_COMMENT && state != IN_LINE_COMMENT){
						if(input[i] == '/' && input[i+1] == '/'){
							*line_comment_counter = *line_comment_counter+1; 
							break;
						}
						if(input[i] == '/' && input[i+1] == '*'){
							*block_comment_counter = *block_comment_counter+1;
							state = IN_BLOCK_COMMENT;
						}
					}
					i++;
				}
			}
}


int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}



int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];
	

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc(&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n",n_char ,cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}