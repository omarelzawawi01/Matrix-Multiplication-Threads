#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

typedef struct
{
    int* A;
    int* B;
    int* C;
    int l,m,n,row,col;
}matrix;

matrix* matrix_init(int* a,int* b,int* c,int l,int m, int n, int row, int col)
{
    matrix* mat = (matrix*) malloc(sizeof(matrix));
    mat->A=a;
    mat->B=b;
    mat->C=c;
    mat->col=col;
    mat->l=l;
    mat->m=m;
    mat->n=n;
    mat->row=row;
    return mat;
}

void* p_matmult_v1(void* e)
{
    matrix* mat =(matrix*)e;
    int sum=0;
    for(int i=0; i<mat->m;i++)
    {
        sum+=Item(mat->A,mat->row,i,mat->m)*Item(mat->B,i,mat->col,mat->n);
    }
    Item(mat->C,mat->row,mat->col,mat->n)=sum;
    pthread_exit(0);    

}
/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }
}



/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t elements[l*n];
    matrix* mat[l*n];
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<n;j++)
        {
            mat[j + i*n]=matrix_init(A,B,C,l,m,n,i,j);
            pthread_create(&Item(elements, i, j ,n),NULL,p_matmult_v1,Item(mat, i, j ,n));
        }
    }
    for(int i=0;i<l;i++)
    {
        for(int j=0;j<n;j++)
        {
            pthread_join(Item(elements, i, j , n),NULL);
            
        }
    }
    
}

void* p_matmult_v2(void* e)
{
    matrix* mat =(matrix*)e;
    int sum=0;
    for(int i=0; i<mat->n;i++)
    {
        sum=0;
        for(int j=0;j<mat->m;j++)
        {
            sum+=Item(mat->A,mat->row,j,mat->m)*Item(mat->B,j,i,mat->n);
        }
        Item(mat->C,mat->row,i,mat->n)=sum;
    }

}
/*
 * matrix multiplication
 *     C = A*B
 *     A has l rows and m cols
 *     B has m rows and n cols
 *     C has l rows and n cols
 */
void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
    pthread_t elements[l];
    matrix* mat[l];
    for(int i=0;i<l;i++)
    {
        mat[i]=matrix_init(A,B,C,l,m,n,i,i);
        pthread_create(&elements[i],NULL,p_matmult_v2,mat[i]);
    }
    for(int i=0;i<l;i++)
    {
        pthread_join(elements[i],NULL);
    }
    
    
}
