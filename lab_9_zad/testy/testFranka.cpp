#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "kol.h"
#include <bits/stdc++.h>
typedef long long ll;
using namespace std;

mt19937_64 rng(1337);
int rd(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

ll mod = 954235538129;
ll base = 877411;

int main() {
    const int n = 100000;
    const int u = 101;
    otwarcie_urzedu(u);
    vector<interesant*> vi(n);
    ll hsh = 0;
    for(int i = 0; i < n; i++) {
        if(rd(0, 2) == 0)
            vi[i] = nowy_interesant(0);
        else
            vi[i] = nowy_interesant(rd(0, u - 2));
        naczelnik(0);
        naczelnik(rd(0, u - 1));
        if(i % 7 == 0) {
            int x = rd(0, u - 1);
            if(rd(0, 1) == 0)
                naczelnik(x);
            interesant* in = obsluz(x);
            int val = (in == NULL ? 0 : numerek(in) + 1);
            hsh = (hsh * base + val) % mod;
        }
        if(i % 10 == 0) {
            int i1 = rd(1, u - 1);
            int i2 = i1;
            while(i2 == i1)
                i2 = rd(1, u - 1);
            zamkniecie_okienka(i1, i2);
        }
    }
    vector<interesant*> v = zamkniecie_urzedu();
    for(auto i : v)
        hsh = (hsh * base + numerek(i)) % mod;

    for(int i = 0; i < n; i++)
        free(vi[i]);

    assert(hsh == 513158497494);
    printf("Działa.");
}