#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


typedef struct _matrix{
    int rows;
    int cols;
    double** data;
}Matrix;

void printmat(Matrix*pm){
    for(int i=0;i<pm->rows; i++){
        for(int j=0; j<pm->cols; j++){
            printf("%.2f ",pm->data[i][j]);
        }
        printf("\n");
    }
}

int create_matrix(Matrix* pmatrix, int rows,int cols){
    if(rows<1 || cols <1)return 0;

    pmatrix->rows = rows;
    pmatrix->cols = cols;

    pmatrix->data = (double**)malloc(rows*sizeof(double*));
    for(int i=0; i<rows; i++){
        pmatrix->data[i] = (double*)calloc(cols,sizeof(double)); 
    }
    return 1;
}


int get(Matrix*pm, int row, int col, double* pvalue){
    if(pm->data == NULL || row <1 || row > pm->rows || col<1 || row > pm->rows) return 0;

    pvalue = *(pm->data + row) + col;

    return 1;
}

int set(Matrix*pm, int row, int col, double value){
    if(pm->data == NULL || row <0 || row > pm->rows || col<0 || row > pm->rows) return 0;

    *(*(pm->data + row) + col) = value;
    return 1;
}


void create_identity_matrix(Matrix*pm, int size){
    create_matrix(pm,size,size);
    for(int i=0; i<size; i++){
        set(pm,i,i,1);    
    }

}

double dot_prod_vect(const double*v1, const double* v2, int size){
    double result = 0;
    for(int i=0; i<size; i++){
        result += v1[i]*v2[i];
    } 

    return result;

}

void mul_vect(double* result, const Matrix* pm, const double* vect){
    for(int i=0; i<pm->rows; i++){
        result[i] = dot_prod_vect(*(pm->data +i),vect,pm->cols);
    }
}


void max_element_in_rows(double* maxdata, const Matrix* pm){
    for(int i=0; i<pm->rows; i++){
        double max = -1e9;
        for(int j=0; j<pm->cols; j++){
            if(pm->data[i][j] >max) max = pm->data[i][j];
        }
        maxdata[i] = max;
    }
}



int main(void){
    Matrix mat;
    create_matrix(&mat,3,4);
    for(int i=0; i<3*4; i++){
        set(&mat,i/4,i%4,i);
    }
    printmat(&mat);
    double maxdata[3];
    max_element_in_rows(maxdata,&mat);
    printf("\n");
    for(int i=0; i<3; i++){
        printf("%.2f ", maxdata[i]);
    }
    printf("\n");

    double tomul[3] = {3.2,5,3};
    double result[3];
    mul_vect(result,&mat,tomul);

    for(int i=0; i<3; i++){
        printf("%.2f ",result[i]);
    }

    return 0;
}