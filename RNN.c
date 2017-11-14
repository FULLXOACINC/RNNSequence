#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RNN.h"


float **X;
float *Y1;
float Y2;

float *W_Y1;
float W_Y2;

float *old_Y1;
float old_Y2;

float **W1;
float  *W2;

float **Y1_W;
float *Y2_W;

float  *T1;
float   T2;

float  *G1;
float   G2;

float  *D;

int x_size;
int y1_size;
int lern_size;

void init_W(){

    W1=(float **)malloc(y1_size*sizeof(float*));
    for(int i = 0; i < y1_size; i++)
        W1[i] = (float *)malloc(x_size * sizeof(float));

    W2=(float *)malloc(y1_size * sizeof(float));

    Y1_W=(float **)malloc(y1_size*sizeof(float*));
    for(int i = 0; i < y1_size; i++)
        Y1_W[i] = (float *)malloc(y1_size * sizeof(float));

    Y2_W=(float *)malloc(y1_size * sizeof(float));

    srand(time(NULL));
    for(int i=0; i<y1_size; i++)
        for(int j=0; j<x_size; j++)
            W1[i][j]=((((float)rand()/(float)(RAND_MAX)) * 2)-1);

    for(int j=0; j<y1_size; j++)
        W2[j]=((((float)rand()/(float)(RAND_MAX)) * 2)-1);

    for(int i=0; i<y1_size; i++)
        for(int j=0; j<y1_size; j++)
            Y1_W[i][j]=((((float)rand()/(float)(RAND_MAX)) * 2)-1);

    for(int j=0; j<y1_size; j++)
        Y2_W[j]=((((float)rand()/(float)(RAND_MAX)) * 2)-1);

}
void start(){

    int size=16;
    double sequence[16];
    char choose;
    printf("Choose sequence\n");
    printf("1) Fibonacci number\n");
    printf("2) Periodic function T = 3 (42, -17, 02, 42, ...)\n");
    printf("3) 2^x \n");
    printf("4) x^2\n");
    printf("5) Input yourself\n");

    choose=getchar();

    switch (choose) {
    case '1':{
        double tmp[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610};
        size = 10;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case '2':{
        double tmp[] =  {42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42};
        size = 10;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case '3':{
        double tmp[] =  {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
        size = 10;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case '4':{
        double tmp[] =  {1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256};
        size = 10;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    default:
        printf("Input error\n");
        exit(0);

    }

    x_size=5;
    y1_size=2;
    lern_size=size-x_size;

    X=(float **)malloc(lern_size*sizeof(float*));
    for(int i = 0; i < lern_size; i++)
        X[i] = (float *)malloc(x_size * sizeof(float));

    Y1=(float *)malloc(y1_size*sizeof(float));

    W_Y1=(float *)malloc(y1_size*sizeof(float));

    old_Y1=(float *)malloc(y1_size*sizeof(float));

    D=(float *)malloc(lern_size*sizeof(float));

    T1=(float *)malloc(y1_size*sizeof(float));

    G1=(float *)malloc(y1_size*sizeof(float));

    for(int i=0;i<lern_size;i++)
        memset(X[i],0,x_size * sizeof(float));

    memset(Y1,0,y1_size * sizeof(float));
    memset(old_Y1,0,y1_size * sizeof(float));
    memset(T1,0,y1_size * sizeof(float));
    memset(G1,0,y1_size * sizeof(float));
    Y2=0;
    T2=0;
    old_Y2=0;
    G2=0;

    int offset=0;
    for (int index = 0; index < lern_size; index++){

        for (int i = 0; i < x_size; i++){
            X[index][i]=sequence[offset+i];

        }
        D[index]=sequence[offset+x_size];
        offset++;
    }
}

void count_Y1(int index){
    for(int i=0;i<y1_size;i++){
            W_Y1[i]=0.0;
        for(int j=0;j<x_size;j++){
            W_Y1[i]+=X[index][j]*W1[i][j]-T1[i];
        }
    }
    for(int i=0;i<y1_size;i++){
        for(int j=0;j<y1_size;j++){
            W_Y1[i]+=old_Y1[j]*Y1_W[i][j]-T1[i];
        }
    }
    for(int j=0;j<y1_size;j++){
            W_Y1[j]+=old_Y2*Y2_W[j]-T1[j];
    }

    for(int j=0;j<y1_size;j++){
            Y1[j]=soft_plus(W_Y1[j]);
    }
}

void count_Y2(){
    W_Y2=0.0;
    for(int j=0;j<y1_size;j++){
        W_Y2+=(Y1[j]*W2[j])-T2;
    }
    Y2=soft_plus(W_Y2);
}

float soft_plus(float x){
     float return_value;

     return_value = log(1 + exp(x));

     return return_value;
}

float d_soft_plus(float x)
{
     float return_value;

     return_value = exp(x)/(1 + exp(x));

     return return_value;
}


void lern(){
    float e=0.1;
    int k=0;
    float E;
    do
    {
        k++;
        E=0;


        for(int i=0; i<lern_size; i++)
        {
            count_Y1(i);
            count_Y2(i);
            E+=(Y2-D[i])*(Y2-D[i])/2;
            countment_increment_W2_T2(i);
            countment_increment_W1_T1(i);
            set_old();
        }

        for(int j=0;j<y1_size;j++){
            old_Y1[j]=0;
        }
        old_Y2=0;

        getchar();
        //sleep(1);

        printf("Error = %f\n",E);
    }while(E>e);
    for(int i=0; i<lern_size; i++)
        {
            count_Y1(i);
            count_Y2(i);
            printf("%f %f\n",Y2,D[i]);

        }
    //print_all();

    printf("OK\n");
}

void countment_increment_W2_T2(int index){
    G2=Y2-D[index];
    float A =0.1;
    //*Y2*(1-Y2)
    float temp =A*G2*d_soft_plus(W_Y2);

    for(int j=0; j<y1_size; j++)
        G1[j]=G2*W2[j];

    for(int j=0; j<y1_size; j++)
        W2[j]-=temp*Y1[j];

    T2+=temp;
}

void countment_increment_W1_T1(int index){
    float A =0.1;

    for(int j=0; j<y1_size; j++)
        T1[j]+=A*G1[j]*d_soft_plus(W_Y1[j]);

    for(int i=0;i<y1_size;i++){
        for(int j=0;j<x_size;j++){
            W1[i][j]-=A*(G1[i])*d_soft_plus(W_Y1[i])*X[index][j];
        }
    }
    for(int i=0;i<y1_size;i++){
        for(int j=0;j<y1_size;j++){
            Y1_W[i][j]-=A*G1[i]*d_soft_plus(W_Y1[i])*old_Y1[j];
        }
    }
    for(int j=0;j<y1_size;j++){
            Y2_W[j]-=A*G1[j]*d_soft_plus(W_Y1[j])*old_Y2;
    }
}

void set_old(){
    for(int j=0;j<y1_size;j++){
        old_Y1[j]=Y1[j];
    }
    old_Y2=Y2;
}


void print_all(){
    printf("\nW1:\n");
    for(int i=0; i<x_size; i++){
        for(int j=0; j<y1_size; j++)
            printf("%f ",W1[i][j]);
        printf("\n");
    }

    printf("\nW2:\n");
    for(int j=0; j<y1_size; j++)
        printf("%f ",W2[j]);

    printf("\n");
    printf("\nY1_W:\n");
    for(int i=0; i<y1_size; i++){
        for(int j=0; j<y1_size; j++)
            printf("%f ",Y1_W[i][j]);
        printf("\n");
    }

    printf("\nY2_W:\n");
    for(int j=0; j<y1_size; j++)
        printf("%f ",Y2_W[j]);
    printf("\n");
/*
    printf("\nX\n");
    for(int i=0; i<lern_size; i++){
        for(int j=0; j<x_size; j++)
            printf("%f ",X[i][j]);
        printf("   %f\n",D[i]);
    }
*/
    printf("\nY1\n");
    for(int j=0; j<y1_size; j++)
            printf("%f ",Y1[j]);
    printf("\n");

    printf("\nY2\n%f\n",Y2);

    printf("\nW_Y1\n");
    for(int j=0; j<y1_size; j++)
            printf("%f ",W_Y1[j]);
    printf("\n");

    printf("\nW_Y2\n%f\n",W_Y2);

    printf("\nT1\n");
    for(int j=0; j<y1_size; j++)
            printf("%f ",T1[j]);
    printf("\n");

    printf("\nT2\n%f\n",T2);

}





