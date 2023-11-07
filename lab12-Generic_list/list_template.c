#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

struct List;
typedef void (*DataFp)(void*);
typedef void (*ConstDataFp)(const void*);
typedef  int (*CompareDataFp)(const void*, const void*);

typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	DataFp modify_data;
} List;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	printf("malloc error\n");
	exit(EXIT_FAILURE);
}

void *safe_strdup(const char *string) {
	void *ptr = strdup(string);
	if(ptr) return ptr;
	printf("strdup error\n");
	exit(EXIT_FAILURE);
}

// --- generic functions --- for any data type

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
		CompareDataFp compare_data, DataFp modify_data) {
	
	p_list->dump_data = dump_data;
	p_list->free_data = free_data;
	p_list->compare_data = compare_data;
	p_list->modify_data = modify_data;
	p_list->tail = NULL;
	p_list->head = NULL;
	
}

// Print elements of the list
void dump_list(const List* p_list) {
	ListElement* ptr = p_list->head;
	while(ptr != NULL){
		p_list->dump_data(ptr->data);
		ptr = ptr->next;	
	}
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data) {
	ListElement* ptr = p_list->head;
	while(ptr != NULL){
		if(!p_list->compare_data(ptr->data,data))p_list->dump_data(ptr->data);
		ptr = ptr->next;	
	}	
}

// Free element pointed by data using free_data() function
void free_element(DataFp free_data, ListElement *to_delete) {
	free_data(to_delete);
}

// Free all elements of the list
void free_list(List* p_list) {
	ListElement* ptr = p_list->head;
	ListElement* todel;
	while(ptr != NULL){
		todel = ptr;
		ptr = ptr->next;	
		p_list->free_data(todel->data);
		free(todel);

	}	
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data){
	ListElement* new_el = (ListElement*)malloc(sizeof(ListElement));
	new_el->data = data;
	if(p_list->head == NULL)
	{
		p_list->head = new_el;
		p_list->tail = p_list->head;
		p_list->tail->next = NULL;
	}
	else
	{
		new_el->next = p_list->head;
		p_list->head = new_el;
	}
}

// Push element at the end of the list
void push_back(List *p_list, void *data) {
	ListElement* new_el = (ListElement*)malloc(sizeof(ListElement));
	new_el->data = data;
	new_el->next = NULL;
	if(p_list->head == NULL)
	{
		p_list->head = new_el;
		p_list->tail = new_el;
	}
	else
	{
		p_list->tail->next  = new_el;
		p_list->tail = new_el;
	}
}

// Remove the first element
void pop_front(List *p_list) {
	if(p_list->head){
		ListElement* tmp = p_list->head;
		p_list->head = p_list->head->next;
		tmp->next = NULL;
		p_list->free_data(tmp);
	}
}

// Reverse the list
void reverse(List *p_list) {
	if(p_list->head == NULL || p_list->head->next == NULL)return;
	ListElement* tmp1;
	ListElement* tmp2;
	ListElement* tmp3;

	tmp1 = p_list->head;
	tmp2 = p_list->head->next;
	tmp3 = p_list->head->next->next;
	p_list->head->next = NULL;
	while(tmp3 != NULL){
		tmp2->next = tmp1;
		tmp1 = tmp2;
		tmp2 = tmp3;
		tmp3 = tmp3->next;
	}
	p_list->tail = p_list->head;
	p_list->head = tmp2;
	p_list->head->next = tmp1; 
}

// find element in sorted list after which to insert given element
ListElement* find_insertion_point(const List *p_list, ListElement *p_element) {
	if(p_list->head == NULL || (p_list->compare_data(p_element,p_list->head->data) <0)){
		return NULL;
	}
	ListElement* ptr = p_list->head;
	while(ptr->next != NULL && p_list->compare_data(p_element,ptr->next->data) >0){
		ptr = ptr->next;
	}
	if(ptr->next != NULL && p_list->compare_data(p_element,ptr->next->data) ==0){
		return ptr->next;
	}
	else{
		return ptr;
	}
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous) {
	ListElement* new_el = (ListElement*)malloc(sizeof(ListElement));
	new_el->data = data;
	if(previous){
		if(previous->next){
			ListElement* tmp = previous->next;
			previous->next = new_el;
			new_el->next = tmp;
		}
		else{
			previous->next = new_el;
			new_el->next = NULL;
			p_list->tail = new_el;
		}
	}
	else{
		new_el->next = p_list->head;
		p_list->head = new_el;
		p_list->tail = new_el;
	}
}

