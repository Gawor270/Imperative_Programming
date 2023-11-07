#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int *get(int cols, int row, int col, int *A) {
	return A + row*cols + col;
}

void set(int cols, int row, int col, int *A, int value) {
	*(A + row*cols + col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
	int sum;
	for(int i=0; i<rowsA; i++){
		for(int j=0; j<colsB; j++){
			sum =0;
			for(int k=0; k<colsA; k++){
				sum += *(get(colsA,i,k,A)) * (*(get(colsB,k,j,B)));
			}
			set(colsB,i,j,AB,sum);
		}
	}
}

void read_mat(int rows, int cols, int *t) {
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			scanf("%d", get(cols,i,j,t));
		}
	}
}

void print_mat(int rows, int cols, int *t) {
	for(int i=0; i<rows; i++){
		for(int j=0; j<cols; j++){
			printf("%d ",*(get(cols,i,j,t)));
		}
		printf("\n");
	}
}

int read_char_lines(char *array[]) {
	char Buff[BUF_SIZE];
	int len;
	char* point;
	int count =0;
	while(fgets(Buff,BUF_SIZE,stdin)){
		point = Buff;
		len =1;
		while(*point != '\n') {len++; point = point+1;}
		*point = '\0';
		char* new_el = (char*)malloc(len*sizeof(char));
		strcpy(new_el,Buff);
		array[count] = new_el;
		count++;
	}
	return count;
}

void write_char_line(char *array[], int n) {
	printf("%s", array[n]);
}

void delete_lines(char *array[],int size) {
	for(int i=0; i<size; i++){
		free(array[i]);
	}
}

int read_int_lines_cont(int *ptr_array[]) {
	char Buff[BUF_SIZE];
	int count = 0;
	int* pointer = ptr_array[0];
	while(fgets(Buff,BUF_SIZE,stdin) != NULL){
		char* i=Buff;
		int j=0;
		while(*i != '\n'){
			while(*i == ' ') i = i+1;
			*(pointer + j) = (int)strtol(i, NULL, 10);
			j++;
			while(*i != ' ' && *i != '\n') i = i+1; 
		}
		count++;
		ptr_array[count] = pointer + j;
		pointer = pointer + j;
	}
	return count;
}

void write_int_line_cont(int *ptr_array[], int n) {
	int *beg,*end;	
	beg = ptr_array[n];
	end = ptr_array[n+1];
	for(int* j =beg; j<end; j= j+1){
		printf("%d ", *j);
	}
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
	char Buff[BUF_SIZE];
	int count = 0;
	int j;
	float sum;
	char* i;
	while(fgets(Buff,BUF_SIZE,stdin) != NULL){
		line_type newline;
		newline.values = (int*)malloc(TAB_SIZE*sizeof(int));
		newline.len = 0;
		i=Buff;
		j=0;
		sum =0;
		while(*i != '\n'){
			while(*i == ' ') i = i+1;
			*(newline.values + j) = (int)strtol(i, NULL, 10);
			sum += *(newline.values +j);
			newline.len++;
			j++;
			while(*i != ' ' && *i != '\n') i = i+1; 
		}
		newline.average = sum/newline.len;
		lines_array[count] = newline;
		count++;
	}
	return count;
}

void write_int_line(line_type lines_array[], int n) {
	line_type el = lines_array[n];
	for(int i=0; i<el.len; i++){
		printf("%d ", *(el.values +i));
	}
	printf("\n%.2f", el.average);
}

void delete_int_lines(line_type array[], int line_count) {
	for(int i=0; i<line_count; i++){
		free(array[i].values);
	}
}

int cmp (const void *a, const void *b) {
	return ((line_type*)a)->average - ((line_type*)b)->average;
}

void sort_by_average(line_type lines_array[], int line_count) {
	qsort(lines_array,line_count,sizeof(line_type),cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
	for(int i=0; i<n_triplets; i++){
		scanf("%d %d %d",&(triplet_array +i)->r ,&(triplet_array +i)->c ,&(triplet_array +i)->v);
	}
	return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
	int res1 =((triplet*)t1)->r - ((triplet*)t2)->r;
	if(res1){
		return res1;
	}
	else return ((triplet*)t1)->c - ((triplet*)t2)->c;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
	int i,j,sum;
	qsort(triplet_array,n_triplets,sizeof(triplet),cmp_triplets);
	memset(R,0,(rows+1)*sizeof(int));
	for(i=0; i<n_triplets; i++){
		V[i] = (triplet_array+i)->v;
		C[i] = (triplet_array+i)->c;
		R[(triplet_array+i)->r]++;
	}
	for(i= rows; i>0; i--){
		sum =0;
		j=i;
		while(j--){sum += R[j];}
		R[i] = sum;
	}
	R[0] = 0;
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
	for(int i=0; i<rows; i++){
		y[i] = 0;
		for(int j=R[i]; j<R[i+1]; j++){
			y[i] += V[j]*x[C[j]];
		}
	}
}

void read_vector(int *v, int n) {
	for(int* it =v; it < v+n; it= it+1){
		scanf("%d",it);
	}
}

void write_vector(int *v, int n) {
	for(int *it = v; it< v+n; it = it+1){
		printf("%d ", *it);
	}
	printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB,size;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			size = read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array,size);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

