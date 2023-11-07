/*
Liczba doskonała jest to taka liczba naturalna, która jest sumą wszystkich swych dzielników właściwych (to znaczy od niej
mniejszych). Najmniejszą liczbą doskonałą jest 6, ponieważ jej dzielnikami właściwymi są 1, 2, 3 i 1 + 2 + 3 = 6. Napisz program,
który znajduje wszystkie liczby doskonałe w zadanym przedziale oraz ich liczbę
*/

#include <stdio.h>
#include <math.h>

typedef enum {false, true} bool;

bool is_perfect(int val){
    int sum=0;
    int maxim = val/2 +1;
    for(int i=1; i<maxim; i++){
        if(val%i==0)sum+=i;
    } 
    return (sum==val);
}


int main(void){
    int a,b;

    printf("enter beginning of range: ");
    scanf("%d", &a);
    printf("enter end of range: ");
    scanf("%d",&b);

    int counter=0;
    for(int i=a; i<=b; i++){ 
        if(is_perfect(i)){
            printf("%d \n", i);
            counter++;
        }
    }
    printf("%d \n", counter);
    return 0;
}

