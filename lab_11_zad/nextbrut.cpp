// brut
#include <cstdio>
#include <cstdlib>
#include <vector>

int *liczby;
int rozmiar;

void init(const std::vector<int> &x) {
    rozmiar = x.size();
    liczby = (int *)malloc(rozmiar * sizeof(int));
    for (int i = 0; i < rozmiar; i++)
        liczby[i] = x[i];
    return;
}


int nextInRange(int i, int a, int b) {
    for (int j = i; j < rozmiar; j++)
        if (liczby[j] >= a && liczby[j] <= b) return j;
    return -1;    
}


void done() {
    free(liczby);
}