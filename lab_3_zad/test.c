/* kompilacja: gcc @opcje test.c ary.c -o test.e */
// albo prościej, make test

#include <stdio.h>
#include <math.h>

#include "stdbool.h"
#include "ary.h"

void drukuj(wartosc w){
    if (w.czy_anty)
        printf("-> [-inf, %f] U [%f, inf]\n", w.dol, w.gora);
    else
        printf("-> [%f, %f]\n", w.dol, w.gora);

    return;
}

int main() {
    wartosc zero = wartosc_dokladna(0.0);
    wartosc jeden = wartosc_dokladna(1.0);
    wartosc rzecz = podzielic(jeden, wartosc_od_do(-1.0, 1.0));
    rzecz = plus(rzecz, rzecz);
    drukuj(zero);
    drukuj(jeden);
    drukuj(rzecz);

    wartosc nic = podzielic(zero, zero);
    drukuj(nic);
    drukuj(podzielic(jeden, nic));

    printf("\n");   //:)))

    // wartosc polprosta = minus(podzielic(jeden, wartosc_od_do(0.0, 1.0)), jeden);
    // drukuj(polprosta);
    // wartosc polprosta2 = minus(zero, polprosta);
    // drukuj(razy(polprosta, polprosta2));
    // drukuj(razy(polprosta2, polprosta));
    // drukuj(razy(polprosta2, polprosta2));

    wartosc aa = minus(podzielic(jeden, wartosc_od_do(-1.0, 1.0)), jeden);
    drukuj(aa);

    drukuj(razy(aa, nic));

    return 0;
}