/*
Napisz program, który wczytuje liczbę całkowitą N a następnie oblicza i drukuje na ekran wartość N!
. Uwaga: silnia bardzo szybko powoduje przepełnienie całkowitoliczbowe. Sprawdź dla jakich wartości N
program wyprowadza prawidłowy wynik. Przetestuj działanie programu dla różnych typów danych (dla ułatwienia można zastosować definicję typedef).
*/

#include <stdio.h>


unsigned long long int factorial(int var){
	unsigned long long int result = 1;
	while(var){
		result *= var;
		var--;
	}
	return result;

}


int main(void){
	int var;
	scanf("%d",&var);
	printf("%llu \n", factorial(var));
	return 0;
}
