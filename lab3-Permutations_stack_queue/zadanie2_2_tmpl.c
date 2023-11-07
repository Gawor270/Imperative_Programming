#include <stdio.h>
#include <math.h>

#define TEST    0        // 1 dla testowania, 0 dla oceny automatycznej

// Deklaracje (prototypy) funkcji:
double stack_push(double x);
double stack_pop(void);
int stack_state(void);

double queue_push(int how_many);
int queue_pop(int how_many);
int queue_state(void);
void queue_print(void);

double cbuff_push(int cli_nr);
int cbuff_pop(void);
int cbuff_state(void);
void cbuff_print(void);

void print(double t[], int n){
    for(int i=0; i<n; ++i){
        printf("%lf ", t[i]);
    }
    printf("\n");
}
///////////////////////

int main(void) {
    int to_do, i, n, ans;
    int nr_klienta, kod_operacji;
    double x,f_ans;
    cbuff_push(5);
    cbuff_push(7);
    cbuff_push(10);
    cbuff_print();
    if(TEST) printf("Nr testu = ");
    scanf("%d",&to_do);
    switch(to_do) {
case 1:
        if(TEST) printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan stosu i konczy sekwencje operacji
        do {
            scanf("%lf",&x);
            if(x > 0) {
                f_ans = stack_push(x);
                if(f_ans == INFINITY) printf("%d ", -2);
            }
            else {
                if(x < 0) printf("%d ",(int) stack_pop());
                else      printf("\n%d\n",stack_state());
            }
        } while(x != 0.0);
        break;
case 2:
//  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) - implementacja w tablicy z przesunieciami
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        do {
            scanf("%d",&n);
            if(n > 0) {
                f_ans = queue_push(n);
                if(f_ans== INFINITY) printf("%d ",-2);
            }
            else {
                if(n < 0) {
                    ans = queue_pop(-n);
                    if(ans < 0) printf("-1 ");
                }
                else  {
                    printf("\n%d\n",queue_state());
                    queue_print();
                }
            }
        } while(n != 0);
        break;
case 3:
//  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo - implementacja w buforze cyklicznym
        if(TEST) printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n");  // 0 podaje stan kolejki i konczy symulacje kolejki
        nr_klienta = 0;
        do {
            scanf("%d",&kod_operacji);
            if(kod_operacji > 0) {
                f_ans = cbuff_push(++nr_klienta);
                if(f_ans == INFINITY) printf("%d ",-2);
            }
            else {
                if(kod_operacji < 0) {
                    printf("%d ",cbuff_pop());
//                    ans = cbuff_pop();
//                    if(ans < 0) printf("%f ",NAN);
//                    else        printf("%d ",ans);
                }
                else  {
                    printf("\n%d\n",cbuff_state());
                    cbuff_print();
                }
            }
        } while(kod_operacji != 0);
        break;
default:
        if(TEST) printf("NOTHING TO DO!\n");
    }
    return 0;
}

///////////////////////////////////////////////////////

// Definicje funkcji i zmiennych o zasięgu plikowym (file scope):

// Stos

#define STACK_SIZE  10

double stack[STACK_SIZE];
int top=0;   // jawne zerowanie nie jest konieczne

double stack_push(double x){
    if(top>=STACK_SIZE) return INFINITY;
    stack[top] = x;
    top++;
    //print(stack,STACK_SIZE);
    return 0.0;
}

double stack_pop(void) {
    if(top<=0) return NAN;
    top--;
    double tmp = stack[top];
    stack[top]= 0.0;
    //print(stack,STACK_SIZE);
    return tmp;
}

int stack_state(void) {
    return top;
}

//Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE  10

int queue[QUEUE_SIZE];
int in=0, curr_nr=0;  // 1. klient dostanie nr = 1

double queue_push(int how_many) {

    int i=in;
    while(i<QUEUE_SIZE && i<in+how_many){
        curr_nr++;
        queue[i]=curr_nr;
        i++;
    }
    //print(queue,QUEUE_SIZE);
    if(i==in+how_many){
        in+=how_many;
        return 0.0;
    }
    else{
        curr_nr += in+how_many-QUEUE_SIZE;
        in = QUEUE_SIZE;
        return INFINITY;
    }
}

int queue_pop(int how_many) {
    if(how_many>=in){
        for(int i=0; i<in; i++){
            queue[i] =0.0;
        }
        if(in==how_many){
            in=0;
            //print(queue,QUEUE_SIZE);
            return in;
        }
        else{
            in=0;
            //print(queue,QUEUE_SIZE);
            return -1;
        }

    }
    else{
        int tmp = in-how_many;
        for(int i=how_many; i<=how_many+in; i++){
            queue[i-how_many] = queue[i];
            queue[i] = 0.0;
        }
        in-=how_many;
        //print(queue,QUEUE_SIZE);
        return tmp;
    }
}

int queue_state(void) {
    return in;
}

void queue_print(void) {
    for(int i=0; i<in; i++){
        printf("%d ", queue[i]);
    }
}


//Kolejka w buforze cyklicznym

#define CBUFF_SIZE  10

int cbuff[CBUFF_SIZE];
int out=0, len=0;

double cbuff_push(int cli_nr) {
    if(len==CBUFF_SIZE)return INFINITY;
    cbuff[(out+len)%CBUFF_SIZE] = cli_nr;
    len++;
    return 0.0;
}

int cbuff_pop(void) {
    if(len==0)return -1;
    int tmp = cbuff[out];
    cbuff[out] = 0.0;
    out = (out+1)%CBUFF_SIZE;
    len--;
    return tmp;
}

int cbuff_state(void) {
    return len;
}

void cbuff_print(void) {
    for(int i=out; i<out+len; i++){
        printf("%d ", cbuff[i%CBUFF_SIZE]);
    }
}