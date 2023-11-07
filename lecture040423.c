#include <stdio.h>
#include <stdlib.h>
//ROZMIARY PAMIECI PODAJEMY W BAJTACH
// kompilatory c oczekując stalego rozmiaru tablicy
// nie stosowac sizeof(tab)/sizeof(tab[0]) w funkcjach bo tam tab traktowany jako pointer 8b
void testing(int* tab){

    for(int i=0; i<2; i++){
        for(int j=0; j<12; j++){
            printf("%d ", *(tab +i*12 +j));
        }
        printf("\n");
    }
}

//structy pozwalaja grupowac zmienne
struct osoba{
    char imie[32];
    char nazwisko[32];
    int wiek;
};

struct complex{
    double re,im;
}  solution = {0.0,0.0};

enum Color {red, green, blue};

// jezeli chce utworzyc zmienna swojego structa to musze napisac struct nazwa_structa nazwa_zmiennej 
// typedef przy deklaracji structa pozwala nie pisac przed deklaracja zmiennej struct
struct circle{
    double xcenter, ycenter, radius;
    enum Color color;
} aCircle = {0.0, 0.0, 10.0, red};

//pola bitowe CUSTOMOWE WIELKOSCI BITOWE ZMIENNYCH !!! wielkosci nie moga przekraczac typu podstawowego
struct{
    unsigned short icon : 8;
    unsigned short color : 4;
    unsigned short underline : 1;
    unsigned short blink : 1;
} screen[25][80];


// UNION zmienne zajmuja to samo miejsce w pamieci
union {
    char a;
    short b;
    int c;
    double d;
} x;

struct Line{
    double x1,y1,x2,y2;
};

struct Circle{
    double centerx, centery;
    double radius;
};

struct Arc{
    double centerx, centery;
    double radius;
    double startAngle, endAngle;
};

// jak stworzyc tablice figur?
enum EntityType {eline,ecircle,earc};
struct Entity{
    // selektor typu unii
    enum EntityType type;

    // wspólne pola
    int color;
    int layer;
    int linetype;

    //elementy składowe
    union{
        struct Line line;
        struct Circle circle;
        struct Arc arc;
    };
};

struct Entity tab[1000];
int main(void){
    int* tab;
    tab = malloc(sizeof(int)*100);
    int days_in_month[2*12] = {31,28,31,30,31,30,31,31,30,31,30,31, 31,29,31,30,31,30,31,31,30,31,30,31};  
    testing(days_in_month);
    return 0;
}