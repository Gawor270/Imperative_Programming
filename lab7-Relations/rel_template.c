#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(const pair*, int, int*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair*, int, int*, int);

int find_max_elements(const pair*, int, int*,int*,int);
int find_min_elements(const pair*, int, int*,int*,int);
int get_domain(const pair*, int, int*);

// Case 3:

int composition (const pair*, int, const pair*, int, pair*);

// Comparator for pair
int cmp_pair (const void *a, const void *b) {
}

int insert_int (int *tab, int n, int new_element) {
	int i=0;

	while(i<n && tab[i] < new_element)i++;
	for(int s=n; s>i; s--){
		tab[s] = tab[s-1];
	}
	tab[i] = new_element;
	return 1;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
	//check if new_pair already exists in tab
	for(int i=0; i< n; i++){
		if(tab[i].first == new_pair.first && tab[i].second == new_pair.second) return 0;
	}
	
	//insert new pair so that tab is always sorted by .first in ascending order
	int j=0;
	while(j<n && tab[j].first < new_pair.first){
		j++;
	}

	for(int i=n; i>j; i--){
		tab[i] = tab[i-1];
	}

	tab[j] = new_pair;
	return 1;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int n;
	int top = 0;
	scanf("%d", &n);
	for(int i=0; i<n; i++){
		pair tmp;
		scanf("%d %d", &tmp.first, &tmp.second);
		top += add_relation(relation,top,tmp);
	}

	return top;
}

void print_int_array(const int *array, int n) {
	printf("%d\n", n);
	for(int i=0; i<n; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size,domain,n_domain));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements,domain,n_domain);
			int no_min_elements = find_min_elements(relation, size, min_elements,domain,n_domain);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


int is_reflexive(const pair* tab, int n){
	int i=0;
	while(i < n){
		int val =tab[i].first;
		while(i<n && tab[i].first == val && tab[i].second != val) i++;
		if(i==n || tab[i].first != val) return 0;
		if(tab[i].second ==val && tab[i].first ==val){
			while(tab[i].first==val)i++;
			if(i==n) return 1;
		}
	}
	return 1;
}

int is_irreflexive(const pair* tab, int n){
	//if loop finds element that xRx returns false
	for(int i=0; i<n; i++){
		if(tab[i].first == tab[i].second) return 0;
	}
	return 1;	
}

int is_symmetric(const pair* tab, int n){
	for(int i=0; i<n; i++){
		int j;
		int val = tab[i].second;

		if(val >= tab[i].first)j=i;
		else j=0;

		while(j<n && tab[j].first < val) j++;
		if(j==n) return 0;
		
		while(j<n && tab[j].first == val && tab[j].second != tab[i].first)j++;
		if(j==n || tab[j].first != val) return 0;

	}
	return 1;
}

int is_antisymmetric(const pair* tab, int n){
	for(int i=0; i<n; i++){
		int j;
		int val = tab[i].second;

		if(val >= tab[i].first)j=i;
		else j=0;

		while(j<n && tab[j].first < val) j++;
		if(j==n) return 1;
		
		while(j<n && tab[j].first == val && tab[j].second != tab[i].first)j++;
		if(j==n) return 1;
		if(tab[j].first == val && tab[j].first != tab[j].second) return 0;

	}
	return 1;
}

int is_asymmetric(const pair* tab, int n){
	if(is_antisymmetric(tab,n) && is_irreflexive(tab,n)) return 1;
	return 0;
}

int is_transitive(const pair* tab, int n){
	for(int i=0; i<n; i++){
		int j,k,val2;
		int val1 = tab[i].second;

		if(val1 >= tab[i].first)j=i;
		else j=0;


		while(j<n && tab[j].first < val1) j++;
		if(j !=n ){
			// in this fragment variable j stores first index of pairs with first value val1		
			while(j<n && tab[j].first == val1){
				val2 = tab[j].second;

				k=0;

				while(k<n && tab[k].first < tab[i].first)k++;
				if(k==n)return 0;

				while(k<n && tab[k].first == tab[i].first && tab[k].second != val2)k++;
				if(k==n || tab[k].first != tab[i].first)return 0;
				
				j++;
			}
		}


	}
	return 1;
}

int is_partial_order(const pair* tab, int n){
	if(is_reflexive(tab,n) && is_transitive(tab,n) && is_antisymmetric(tab,n)) return 1;
	return 0;
}

int get_domain(const pair* tab, int n, int* X){
	int top =0;
	for(int i=0; i<n; i++){
		int flag1 =1;
		int flag2 =1;
		for(int j=0; j<top; j++){
			if(X[j] == tab[i].first)flag1 =0;

			if(X[j] == tab[i].second)flag2 =0;
		}

		if(tab[i].first == tab[i].second && flag1){
			top += insert_int(X,top,tab[i].first);
		}

		else if(tab[i].first != tab[i].second && flag1 && flag2){
			top += insert_int(X,top,tab[i].first);
			top += insert_int(X,top,tab[i].second);
		}

		else if(tab[i].first != tab[i].second && flag1){
			top += insert_int(X,top,tab[i].first);
		}

		else if(tab[i].first != tab[i].second && flag2){
			top += insert_int(X,top,tab[i].second);
		}
	}

	return top;
}

int is_connected(const pair* tab, int n, int* X, int ndom){
	for(int i=0; i<ndom; i++){
		for(int j=0; j<ndom; j++){
			int flag = 1;
			for(int k=0; k<n; k++){
				if((tab[i].first == X[i] && tab[i].second  == X[j]) || (tab[i].second == X[i] && tab[i].first  == X[j])){
					flag =0;
					break;
				}
			}
			if(flag)return 0;
		}
	}
	return 1;
}

int is_total_order(const pair* tab, int n, int* X,int ndom){
	if(is_partial_order(tab,n) && is_connected(tab,n,X,ndom))return 1;
	return 0;
}

int find_max_elements(const pair* tab, int n, int*max_el,int*X,int ndom){
	int top =0;
	for(int i=0; i<ndom; i++){
		int flag =1;
		for(int j=0; j<n; j++){
			if(tab[j].first == X[i] && tab[j].second != X[i]){
				flag =0;
				break;
			}
		}
		if(flag){
			max_el[top] = X[i];
			top++;
		}
	}
	return top;
}

int find_min_elements(const pair* tab, int n, int*min_el,int*X,int ndom){
	int top =0;
	for(int i=0; i<ndom; i++){
		int flag =1;
		for(int j=0; j<n; j++){
			if(tab[j].second == X[i] && tab[j].first != X[i]){
				flag =0;
				break;
			}
		}
		if(flag){
			min_el[top] = X[i];
			top++;
		}
	}
	return top;
}

int composition (const pair* relation1, int n1, const pair* relation2, int n2, pair* result){
	int top=0;
	int first,second;
	for(int i=0; i<n1; i++){
		first = relation1[i].first;
		for(int j=0; j<n2; j++){
			if(relation2[j].first == relation1[i].second){
				second = relation2[j].second;
				pair new_pair;
				new_pair.first = first;
				new_pair.second = second;
				top += add_relation(result,top,new_pair);
			}
		}
	}
	return top;
}