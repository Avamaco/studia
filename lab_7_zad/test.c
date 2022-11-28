#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int crossProduct(double x1, double y1, double x2, double y2, double x3, double y3) {
    double p = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
    if (fabs(p) < 1e-10) return 0;
    if (p < 0) return 2;
    if (p > 0) return 1;
}

int main() {
    int n;
    int errory;

    errory = scanf("%d", &n);
    printf("%d", errory);

    return 0;
}