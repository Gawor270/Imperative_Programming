#include <stdio.h>
#include <math.h>

/*
Dana jest liczba całkowita dodatnia n Napisz program, który znajduje wszystkie liczby pierwsze mniejsze od n, których cyfry
tworzą ciąg niemalejący.
*/

typedef enum {false, true} bool;

bool nondecseq(int num){
    int cpy = num;
    while(cpy){
        if((cpy%10)<((cpy/10)%10))return false;
        cpy/=10;
    }
    return true;
}

bool is_prime(int num){
    if(num==2)return true;
    if(num==3)return true;
    if(num%2==0 || num%3==0)return false;
    int i=5;
    while(i*i<=num){
        if(num%i==0)return false;
        i+=2;
        if(num%i==0)return false;
        i+=4;
    }
    return true;
}

int main(void){
    int n;
    scanf("%d", &n);
    for(int i=2; i<n; i++){
        if(is_prime(i) && nondecseq(i))printf("%d ", i);
    }
    printf("\n");    
    return 0;
}
