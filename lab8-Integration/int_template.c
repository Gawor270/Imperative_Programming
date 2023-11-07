#include <stdio.h>
#include <math.h>
#include <errno.h>
#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double) ;

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
	return (x*(x*(x*x*(2*x - 4) + 3.5) + 1.35) - 6.25);
}

double f_rat(double x) {
	return (1/((x-0.5)*(x-0.5) + 0.01));
}

double f_exp(double x) {
	return (2*x*exp(-1.5*x) -1);
}

double f_trig(double x) {
	return (x*tan(x));
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
	double h  = (b-a)/n;
	double sum =0;

	double x = a;
	for(int i=0; i<n; i++){
		sum += f1(x);
		x += h;
	}

	return h*sum;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
	double h  = (b-a)/n;
	double sum =0;

	for(int i=1; i<=n; i++){
		sum += f1(a + i*h);
	}

	return h*sum;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
	double h  = (b-a)/n;
	double x = a + h/2;
	double sum =0;

	for(int i=0; i<n; i++){
		sum += f1(x);
		x += h;
	}

	return h*sum;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
	double h  = (b-a)/n;
	double sum = func(a) + func(b);

	for(int i=1; i<n; i++){
		sum += 2*func(a + i*h); 
	}

	return h/2*sum;
}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
	int i;
	double h  = (b-a)/n;
	double sum = f(a) + f(b);
	double start = a + h/2;

	for(i=0; i<n; i++){
		sum += 4*f(start + i*h);
	}

	for(i=1; i<n; i++){
		sum += 2*f(a + i*h);
	}

	return h/6*sum;
}

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp,double,double,int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
	return quad_tab[quad_no](func_tab[fun_no],a,b,n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {

	double m = (a+b)/2;
	double lm = (a+m)/2;
	if((delta/2 == delta) || (a == lm)){errno = EDOM; return S;}

	double left = quad(f,a,m,1);
	double right = quad(f,m,b,1);

	double eps = left+right -S;
	if(level >= RECURS_LEVEL_MAX && errno != EDOM) return NAN;

	if(level >= RECURS_LEVEL_MAX || fabs(eps) <= delta) {
		return left+right;
	}
	return recurs(f,a,m,left,delta/2,quad,level+1) + recurs(f,m,b,right,delta/2,quad,level+1);
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
	double S = quad(f,a,b,1);
	return recurs(f,a,b,S,delta,quad,0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double,double);

double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
	double x,y;
	double hx = (x2-x1)/nx;
	double h2 = (y2-y1)/ny;

	double sum = 0;
	x = x1;
	for(int i=0; i<nx; i++){
		y = y1;
		for(int j=0; j<ny; j++){
			sum += f(x,y);
			y += h2;
		}
		x += hx;
	}
	
	return h2*hx*sum;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
						   Func1vFp fg, Func1vFp fh) {
	hy *= 0.01;
	double  x,y;
	double hx = (x2-x1)/nx;
	int ny;
	double sum = 0;
	x = x1 + hx/2;
	for(int i=0; i<nx; i++){
		ny = ceil((fh(x)-fg(x))/hy);

		y = fg(x) + hy/2;
		for(int j=0; j<ny; j++){
			sum = sum + f(x,y);
			y = y + hy;
		}
		x = x + hx;
	}
	
	return hx*hy*sum;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
		int ny, Func1vFp fg, Func1vFp fh)  {
	double  x,y;
	double hx = (x2-x1)/nx;
	double hy = (y2-y1)/(ny);
	double sum = 0;
	x = x1 ;
	for(int i=0; i<nx; ++i){
		if(fg(x) < fh(x)){
			double lowerbound = fmax(y1,fg(x));
			double upperbound = fmin(y2,fh(x));
			int currny = ceil((upperbound-lowerbound)/hy);
			double currhy = ((upperbound-lowerbound)/currny);
			y =lowerbound;
			for(int j=0 ; j< currny; ++j){
				sum += currhy*f(x,y);
				y += currhy;
			}
		}
		x = x + hx;
	}
	
	return hx*sum;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2],  int tn[], BoundNvFp boundary) {
	double sum =0;
	double coord[3] = {variable_lim[0][0],variable_lim[1][0], variable_lim[2][0]};
	double hx = (variable_lim[0][1]-variable_lim[0][0])/tn[0];
	double hy = (variable_lim[1][1]-variable_lim[1][0])/tn[1];
	double hz = (variable_lim[2][1]-variable_lim[2][0])/tn[2];
	coord[0] += hx;

	for(int i=0; i<tn[0]; ++i){
		coord[1] = variable_lim[1][0] + hy;
		for(int j=0; j<tn[1]; ++j){
			coord[2] = variable_lim[2][0] + hz;
			for(int k=0; k< tn[2]; ++k){
				if(boundary != NULL && boundary(coord,3)){
					sum += f(coord,3);
				}
				else if(boundary == NULL) sum += f(coord,3);

				coord[2] += hz;
			}
			coord[1] += hy;
		}
		coord[0] += hx;
	}
	
	return hx*hy*hz*sum;
}

// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],
         double variable_lim[][2], const int tn[], int level, BoundNvFp boundary,double product) {

	if(level == variable_no -1){
		double h = (variable_lim[level][1] - variable_lim[level][0])/tn[level];
		t_variable[level] = variable_lim[level][0] + h/2;
		for(int j=0; j<tn[level]; ++j){
			if(boundary == NULL  || boundary(t_variable,variable_no)){
				*p_sum += product*f(t_variable,variable_no);
			}
			t_variable[level] += h;
		}
	}
	else{
		double ht = (variable_lim[level][1] - variable_lim[level][0])/tn[level];
		t_variable[level] = variable_lim[level][0] +ht/2;
		for(int i=0; i<tn[level]; ++i){
			recur_quad_rect_mid(p_sum,f,variable_no,t_variable,variable_lim,tn,level+1,boundary,product);
			t_variable[level] += ht;
		}
	}
}

int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.;
			double product =1;
			for(int i=0; i<n; ++i){
				product *= (variable_lim[i][1]-variable_lim[i][0])/tn[i]; 
			}

			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL,product);
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}

