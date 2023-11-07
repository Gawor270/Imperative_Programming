#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
	if(((Date*)d1)->year == ((Date*)d2)->year){

		if(((Date*)d1)->month == ((Date*)d2)->month){

			if(((Date*)d1)->day == ((Date*)d2)->day){
				return 0;
			}
			return ((Date*)d1)->day - ((Date*)d2)->day;
		}
		return ((Date*)d1)->month - ((Date*)d2)->month;
	}
	return ((Date*)d1)->year - ((Date*)d2)->year;
}

// compare foods
int cmp(const void *a, const void *b) {
	int res1 = strcmp(((Food*)a)->name,((Food*)b)->name);
	if(!res1){

		if(((Food*)a)->price == ((Food*)b)->price){
			int res2 = cmp_date(&(((Food*)a)->valid_date),&(((Food*)b)->valid_date));
			if(!res2){
				return 0;
			}
			return res2;
		}
		float dif = ((Food*)a)->price - ((Food*)b)->price;
		if(dif <0) return -1;
		else return 1;
	}
	return res1;
}
// bsearch returning address where to insert new element
void *bsearch2 (const void *key, void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
	void *adr = base;
	int b = 0;
	int e = nmemb-1;
	while(b<=e){
		int mid = b + (e-b)/2;
		adr =(char*)base + size*mid; 
		if(compar(adr,key) == 0){
			*result = 1;
			return adr;
		}
		else if(compar(adr,key) > 0){
			e = mid -1;
		}
		else{
			b = mid +1;
		}
	}
	*result = 0;
	return (char*)base + size*b;
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
	Food* point = p;
	for(int i=0; i<n; i++){
		if(!strcmp(point->name,art)){
			if(point->valid_date.day >9 && point->valid_date.month >9)
				printf("%.2f %d %d.%d.%d\n", point->price,point->amount,point->valid_date.day,point->valid_date.month,point->valid_date.year);

			else if(point->valid_date.day >9 && point->valid_date.month <9)
				printf("%.2f %d %d.0%d.%d\n", point->price,point->amount,point->valid_date.day,point->valid_date.month,point->valid_date.year);

			else if(point->valid_date.day <9 && point->valid_date.month >9)
				printf("%.2f %d 0%d.%d.%d\n", point->price,point->amount,point->valid_date.day,point->valid_date.month,point->valid_date.year);

			else
				printf("%.2f %d 0%d.0%d.%d\n", point->price,point->amount,point->valid_date.day,point->valid_date.month,point->valid_date.year);
		}
		point++;
	}
}

// add record to table if absent
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
	char result =0;
	Food* adr;
	adr = bsearch2(new,tab,*np,sizeof(Food),compar,&result);
	if(result){
		adr->amount = adr->amount + new->amount;
	}
	else{
		Food* tmpadr = adr+1;
		Food tmp = *adr;
		*adr = *new;
		*np = *np +1;
		while(tmpadr < tab + *np){
			Food tmp2 = *tmpadr;
			*tmpadr = tmp;
			tmpadr += 1;
			tmp = tmp2;
		}
	}
	return adr;
}

void add_elem(Food* tab,int *top,Food *elem){
	tab[*top] = *elem;
	*top = *top +1;
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
	Food elem;
	int n=0;
	for(int i=0; i<no; i++){
		// char buff[RECORD_MAX];
		// fgets(buff,RECORD_MAX,stream);
		// readline(buff,&elem);
		fscanf(stream,"%s %f %d %d.%d.%d",elem.name,&elem.price,&elem.amount,&elem.valid_date.day,&elem.valid_date.month,&elem.valid_date.year);

		if(sorted) add_record(tab,&n,cmp,&elem);
		else add_elem(tab,&n,&elem);
	}
	return n;
}

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}


// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
	qsort(food_tab,n,sizeof(Food),cmp_qs);

	struct tm t = {.tm_year = curr_date.year-1900, .tm_mon = curr_date.month-1, .tm_mday = curr_date.day};
	t.tm_mday += days;
	mktime(&t);

	curr_date.year = t.tm_year+1900;
	curr_date.month = t.tm_mon+1;
	curr_date.day = t.tm_mday;

	Food* ptr = bsearch(&curr_date,food_tab,n,sizeof(Food),cmp_bs);
	Food* curr = ptr;
	float sum =0;
	while(curr < food_tab +n && (*curr).valid_date.year == curr_date.year && (*curr).valid_date.month == curr_date.month && (*curr).valid_date.day == curr_date.day){
		sum += ((*curr).amount)*((*curr).price);
		curr ++;
	}
	curr = ptr-1;
	while(curr >= food_tab && (*curr).valid_date.year == curr_date.year && (*curr).valid_date.month == curr_date.month && (*curr).valid_date.day == curr_date.day){
		sum += ((*curr).amount)*((*curr).price);
		curr --;
	}

	return sum;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
	int no_children;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
	if(((Person*)a)->parent == NULL) return -1;
	if(((Person*)b)->parent == NULL) return 1;
	int res1 = strcmp(((Person*)a)->parent,((Person*)b)->parent);
	if(! res1){
		int res2 = cmp_primo((Person*)a,(Person*)b);
		if(!res2){
			return cmp_date(&((Person*)a)->born,&((Person*)b)->born);
		}
		return res2;
	}
	return res1;
}

int cmp_parent(const void*a, const void*b){
	if(((Parent*)a)->par_name == NULL) return -1;
	if(((Parent*)b)->par_name == NULL) return 1;	
	return strcmp(((Parent*)a)->par_name,((Parent*)b)->par_name);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab,int size) {
	idx_tab[0].index = 0;
	idx_tab[0].par_name = NULL;
	idx_tab[0].no_children =1;

	idx_tab[1].index = 1;
	idx_tab[1].par_name = pers_tab[1].parent;
	idx_tab[1].no_children = 1;
	int j=1;
	int countelem =2;
	for(int i=2; i<size; i++){
		if(strcmp(pers_tab[i-1].parent,pers_tab[i].parent)){
			j++;
			idx_tab[j].index = i;
			idx_tab[j].par_name = pers_tab[i].parent;
			idx_tab[j].no_children = 1;
			countelem++;
		}
		else{
			idx_tab[j].no_children  = idx_tab[j].no_children +1;
		}
	}

	return countelem;
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
	int len,beg,end,i;
	Person* Personbuff = malloc(size*sizeof(Person));
	Parent* p;

	for(i=0 ;i<size; i++){
		Parent searcheditem;
		searcheditem.par_name = person_tab[i].name;

		p = bsearch(&searcheditem,idx_tab,no_parents,sizeof(Parent),cmp_parent);
		if(p == NULL) continue;

		beg = p->index;
		end = beg + p->no_children;

		len = p->no_children;
		memcpy(Personbuff,person_tab+beg,len*sizeof(Person));
		memmove(person_tab+i+1+len,person_tab+i+1,(beg-(i+1))*sizeof(Person));
		memcpy(person_tab+i+1,Personbuff,len*sizeof(Person));
		
		for(int j=0; j<no_parents; j++){
			if((idx_tab+j)->index >=i+1 && (idx_tab+j)->index <end) (idx_tab+j)->index = (idx_tab+j)->index +len;
		}
		p->index = i+1;
	}

	free(Personbuff);
}

int cleaning(Person *person_tab, int n) {
	int deleted=0;
	int count = n;
	int top  = n;
	int j=0;
	while(count){
		while(count && !(person_tab+j)->bits.in_line){
			memmove(person_tab+j,person_tab+j+1,(top-j-1)*sizeof(Person));
			top --;
			count--;
			deleted++;
		}
		count--;
		j++;
	}
	return deleted;
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
	int del;
	qsort(person_tab,n,sizeof(Person),cmp_person);
	Parent* indices_tab = malloc(n*sizeof(Parent));

	int indtabsize =fill_indices_tab(indices_tab, person_tab, n);

	persons_shifts(person_tab,n,indices_tab,indtabsize);
	del = cleaning(person_tab,n);
	return n-del;

}

int main(void) {
	
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};

	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}