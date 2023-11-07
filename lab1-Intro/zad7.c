#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

/*
najdź pole powierzchni ograniczone osią Ox i wykresem funkcji sin(x) w przedziale [a,b] metodą Monte Carlo. Dane
wejściowe: a,b,N (liczba losowanych punktów).
*/

float monte(float a, float b, int N){
        float area = b-a;
        printf("%f \n",area);
        int counter =0;
        for(int i=0; i<N; i++){
            float x = a + area*((rand()%101)/100);
            float y = (rand()%101)/100;
            printf("%f %f \n",x,y);
            if(y<sin(x))counter++;            
        }
        return (counter/N)*area;     
}

int main(void){
    srand(time(NULL));
    printf("%f",monte(4.0,4.8,100));
    return 0;
}
