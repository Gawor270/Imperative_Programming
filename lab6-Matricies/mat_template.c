#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 40


void swap(int* xp, int* yp){
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void fswap(double* xp, double* yp){
	double temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		// if((floorf(x[i]*10000)/10000) != 0)
		printf("%.4f ", x[i]);
		// else printf("-0.0000 ");
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			if((floorf(A[i][j]*10000)/10000) != 0)printf("%.4f ", A[i][j]);
			else printf("-0.0000 ");
		}
		printf("\n");
	}
}
 
void print_matind(double A[][SIZE], int m, int n, int* indices) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[indices[i]][j]);
		}
		printf("\n");
	}
}
// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	// emptying array AB
	// for(int i=0; i<m; i++){
	// 	for(int j=0; j<n; j++){
	// 		AB[i][j] = 0;
	// 	}
	// }

	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			for(int k=0; k<p; k++){
				AB[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	// print_mat(A,n,n);
	double determinant=1;
	double ratio;
	for(int i=0; i<n; i++){
		determinant *= A[i][i];
		for(int j=i+1; j<n; j++){
			ratio = A[j][i]/A[i][i];
			for(int k=0; k<n; k++){
				A[j][k] -= ratio*A[i][k];
			}
		}
	}
	// print_mat(A,n,n);
	
	return determinant;
}
//Performs Back Substitution to find values of Unknowns
void ApplyBackSubstitution(double A[][SIZE], const int indices[], double x[], int n)
{   
    int i,j;
    double sum;
	double* temp;
	temp = (double*) malloc(sizeof(double)*n);
    temp[indices[n-1]]=x[indices[n-1]]/A[indices[n-1]][n-1]; 

    for(i=n-2;i>=0;i--)
    {
	
        sum=0.0;
        for(j=i+1;j<n;j++)
            sum=sum+A[indices[i]][j]*temp[indices[j]];  
        temp[indices[i]] = (x[indices[i]]-sum)/A[indices[i]][i];
    } 
	for(int i=0; i<n; i++){
		x[i] = temp[indices[i]];
	}
	free(temp);
}

double convertToUpperTriangular(double A[][SIZE], const double b[], double x[], const int n, const double eps)
{
    int i,j,k,maxind;
    double determinant=1.0;
	double max,ratio;
	int* indices;
	indices = (int*) malloc(sizeof(int)*n);

	for(i=0; i<n; i++){
		x[i] = b[i];
		indices[i] = i;
	}


    for( i=0;i<n;i++)
    {
		max= fabs(A[indices[i]][i]);
		maxind = i; 
		for(j=i+1; j<n; j++){
			if(fabs(A[indices[j]][i]) > max){
				max = fabs(A[indices[j]][i]);
				maxind = j;
			}
		}
		// printf("%lf \n", max);
		// for(int i=0; i<n; i++){
		// 	printf("indices: %d ", indices[i]);
		// }
		// printf("\n");
		if(i != maxind){
			swap(&indices[i], &indices[maxind]);
			if(determinant!=0)determinant *= -1;
		}
		if(fabs(A[indices[i]][i]) < eps){
			// printf("%lf %lf \n",fabs(A[indices[i]][i]),eps );
			determinant = 0;
			
		}
		if(determinant!= 0)determinant *= A[indices[i]][i];
	
    	for(j=0;j<n;j++)
      	{
      	   if(j>i)
      	   {
      	       	ratio=A[indices[j]][i]/A[indices[i]][i];
      	       	for(k=0;k<n+1;k++){
      	        	A[indices[j]][k]=A[indices[j]][k] -( ratio * A[indices[i]][k]);
				}
				x[indices[j]] = x[indices[j]] - (ratio*x[indices[i]]);

      	    //    print the Matrix after changed row (Intermediate forms)
      	    //    printf("Intermediate forms:\n");
      	    //    for(z=0; z<n; z++)
      	    //    {
      	    //      for(y=0;y<n;y++)
      	    //         printf("%.4f ", A[indices[z]][y]);
      	    //      printf("\n");   
      	    //    }
      	    //    printf("\n");
	
      	   } 
      	}
    }
	if(determinant != 0 && b != NULL && x != NULL){
		ApplyBackSubstitution(A,indices,x,n);
	}

	free(indices);
	return determinant;

}



void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
	double* temp;
	for(int i=n-1; i>=0; i--){
		double sum=x[indices[i]];
		for(int j=i+1; j<n; j++){
			sum -= A[indices[i]][j]*x[indices[j]];
		}
		x[indices[i]] = sum/A[indices[i]][i];
	}
	temp = (double*) malloc(sizeof(double)*n);
	for(int i=0; i<n; i++){
		temp[i] = x[i];
	}
	for(int i=0; i<n; i++){
		x[i] = temp[indices[i]];
	}
	free(temp);

}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	int i,j,k,maxind;
	int* indices;
	indices = (int*) malloc(sizeof(int)*n);
	double determinant=1.0;
	double max,ratio;

	for(i=0; i<n; i++){
		x[i] = b[i];
		indices[i] = i;
	}

	for(i=0; i<n; i++){
		max= fabs(A[indices[i]][i]);
		maxind = i; 
		for(j=i+1; j<n; j++){
			if(fabs(A[indices[j]][i]) > max){
				max = fabs(A[indices[j]][i]);
				maxind = j;
			}
		}
		// printf("%lf \n", max);
		// for(int i=0; i<n; i++){
		// 	printf("indices: %d ", indices[i]);
		// }
		// printf("\n");
		if(i != maxind){
			swap(&indices[i], &indices[maxind]);
			determinant *= -1;
		}

		if(fabs(A[indices[i]][i]) < eps){
			// printf("%lf %lf \n",fabs(A[indices[i]][i]),eps );
			determinant = 0;
		}
			
		determinant *= A[indices[i]][i];
		
		for(j=i+1; j<n; j++){
			ratio = A[indices[j]][i]/A[indices[i]][i];
			// printf("ratio: %lf \n", ratio);
			for(k=0; k<n; k++){
				A[indices[j]][k] -= ratio*A[indices[i]][k];
			}
			x[indices[j]] -= ratio*x[indices[i]];
		}
	}

	// for(int i=0; i<n; i++){
	// 	printf("%lf ", x[i]);
	// }
	// printf("\n");
	// print_matind(A,n,n,indices);
	if(determinant != 0 && b != NULL && x != NULL){
		backward_substitution_index(A,indices,x,n);
	}
	// print_matind(A,n,n,indices);
	// for(int i=0; i<n; i++){
	// 	printf("%d ", indices[i]);
	// }
	// printf("\n");
	// printf("\n");

	// printf("%lf", determinant);
	free(indices);
	return determinant;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	int* indices;
	double* temp;
	temp = (double*) malloc(sizeof(double)*n*n);
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			if(i==j)temp[n*i+j] =1.0;
			else temp[n*i+j] = 0.0;
		}
	}
	int i,j,k,maxind;
	indices = (int*) malloc(sizeof(int)*n);
	double determinant = 1.0;
	double max,ratio;

	for(i=0; i<n; i++){
		indices[i] = i;
	}

	// print_matind(B,n,n,indices);
	// printf("\n");
	for(i=0; i<n; i++){ 
		max= fabs(A[indices[i]][i]);
		maxind = i; 
		for(j=i+1; j<n; j++){
			if(fabs(A[indices[j]][i]) > max){
				max = fabs(A[indices[j]][i]);
				maxind = j;
			}
		}
		// // printf("%lf \n", max);
		// // for(int i=0; i<n; i++){
		// // 	printf("indices: %d ", indices[i]);
		// }
		// printf("\n");
		if(i != maxind){
			swap(&indices[i], &indices[maxind]);
			determinant *= -1;
		}
		if(fabs(A[indices[i]][i]) < eps){
			// printf("%lf %lf \n",fabs(A[indices[i]][i]),eps );
			determinant = 0;
		}
			
		determinant *= A[indices[i]][i];
		
		ratio = 1/A[indices[i]][i];

		for(k=0; k<n; k++){
			A[indices[i]][k] *= ratio;
			temp[n*indices[i]+k] *= ratio;
		}
		
		for(j=0; j<n; j++){
			if(j != i){
				ratio = A[indices[j]][i]/A[indices[i]][i];
				// printf("ratio: %lf \n", ratio);
				for(k=0; k<n; k++){
					A[indices[j]][k] -= ratio*A[indices[i]][k];
					temp[n*indices[j]+k] -= ratio*temp[n*indices[i]+k];
				}

			}
		}
	}
	
	// temp = (double*) malloc(sizeof(double)*n*n);
	// for(int i=0; i<n ;i++){
	// 	for(int j=0; j<n; j++){
	// 		temp[n*i + j] = B[i][j];
	// 	}
	// }

	for(int i=0; i<n ;i++){
		for(int j=0; j<n; j++){
			B[i][j] = temp[indices[i]*n+j];
		}
	}
	
	// print_matind(B,n,n,indices);
	// printf("\n");
	// for(i=n-1; i>=0; i--){
	// 	for(j=i-1; j>=0; j--){
	// 		ratio = A[indices[j]][i]/A[indices[i]][i];
	// 		// A[indices[j]][i] = 0;
	// 		for(k=0; k<n; k++){
	// 			A[indices[j]][k] -= ratio*A[indices[i]][k];
	// 			B[indices[j]][k] -= ratio*B[indices[i]][k];
	// 		}
	// 	}
	// }
	// print_matind(A,n,n,indices);
	// printf("\n");
	// print_matind(B,n,n,indices);
	// printf("\n");
	// printf("%lf", determinant);
	free(indices);
	free(temp);
	return determinant;

}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = convertToUpperTriangular(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		// case 5:
		// 	int numbers[] = {100,7,8,40,10,15};
		// 	int test[] = {5,0,1,4,2,3};
		// 	for(int i=0; i<6; i++){
		// 		int j = i;
		// 		while(i != test[j]){
		// 			j++;
		// 		}
		// 		printf("%d %d \n", i, j);
		// 		swap(&numbers[i], &numbers[j]);
		// 		swap(&test[i], &test[j]);
		// 	}
		// 	for(int i=0; i<6; i++){
		// 		printf("%d ", numbers[i]);
		// 		// printf("%d ", test[i]);
		// 	}
		// 	break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

