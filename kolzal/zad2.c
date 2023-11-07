#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct{
    double* results;
    int len;
}Data;


typedef struct QueueElement{
    Data data;
    struct QueueElement* next;
}QueueElement;

typedef struct tagQueue{
    QueueElement* head;
    QueueElement* tail;
}Queue;

void init_queue(Queue* pqueue){
    QueueElement* newel = (QueueElement*)malloc(sizeof(QueueElement));
    newel->next = NULL;
    pqueue->head = newel;
    pqueue->tail = newel;
}

void free_queue(Queue*pqueue){
    QueueElement* curr = pqueue->head->next;
    QueueElement* todel;
    while(curr){
        todel = curr;
        curr = curr->next;
        free(todel);
    }
    pqueue->head->next = NULL;
}

void push_copy1(Queue* pqueue, const Data* pdata){
    QueueElement* newel = (QueueElement*)malloc(sizeof(QueueElement));
    Data* newdata = (Data*)malloc(sizeof(Data));
    newdata->len = pdata->len;
    newdata->results = (double*)malloc(newdata->len*sizeof(double));
    memcpy(newdata->results,pdata->results,newdata->len*sizeof(double));
    newel->data = *newdata;
    newel->next = NULL;

    pqueue->tail->next = newel;
    pqueue->tail = newel;
}

void push_copy(Queue* pqueue, const Data* pdata){
    QueueElement* newel = (QueueElement*)malloc(sizeof(QueueElement));
    newel->data = *pdata;
    newel->next = NULL;
    pqueue->tail->next = newel;
    pqueue->tail = pqueue->tail->next;
}


int peek(const Queue* pqueue, Data* pdata){
    if(pqueue->head->next == NULL) return 0;
    pdata->len = pqueue->head->next->data.len;
    pdata->results = (double*)malloc(pdata->len*sizeof(double));
    memcpy(pdata->results,pqueue->head->next->data.results,pdata->len*sizeof(double));
    return 1;
}

int pop(Queue* pqueue, Data* pdata){
    if(pqueue->head->next == NULL) return -1;
    QueueElement* todel;
    todel = pqueue->head->next;
    pqueue->head->next = pqueue->head->next->next;
    pdata = &todel->data;
    free(todel);
    if(pqueue->head->next == NULL) return 0;
    return 1;
}

int get_total_count(const Queue* pqueue){

    int counter =0;
    QueueElement* curr = pqueue->head->next;
    while(curr){
        counter += curr->data.len;
        curr = curr->next;
    }
    return counter;
}

void printdata(Data data){
    for(int i=0; i<data.len; i++){
        printf("%.2f ", *(data.results + i));
    }
}

void dumpqueue(Queue* pqueue){
    QueueElement* curr = pqueue->head->next;
    while(curr){
        printdata(curr->data);
        printf("\n");
        curr = curr->next;
    }
}

int main(void){
    Data d1;
    double tab1[]=  {1.3,523.3,5324.532,125.432};
    int len1 = 4;
    d1.results = tab1;
    d1.len = len1;
    
    Data d2;
    double tab2[] = {1234.2,532.4,1.0};
    int len2 = 3;
    d2.results = tab2;
    d2.len = len2;

    Data d3;
    double tab3[] = {1324.32,5324.1};
    int len3 = 2;
    d3.results = tab3;
    d3.len = len3;

    Queue Q;
    init_queue(&Q);

    push_copy(&Q,&d1);
    push_copy(&Q,&d2);
    push_copy(&Q,&d3);
    printf("%d\n",get_total_count(&Q));
    dumpqueue(&Q);
    pop(&Q,&d1);
    printf("\n");
    dumpqueue(&Q);
    printf("\n");
    // printdata(d1);
    free_queue(&Q);
    dumpqueue(&Q);

    return 0;
}