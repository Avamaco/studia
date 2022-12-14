#include <cstdio>
#include <cstdlib>
#include <vector>

#include "next.h"

int main() {
    int n, minval, maxval, seed;
    scanf("%d %d %d %d", &n, &minval, &maxval, &seed);

    srand(seed);

    std::vector <int> x;

    for (int i = 0; i < n; i++) {
        int r = (rand() % (maxval - minval)) + minval;
        x.push_back(r);
        printf("%d ", r);
    }
    printf("\n\n");

    init(x);

    int w = nextInRange(4, 42, 94);
    printf("Wynik: %d Poprwany wynik: 5\n", w);

    done();

    return 0;
}