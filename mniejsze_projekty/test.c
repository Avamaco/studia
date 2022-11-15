#include<stdio.h>

int main()
{
double n = 5000.0;
double iloczyn = 1.0;
for(double i = 1.0; i < n; i += 1.0){
    iloczyn *= 2.0 * i;
    iloczyn /= (2.0 * i) + 1;
    printf("%lf\n", iloczyn);
}
return 0;
}
