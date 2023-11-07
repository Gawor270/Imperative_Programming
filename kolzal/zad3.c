#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFF_SIZE 1024
char buff[BUFF_SIZE];

enum type{car,bus,truck};
enum drive{electric,hybrid,combustion};

struct carparam{
    int carseats;
    float power;
};

struct busparam{
    int busseats;
    int stand;
};

struct truckparam{
    float capacity;
    float pressure;
};

typedef struct Vehicle{
    char owner[BUFF_SIZE];
    int date[3];
    enum type t;
    enum drive d;

    union{
        struct carparam car;
        struct busparam bus;
        struct truckparam truck;
    };

}Vehicle;

void new_owner(Vehicle* pvehicle){
    fgets(buff,BUFF_SIZE,stdin);
    strcpy(pvehicle->owner,buff);

}

int comp_dates(int* date1, int* date2){
    if(date1[2] == date2[2]){
        if(date1[1] == date2[1]){
            if(date1[0] == date2[0]){
                return 0;
            }
            return date1[0] - date2[0];
        }
        return date1[1] - date2[1];
    }
    return date1[2]-date2[2];
}

int delayed(Vehicle* pvehicle,int size, int* base_date,  char*** pdelayed_owners){
    int counter =0;
    char** delayed_owners = (char**)malloc(sizeof(char*));

    for(int i=0; i<size; i++){
        if(comp_dates(pvehicle[i].date,base_date) <0 ){
            delayed_owners = (char**)realloc(delayed_owners,(counter+1)*sizeof(char*));
            delayed_owners[counter] = pvehicle[i].owner;
            counter++;
        }
    }
    if(counter){
        *pdelayed_owners = delayed_owners;
        return counter;
    }
    else{
        free(delayed_owners);
        pdelayed_owners = NULL;
        return 0;
    }
}

void print_pojazd(Vehicle* pvehicle){
    printf("Owner: %s",pvehicle->owner);
    printf("Overview date: %d.%d.%d\n", pvehicle->date[0],pvehicle->date[1],pvehicle->date[2]);
    printf("Type: %d\n", pvehicle->t);
    printf("Drive: %d\n",pvehicle->d);
    switch(pvehicle->t){
        case car:
            printf("Seats: %d\n",pvehicle->car.carseats);
            printf("Power: %.2f\n",pvehicle->car.power);
            break;
        
        case bus:
            printf("Seats: %d\n",pvehicle->bus.busseats);
            printf("Standing places: %d\n", pvehicle->bus.stand);
            break;
        
        case truck:
            printf("Engine capacity: %.2f\n",pvehicle->truck.capacity);
            printf("Max axis pressure: %.2f\n",pvehicle->truck.pressure);
            break;
        
        default:
            break;
    }
}
int main(void){
    Vehicle* v = (Vehicle*)malloc(3*sizeof(Vehicle));
    new_owner(&v[0]);
    v[0].date[0]  = 3;
    v[0].date[1] = 4;
    v[0].date[2] = 2020;
    v[0].t = car;
    v[0].d = hybrid;
    v[0].car.power = 3;
    v[0].car.carseats = 23;
    new_owner(&v[1]);
    v[1].date[0]  = 12;
    v[1].date[1] = 9;
    v[1].date[2] = 2015;
    v[1].t = bus;
    v[1].d = electric;
    v[1].bus.busseats = 102;
    v[1].bus.stand = 112;
    new_owner(&v[2]);
    v[2].date[0]  = 30;
    v[2].date[1] = 1;
    v[2].date[2] = 2019;
    v[2].t = truck;
    v[2].d = combustion;
    v[2].truck.capacity = 34;
    v[2].truck.pressure = 223;
    print_pojazd(&v[0]);
    print_pojazd(&v[1]);
    print_pojazd(&v[2]);
    printf("\n");

    char** delay ;
    int del;
    int basedate[3] = {3,10,2019};
    del = delayed(v,3,basedate,&delay);
    for(int i=0; i<del; i++){
        printf("%s",delay[i]);
    }


    return 0;
}