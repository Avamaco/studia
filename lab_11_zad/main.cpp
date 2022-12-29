#include <cstdio>
#include <cstdlib>
#include <vector>

#include "nextbrut.h"

int main() {
    int n, q, minval, maxval, seed;
    scanf("%d %d %d %d %d", &n, &q, &minval, &maxval, &seed);

    srand(seed);

    std::vector <int> x;

    for (int i = 0; i < n; i++) {
        int r = (rand() % (maxval - minval)) + minval;
        x.push_back(r);
        printf("%d ", r);
    }
    printf("\n\n");

    init(x);

    for (int i = 0; i < q; i++) {
        int r = rand() % n;
        int a = (rand() % (maxval - minval)) + minval;
        int b = (rand() % (maxval - minval)) + minval;
        if (a > b) {
            int temp = a;
            a = b;
            b = temp;
        }
        printf("nextInRange(%d, %d, %d) = ", r, a, b);
        printf("%d\n", nextInRange(r, a, b));
    }

    done();

    return 0;
}