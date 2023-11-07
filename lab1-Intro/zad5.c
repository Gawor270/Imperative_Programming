#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
Program losuje liczbę 0≤X≤100 Napisz funkcję, która zgaduje wartość X W pętli losujemy n∈[0,100]. Jeżeli X=n
zgadliśmy X, jeżeli nie na podstawie wartości X i n ograniczamy przedział, z którego losujemy kolejne n.
*/


#define min(a,b) ((a)<(b) ? (a) : (b))
#define max(a,b) ((a)>(b) ? (a) : (b))

int randominrange(int a, int b){
   return (a+(rand()%(b-a+1)));
}

int main(void){
    int X,n;
    srand(time(NULL));

    X = randominrange(0,100); 
    printf("%d \n", X);
    
    n = randominrange(0,100);
    printf("%d\n",n);

    while(X!=n){
        n = randominrange(min(X,n),max(X,n)); 
        printf("%d ", n);
    } 
    printf("\n");
    return 0;

}
