#include <stdio.h>
#include <stdlib.h>




void swap(int* x, int* y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}




int main(void){
    int val1 = 5;
    int val2 = 8;
    swap(&val1, &val2);
    printf("%d %d",val1,val2);


    return 0;
}