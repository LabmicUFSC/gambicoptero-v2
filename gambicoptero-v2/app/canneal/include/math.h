#ifndef MATH_H
#define MATH_H

double log(double x);
double sqrt(double x);
double __kernel_cos(double x, double y);
double __kernel_sin(double x, double y, int iy);
double fabs(double x);
double floor(double x);
double copysign(double x, double y);
double scalbn (double x, int n);
int __kernel_rem_pio2(double *x, double *y, int e0, int nx, int prec, const int *ipio2);
int __ieee754_rem_pio2(double x, double *y);
double cos(double x);
double exp(double x);

#endif