// Insert element preserving order
void insert_in_order(List *p_list, void *p_data) {
	ListElement* ptr = find_insertion_point(p_list,p_data);
	if(ptr != NULL && p_list->compare_data(((ListElement*)ptr)->data,p_data) ==0 ){
		if(p_list->modify_data){
			p_list->modify_data(((ListElement*)ptr)->data);
		}
	}
	else{
		push_after(p_list,p_data,ptr);
	}
}

// -----------------------------------------------------------
// --- type-specific definitions

// int element

void dump_int(const void *d) {
	printf("%d ", *(int*)d);
}

void free_int(void *d) {
	free((int*)d);
}

int cmp_int(const void *a, const void *b) {
	return *(int*)a - *(int*)b;
}

int *create_data_int(int v) {
	int* p = (int*)malloc(sizeof(int));
	memcpy(p,&v,sizeof(int));
	return p;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word (const void *d) {
	printf("%s ", ((DataWord*)d)->word);
}

void free_word(void *d) {
	free((char*)d);
}

int cmp_word_alphabet(const void *a, const void *b) {
	return strcmp(((DataWord*)a)->word,((DataWord*)b)->word);
}

int cmp_word_counter(const void *a, const void *b) {
	return ((DataWord*)a)->counter - ((DataWord*)b)->counter;
}

void modify_word(void *p) {
	((DataWord*)p)->counter++;
}

void *create_data_word(const char *string, int counter) {
	DataWord* new_word = (DataWord*)malloc(sizeof(DataWord));
	new_word->counter = counter;
	new_word->word = strdup(string);
	return new_word;
}

// read text, parse it to words, and insert those words to the list.
// Order of insertions is given by the last parameter of type CompareDataFp.
// (comparator function address). If this address is not NULL the element is
// inserted according to the comparator. Otherwise, read order is preserved.
void stream_to_list(List *p_list, FILE *stream, CompareDataFp cmp) {
	p_list->compare_data = cmp_word_alphabet;
	char buff[BUFFER_SIZE];
	char delimiters[] = " \n\t\r\v\f.,?!:;-";
	char* tmp;
	while(fgets(buff,BUFFER_SIZE,stream)){
			
		tmp = strtok(buff,delimiters);
		while(tmp != NULL){
			if(cmp){
				char* p = tmp;
				for ( ; *p; ++p){
					if(*p < 97){
						*p = *p +32;
					}
				} 
				void* n_el = create_data_word(tmp,1);
				insert_in_order(p_list,n_el);
			}
			else{
				void* n_el = create_data_word(tmp,1);
				push_back(p_list,n_el);
			}
			tmp = strtok(NULL,delimiters);
		}
	}
}

// test integer list
void list_test(List *p_list, int n) {
	char op;
	int v;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		switch (op) {
			case 'f':
				scanf("%d", &v);
				push_front(p_list, create_data_int(v));
				break;
			case 'b':
				scanf("%d", &v);
				push_back(p_list, create_data_int(v));
				break;
			case 'd':
				pop_front(p_list);
				break;
			case 'r':
				reverse(p_list);
				break;
			case 'i':
				scanf("%d", &v);
				insert_in_order(p_list, create_data_int(v));
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	List list;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer list
			scanf("%d",&n);
			init_list(&list, dump_int, free_int, cmp_int, NULL);
			list_test(&list, n);
			dump_list(&list);
			free_list(&list);
			break;
		case 2: // read words from text, insert into list, and print
			init_list(&list, dump_word, free_word, NULL, NULL);
			stream_to_list(&list, stdin, NULL);
			dump_list(&list);
			free_list(&list);
			break;
		case 3: // read words, insert into list alphabetically, print words encountered n times
			scanf("%d",&n);
			init_list(&list, dump_word, free_word, NULL, modify_word);
			stream_to_list(&list, stdin, cmp_word_alphabet);
			list.compare_data = cmp_word_counter;
			DataWord data = { NULL, n };
			dump_list_if(&list, &data);
			free_list(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

