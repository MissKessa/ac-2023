#include <stdio.h>
void f(double * d)
{
*d = 4;
}
int main(int argc, char* argv[])
{
double d;
d = 3;
double *pD;
pD = &d;
f(pD);
printf("%f\n", d);
return 0;
}
