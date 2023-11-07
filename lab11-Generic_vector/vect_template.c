#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_STR_LEN 64

typedef struct Vector {
	void *data;
	size_t element_size;
	size_t size;
	size_t capacity;
} Vector;

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

typedef int(*cmp_ptr)(const void*, const void*);
typedef int(*predicate_ptr)(void*);
typedef void(*read_ptr)(void*);
typedef void(*print_ptr)(const void*);

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
	vector->data = malloc(block_size*element_size);
	vector->element_size = element_size;
	vector->size = 0;
	vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
	if(new_capacity > vector->capacity){
		vector->data = realloc(vector->data,new_capacity*vector->element_size +2);
		vector->capacity = new_capacity;
	}
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
	if(vector->size > new_size){
		// vector->data = realloc(vector->data,new_size*vector->element_size);
		// vector->capacity = new_size;
		vector->size = new_size;
	}
	else if(vector->size < new_size){
		if(vector->capacity < new_size)	reserve(vector,new_size);
		memset((char*)vector->data +vector->size*vector->element_size ,0, (new_size-vector->size)*vector->element_size);
		vector->size = new_size;
	}
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
	if(vector->size +1 > vector->capacity) reserve(vector,2*vector->capacity);
	memcpy((char*)vector->data + vector->size*vector->element_size,(char*)value,vector->element_size);
	vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
	vector->size = 0;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, size_t index, void *value) {
	if(vector->size+1 > vector->capacity) reserve(vector,2*vector->capacity);
	memmove((char*)vector->data + (index+1)*vector->element_size,(char*)vector->data + index*vector->element_size,(vector->size-index)*vector->element_size);
	memcpy((char*)vector->data + index*vector->element_size,(char*)value,vector->element_size);
	vector->size++;
}

// Erase element at position index
void erase(Vector *vector, size_t index) {
	memmove((char*)vector->data + index*vector->element_size,(char*)vector->data + (index+1)*vector->element_size,(vector->size -index -1)*vector->element_size);
	vector->size--;
}

// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, cmp_ptr cmp) {
	int i=0;
	int j=0;
	while(i<vector->size){
		j=i;
		while(!cmp((void*)((char*)vector->data + j*vector->element_size),value) && j<vector->size){
			j++;
		}
		memmove((char*)vector->data + i*vector->element_size,(char*)vector->data + j*vector->element_size,(vector->size-j)*vector->element_size);
		vector->size -=(j-i);
		i++;
	}
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
	int i=0;
	int j=0;
	while(i<vector->size){
		j=i;
		while(predicate((void*)((char*)vector->data + j*vector->element_size)) && j<vector->size){
			j++;
		}
		if(j!=i){
			memmove((char*)vector->data + i*vector->element_size,(char*)vector->data + j*vector->element_size,(vector->size-j)*vector->element_size);
			vector->size -=(j-i);
		}
		i++;
	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector) {
	if(vector->size < vector->capacity){
		vector->data = realloc(vector->data,vector->size*vector->element_size);
		vector->capacity = vector->size;
	}
}

// integer comparator
int int_cmp(const void *v1, const void *v2) {
	return *(int*)v1 - *(int*)v2;
}

// char comparator
int char_cmp(const void *v1, const void *v2) {
	return *(char*)v1 - *(char*)v2;
}

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2) {
	int res1 = ((Person*)p2)->age - ((Person*)p1)->age;
	if(!res1){
		int res2 =strcmp(((Person*)p1)->first_name,((Person*)p2)->first_name);
		if(!res2){
			return strcmp(((Person*)p1)->last_name,((Person*)p2)->last_name);
		}
		return res2;
	}
	return res1;
}

// predicate: check if number is even
int is_even(void *value) {
	return !((*(int*)value)&1);
}

char vowels[12] = {'A','a','E','e','I','i','O','o','U','u','Y','y'};
// predicate: check if char is a vowel
int is_vowel(void *value) {
	
	for(int i=0; i<12; i++){
		if(!char_cmp((void*)(vowels +i),value)){
			return 1;
		}
	}
	return 0;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person) {
	return ((Person*)person)->age >25;
}

// print integer value
void print_int(const void *v) {
	printf("%d ",*(int*)v);
}

// print char value
void print_char(const void *v) {
	printf("%c ",*(char*)v);
}

// print structure Person
void print_person(const void *v) {
	printf("%d %s %s\n",((Person*)v)->age,((Person*)v)->first_name,((Person*)v)->last_name);
}

// print capacity of the vector and its elements
void print_vector(Vector *vector, print_ptr print) {
	printf("%d\n", vector->capacity);
	for(int i=0; i<vector->size; i++){
		print((void*)((char*)vector->data + i*vector->element_size));
	}
}

// read int value
void read_int(void* value) {
	int x;
	scanf("%d",&x);
	memcpy(value,&x,sizeof(int));
}

// read char value
void read_char(void* value) {
	char c;
	scanf(" %c",&c);
	memcpy(value,&c,sizeof(char));
}

// read struct Person
void read_person(void* value) {
	Person p;
	scanf(" %d %s %s",&p.age,p.first_name,p.last_name);
	memcpy(value,&p,sizeof(Person));
}

void vector_test(Vector *vector, size_t block_size, size_t elem_size, int n, read_ptr read,
		 cmp_ptr cmp, predicate_ptr predicate, print_ptr print) {
	init_vector(vector, block_size, elem_size);
	void *v = malloc(vector->element_size);
	size_t index, size;
	for (int i = 0; i < n; ++i) {
		char op;
		scanf(" %c", &op);
		switch (op) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%zu", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%zu", &index);
				erase(vector, index);
				break;
			case 'v': // erase
				read(v);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
	print_vector(vector, print);
	free(vector->data);
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			vector_test(&vector_int, 4, sizeof(int), n, read_int, int_cmp,
				is_even, print_int);
			break;
		case 2:
			vector_test(&vector_char, 2, sizeof(char), n, read_char, char_cmp,
				is_vowel, print_char);
			break;
		case 3:
			vector_test(&vector_person, 2, sizeof(Person), n, read_person,
				person_cmp, is_older_than_25, print_person);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}

