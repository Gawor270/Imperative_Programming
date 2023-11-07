#include <stdio.h>
/*
Szyfr Cezara polega na szyfrowaniu kolejnych liter (pozostałe znaki pozostawiamy bez zmian). Każda litera zostaje zamieniona
w k-tą następną w alfabecie (k jest stałą szyfru), przy czym jeżeli taka nie istnieje (wychodzimy za 'z'), to odliczanie jest
kontynuowane z powrotem od 'a'. Szyfrowanie zachowuje wielkość liter. Napisz funkcję, która szyfruje ciąg znaków podany jako
argument.
*/


void ceasar(char* str, int k){
   for(int i=0; i<sizeof(str); i++){
        if(str[i]>=65 && str[i] <=90){
            str[i] = 65 + (k+(str[i]-65+1)-1)%26;
        }
        else if(str[i]>=97 && str[i]<=122){
            str[i] =  97 + (k+(str[i]-97+1)-1)%26;
        }
    }
}


int main(void){
    char str[20];
    int k;
    fgets(str,20,stdin);
    scanf("%d", &k);
    ceasar(str,8);
    printf("%s \n",str);
    return 0;
}
