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
    double x1, y1, x2, y2;
    scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);

    for (double y3 = 5.0; y3 > -5.5; y3 -= 1.0) {
        for (double x3 = -5.0; x3 < 5.5; x3 += 1.0) {
            printf("%d ", crossProduct(x1, y1, x2, y2, x3, y3));
        }
        printf("\n");
    }

    return 0;
}