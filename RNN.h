#ifndef RNN_H_INCLUDED
#define RNN_H_INCLUDED

void count_Y1(int index);
void start();
void count_Y2();
void init_W();
void set_old();
double soft_plus(double x);
double d_soft_plus(double x);
void learn(int it);
void countment_increment_W2_T2(int index);
void countment_increment_W1_T1(int index);

#endif // RNN_H_INCLUDED
