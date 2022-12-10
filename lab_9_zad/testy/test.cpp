#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "kol.h"

int main() {
    otwarcie_urzedu(3);

    interesant *i0 = nowy_interesant(0);
    interesant *i1 = nowy_interesant(0);
    interesant *i2 = nowy_interesant(0);

    interesant *i3 = nowy_interesant(1);
    interesant *i4 = nowy_interesant(1);
    
    interesant *i5 = nowy_interesant(2);

    // 0: i0 i1 i2
    // 1: i3 i4
    // 2: i5

    naczelnik(0);
    zamkniecie_okienka(0, 1);

    // 0:
    // 1: i3 i4 i2 i1 i0
    // 2: i5

    zmiana_okienka(i2, 2);
    naczelnik(1);
    zamkniecie_okienka(1, 2);

    // 0:
    // 1:
    // 2: i5 i2 i0 i1 i4 i3

    std::vector<interesant*> v = zamkniecie_urzedu();
    
    assert(v.size() == 6U);
    assert(v[0] == i5);
    assert(v[1] == i2);
    assert(v[2] == i0);
    assert(v[3] == i1);
    assert(v[4] == i4);
    assert(v[5] == i3);
    
    for (int i = 0; i < 6; ++i)
        free(v[i]);
    
    printf("Wszystko działa! UwU\n");

    return 0;
}