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


enum Drive_type {combustion, electic, d_ndef};
enum Fuel_type {gasoline, diesel, gas, hydrogen, f_d_ndef};
enum Baterry_type {Li_ion, Na_Ion, NiMH, b_d_ndef};

typedef struct{
        char emmision[15];
        const enum Fuel_type fuel_t;
}Comb_data;     

typedef struct{
        enum Baterry_type bat_t;
        float capacity;
}El_data;

typedef struct {int d; int m; int y;} Date;

typedef struct{
        char owner[30];
        Date valid_date;
        enum Drive_type d_type;
        union{
                Comb_data cs;
                El_data es;
        }drv_union;

}Vehicle;

int comp_dates(Date* date1, Date* date2){
    if(date1->y == date2->y){
        if(date1->m == date2->m){
            if(date1->d == date2->d){
                return 0;
            }
            return date1->d - date2->d;
        }
        return date1->m - date2->m;
    }
    return date1->y -date2->y;
}

int delayed(const Vehicle* tv,int size, const Date* base_date,  char*** pdelayed_owners){
    int counter =0;
    char** delayed_owners = (char**)malloc(sizeof(char*));

    for(int i=0; i<size; i++){
        if(comp_dates(&tv[i].valid_date,base_date) <0 ){
            delayed_owners = (char**)safe_realloc(delayed_owners,(counter+1)*sizeof(char*));
            delayed_owners[counter] = tv[i].owner;
            counter++;
        }
    }
    if(counter){
        pdelayed_owners = &delayed_owners;
        return counter;
    }
    else{
        free(delayed_owners);
        pdelayed_owners = NULL;
        return 0;
    }
}

void print_owners(const char **t_owners, size_t size){
        for(int i=0; i<size; i++){
                printf("%s\n", t_owners[i]);
        }
}

void init_vehicle(Vehicle *v){
        v = (Vehicle*)safe_malloc(sizeof(Vehicle));
        v->owner[0] = '\0';
        v->valid_date.d = 1;
        v->valid_date.m = 1;
        v->valid_date.y = 1970;
        v->d_type = d_ndef;
}

void print_drive_data(const Vehicle *v){
        switch (v->d_type)
        {
        case combustion:
                printf("Naped: cs, rodzaj paliwa: ");
                switch (v->drv_union.cs.fuel_t)
                {
                case gasoline:
                        printf("gasoline, ");
                        break;
                
                case diesel:
                        printf("diesel, ");
                        break;
                
                case gas:
                        printf("gas, ");
                        break;

                case hydrogen:
                        printf("hydrogen, ");
                        break;

                case f_d_ndef:
                        break;

                default:
                        break;
                }

                printf("emisja, %s",v->drv_union.cs.emmision);
             
                break;
        
        case electic:
                printf("Naped: el, bateria: ");
                switch (v->drv_union.es.bat_t)
                {
                case Li_ion:
                        printf("Li_ion, ");
                        break;
                
                case Na_Ion:
                        printf("Na_Ion, ");
                        break;
                
                case NiMH:
                        printf("NiMH, ");
                        break;

                case b_d_ndef:
                        break;

                default:
                        break;
                }

                printf("pojemność, %.2fkWH",v->drv_union.es.capacity);

                break;

        
        case d_ndef:
                break;

        default:
                break;
        }
}

void new_owner(Vehicle *v, const char *str){
        memcpy(v->owner,str, 30*sizeof(char));
        free(str);

}

int main(void){
        return 0;
}