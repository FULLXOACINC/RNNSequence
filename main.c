#include <stdio.h>
#include <stdlib.h>
#include "RNN.h"

int main()
{

    int size_x=8;
    int size_y=8;
    double A=1000;
    double e=1000;
    int it=8;
    printf("enter P:\n");
    scanf("%i",&size_x);
    printf("enter y1 size:\n");
    scanf("%i",&size_y);
    printf("enter max iteration count:\n");
    scanf("%i",&it);
    printf("enter max error:\n");
    scanf("%lf",&e);
    printf("enter learning step:\n");
    scanf("%lf",&A);

    start(size_x,size_y,e,A);
    init_W();
    learn(it);
    //count_Y1(0);
    //count_Y2();

    return 0;
}







