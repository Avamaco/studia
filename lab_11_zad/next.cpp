#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>


typedef struct node *bin_tree;
struct node {
    long long range_min;
    long long range_max;
    long long first_in_range;
    bin_tree left;
    bin_tree right;
};


std::vector<int> sort_no_duplicates(const std::vector<int> &x) {
    std::vector<int> y = x;
    std::sort(y.begin(), y.end());
    y.erase(std::unique(y.begin(), y.end()), y.end());
    return y;
}


std::vector<int> przenumeruj(const std::vector<int> &x, std::vector<int> &klucz) {
    std::vector<int> wynik;
    for (int i = 0; i < x.size(); i++) {
        wynik.push_back(std::lower_bound(klucz.begin(), klucz.end(), x[i]) - klucz.begin());
    }
    return wynik;
}


void init(const std::vector<int> &x) {
    std::vector<int> snd = sort_no_duplicates(x);
    std::vector<int> nowy = przenumeruj(x, snd);
}