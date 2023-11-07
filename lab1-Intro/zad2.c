#include <stdio.h>
/*
Napisz program, który dla zadanej liczby naturalnej n odpowiada na pytanie, czy liczba ta jest iloczynem dowolnych dwóch
kolejnych wyrazów ciągu Fibonacciego. Zakładamy, że pierwsze dwa wyrazy ciągu Fibonacciego to 0 i 1.
*/

typedef enum { false, true} bool;

bool is_fibb_prod(int x){
    int a=0;
    int b=1;
    int c = a+b;
    while(a*b != x && a*b <= x ){        
        a=b;
        b=c;
        c=a+b;
    }
    if(a*b==x)return true;
    return false;
}


int main(void){
    int var;
    scanf("%d", &var);
    printf("%d \n", is_fibb_prod(var));
    return 0;

}
