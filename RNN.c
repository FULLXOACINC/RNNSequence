#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "RNN.h"


float **X;
float *Y1;
float Y2;

float **W1;
float  *W2;

float **Y1_W;
float *Y2_W;

float  *T1;
float   T2;


int x_size;
int y1_size;
int lern_size;

void init_W(){

    W1=(float **)malloc(x_size*sizeof(float*));
    for(int i = 0; i < x_size; i++)
        W1[i] = (float *)malloc(y1_size * sizeof(float));

    W2=(float *)malloc(y1_size * sizeof(float));

    Y1_W=(float **)malloc(y1_size*sizeof(float*));
    for(int i = 0; i < y1_size; i++)
        Y1_W[i] = (float *)malloc(y1_size * sizeof(float));

    Y2_W=(float *)malloc(y1_size * sizeof(float));

    srand(time(NULL));
    for(int i=0; i<x_size; i++)
        for(int j=0; j<y1_size; j++)
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
        size = 16;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    case '2':{
        double tmp[] =  {42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42, -17, 02, 42};
        size = 16;
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
        size = 16;
        memcpy(sequence, tmp, sizeof(tmp));
        break;
    }
    default:
        printf("Input error\n");
        exit(0);

    }

    x_size=5;
    y1_size=3;
    lern_size=size-x_size;

    X=(float **)malloc(lern_size*sizeof(float*));
    for(int i = 0; i < lern_size; i++)
        X[i] = (float *)malloc(x_size * sizeof(float));

    Y1=(float *)malloc(y1_size*sizeof(float));

    T1=(float *)malloc(y1_size*sizeof(float));

    for(int i=0;i<lern_size;i++)
        memset(X[i],0,x_size * sizeof(float));

    memset(Y1,0,y1_size * sizeof(float));
    Y2=0;
    T2=0;

    int offset=0;
    for (int index = 0; index < lern_size; index++){

        for (int i = 0; i < x_size; i++){
            X[index][i]=sequence[offset+i];
        }
        offset++;
    }
}

void count_Y1(int index){
    for(int i=0;i<y1_size;i++){
            Y1[i]=0;
        for(int j=0;j<x_size;j++){
            Y1[i]+=X[index][j]*W1[j][i];
        }
    }
    for(int i=0;i<y1_size;i++){
        for(int j=0;j<y1_size;j++){
            Y1[i]+=Y1[j]*Y1_W[j][i];
        }
    }
    for(int j=0;j<y1_size;j++){
            Y1[j]+=Y2*Y2_W[j];
    }
    for(int j=0;j<y1_size;j++){
            Y1[j]=sigmoid(Y1[j],T1[j]);
    }
}

void count_Y2(){
    Y2=0;
    for(int j=0;j<y1_size;j++){
        Y2+=Y1[j]*W2[j];
    }
    Y2=sigmoid(Y2,T2);
}

float sigmoid(float x,float T)
{
     float exp_value;
     float return_value;

     exp_value = exp((double) (-x + T));

     return_value = 1 / (1 + exp_value);

     return return_value;
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

    printf("\nX\n");
    for(int i=0; i<lern_size; i++){
        for(int j=0; j<x_size; j++)
            printf("%f ",X[i][j]);
        printf("\n");
    }

    printf("\nY1\n");
    for(int j=0; j<y1_size; j++)
            printf("%f ",Y1[j]);
    printf("\n");

    printf("\nY2\n%f\n",Y2);

    printf("\nT1\n");
    for(int j=0; j<y1_size; j++)
            printf("%f ",T1[j]);
    printf("\n");

    printf("\nT2\n%f\n",T2);

}









