#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define NUMOFCARDS 52
#define NUMOFCOLORS 4


int cbuffA[NUMOFCARDS];
int outA=0, lenA=0;

int cbuffB[NUMOFCARDS];
int outB=0, lenB=0;

void swap(int *xp, int *yp);
int rand_from_interval(int a, int b);
void rand_permutation(int n, int tab[]);

double cbuffA_push(int cli_nr);
int cbuffA_pop(void);
int cbuffA_state(void);
void cbuffA_print(void);

double cbuffB_push(int cli_nr);
int cbuffB_pop(void);
int cbuffB_state(void);
void cbuffB_print(void);

double cbuff_push(int cli_nr);
int cbuff_pop(void);
int cbuff_state(void);
void cbuff_print(void);

void print(int t[], int n){
    for(int i=0; i<n; ++i){
        printf("%d ", t[i]);
    }
    printf("\n");
}

int counter;

int war(int code, int maxim){
    int cards[NUMOFCARDS];
    rand_permutation(NUMOFCARDS, cards);
    for(int i=0; i<NUMOFCARDS/2; i++){
        cbuffA_push(cards[i]);
        cbuffB_push(cards[NUMOFCARDS/2+i]);
    }

    if(code==1){
        while(counter < maxim){
            counter++;
            int tmpA = cbuffA_pop();
            int tmpB = cbuffB_pop();
            if((tmpA/NUMOFCOLORS)==(tmpB/NUMOFCOLORS)){
                cbuffA_push(tmpA);
                cbuffB_push(tmpB);
            }
            else if((tmpA/NUMOFCOLORS)>(tmpB/NUMOFCOLORS)){
                cbuffA_push(tmpA);
                cbuffA_push(tmpB);
            }
            else{
                cbuffB_push(tmpB);
                cbuffB_push(tmpA);
            }
            if(lenA==NUMOFCARDS){
                return 2;
            }
            if(lenB==NUMOFCARDS){
                return 3;
            }
        }
    }
    else{
        while(counter < maxim){
            int i = 0;
            while(i<lenA && i<lenB && counter<maxim){
                counter++;
                int A = cbuffA[(outA+i)%NUMOFCARDS]/NUMOFCOLORS;
                int B = cbuffB[(outB+i)%NUMOFCARDS]/NUMOFCOLORS;
                if(A > B){
                    for(int j=0; j<i+1; j++){
                        cbuffA_push(cbuffA_pop());
                    }
                    for(int j=0; j<i+1; j++){
                        cbuffA_push(cbuffB_pop());
                    }
                    break;
                }
                else if(A < B){
                    for(int j=0; j<i+1; j++){
                        cbuffB_push(cbuffB_pop());
                    }
                    for(int j=0; j<i+1; j++){
                        cbuffB_push(cbuffA_pop());
                    }
                    break;
                }
                else{
                    i+=2;
                }
            }
            if(lenA == NUMOFCARDS){
                return 2;
            }
            if(lenB == NUMOFCARDS){
                return 3;
            }
            if(i >=lenA || i >=lenB){
                return 1;
            }
        }
    }
    return 0;
}
    

    
    



int main(void) {
    int seed, code, maxim;
    scanf("%d %d %d", &seed, &code, &maxim);
    srand(seed);
    int result = war(code,maxim);
    if(result ==3)printf("%d \n", result);
    else printf("%d ", result);
    switch(result){
        case(0):
            printf("%d %d", lenA, lenB);
            break;
        case(1):
            printf("%d %d", lenA, lenB);
            break;
        case(2):
            printf("%d", counter);
            break;
        case(3):
            cbuffB_print();
            break;
    }
    return 0;
}

void swap(int *xp, int *yp){                                                    
    int temp = *yp;                                                             
    *yp = *xp;                                                                  
    *xp = temp;                                                                 
}          

// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN
// Jezeli b-a > RAND_MAX to zwraca INT_MAX
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b) {
        if(a>b) return INT_MIN;
        if(b-a > RAND_MAX) return INT_MAX;
        if(a==b) return a;
        return a + rand()%(b-a+1);
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]) {
    for(int i=0; i<n; ++i){                                                     
        tab[i] = i;                                                             
    }                                                                           
    for(int i=0; i<n-1; ++i){                                                   
        int k = rand_from_interval(i,n-1);                                      
        swap(&tab[i],&tab[k]);                                                  
    }              
}

//Kolejka w buforze cyklicznym


double cbuffA_push(int cli_nr) {
    if(lenA==NUMOFCARDS)return INFINITY;
    cbuffA[(outA+lenA)%NUMOFCARDS] = cli_nr;
    lenA++;
    return 0.0;
}

int cbuffA_pop(void) {
    if(lenA==0)return -1;
    int tmp = cbuffA[outA];
    cbuffA[outA] = 0.0;
    outA = (outA+1)%NUMOFCARDS;
    lenA--;
    return tmp;
}

int cbuffA_state(void) {
    return lenA;
}

void cbuffA_print(void) {
    for(int i=outA; i<outA+lenA; i++){
        printf("%d ", cbuffA[i%NUMOFCARDS]);
    }
}

double cbuffB_push(int cli_nr) {
    if(lenB==NUMOFCARDS)return INFINITY;
    cbuffB[(outB+lenB)%NUMOFCARDS] = cli_nr;
    lenB++;
    return 0.0;
}

int cbuffB_pop(void) {
    if(lenB==0)return -1;
    int tmp = cbuffB[outB];
    cbuffB[outB] = 0.0;
    outB = (outB+1)%NUMOFCARDS;
    lenB--;
    return tmp;
}

int cbuffB_state(void) {
    return lenB;
}

void cbuffB_print(void) {
    for(int i=outB; i<outB+lenB; i++){
        printf("%d ", cbuffB[i%NUMOFCARDS]);
    }
}