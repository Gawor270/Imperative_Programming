#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS 94
#define MAX_BUFF_SIZE 1024

char delimiters[] = " \n\t\r\v\f.,?!:;-";

void swap(int *xp, int *yp){                                                    
    int temp = *yp;                                                             
    *yp = *xp;                                                                  
    *xp = temp;                                                                 
}

void fswap(double* xp, double* yp){
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

// read double vector of size n
void read_vector(double v[], int n) {
        for (int i = 0; i < n; ++i) {
                scanf("%lf", v + i);
        }
}

// print double vector of size n (with 2 figures after the decimal point)
void print_vector(const double v[], int n) {
        for (int i = 0; i < n; ++i) {
                printf("%.2f ", (double)v[i]);
        }
        printf("\n");
}

void append_to_file(char* data){
        FILE* fptr = fopen("file.txt","a");

        fputs(data,fptr);

        fclose(fptr);
}
//funkcje do stringow
// pamietaj ze string to nic innego jak tablica charow ktory konczy sie pierwszym elementem \0 
/* (int)strtol(line, NULL, 10); - zamiana string na postac liczbowa, ostatni argument to podstawa systemu
   strlen(char* string) - dlugosc stringa
   strcmp(char* string1, char* string2) - porownywanie stringow
   stricmp(char* string1, char* string2) - prownywanie stringow nie biorac pod uwage wielkosci liter 
*/

typedef struct MatTag{
        int** data;
        int rows;
        int cols;
} Mat;

typedef struct {
	int first;
	int second;
} Pair;


//Linked list
typedef struct ListElement {
	struct ListElement *next;
	void *data;
} ListElement;

typedef struct {
	ListElement *head;
	ListElement *tail;
} List;

void init_list(List* p_list){
        ListElement* new_el = (ListElement*)safe_malloc(sizeof(ListElement));
        p_list->head = new_el;
        p_list->tail = new_el;
}

void dump_list(List* list){
        ListElement* curr = list->head;
        while(curr){
                printf("%.2f ",*((double*)curr->data));
        }
}
// solution

typedef struct {
        char* name;
        char* surname;
        char pesel[12];
}Person;

typedef struct{
        Person* list;
        int len;       
}PersonList;

void init_pl(PersonList* p_pl){
        p_pl = (PersonList*)safe_malloc(sizeof(PersonList));
        p_pl->len = 0;
}

void free_pl(PersonList *p_pl){
        for(int i=0; i<p_pl->len; i++){
                free(p_pl->list[i].name);
                free(p_pl->list[i].surname);
                free(p_pl->list[i].pesel);
        }
        free(p_pl->list);
        free(p_pl);
}

void fill_person_data(Person* p_p,const char *pesel, char *name,const char *surname){
        p_p = (Person*)safe_malloc(sizeof(Person));
        strcpy(p_p->pesel,surname);
        p_p->name = name;
        p_p->surname = surname;
}

Person* find(const PersonList *p_pl, const char *pesel){
        Person* curr = p_pl->list;
        for(int i=0; i<p_pl->len; i++){
                if(!strcmp(pesel,curr->pesel)){
                        return curr;
                }
                curr++;
        }
        return NULL;

}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname){
        Person* ptr = find(p_pl,pesel);
        if(ptr == NULL){
                p_pl->len++;
                p_pl->list = (Person*)safe_realloc(p_pl->list,p_pl->len);
                fill_person_data(p_pl->list + p_pl->len-1,pesel,name,surname);
        }
        ptr->name = name;
        ptr->surname = surname;
        return 1;
}


void merge(PersonList *p_target, const PersonList *p_src){
        for(int i=0; i<p_src->len; i++){
                add(p_target,p_src->list[i].pesel,p_src->list[i].name,p_src->list[i].surname);
        }
}

int main(void){
        Person p1;
        p1.name = "heniek";
        p1.surname = "dasfdsa";

        Person p2;
        p2.name = "sdafasd";
        p2.surname = "sadfadfa";

        return 0;
}