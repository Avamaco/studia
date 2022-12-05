#include <cstdlib>
#include <vector>

#include "kol.h"


int numer;
kolejka *okienka; 


struct kolejka {
    interesant *poczatek, *koniec;
}


void link(interesant *kto, interesant *old, interesant *nw) {
    if (kto->l1 == old)
        kto->l1 = nw;
    else
        kto->l2 = nw;
}


void otwarcie_urzedu(int m) {
    numer = 0;
    okienka = (kolejka*) malloc((size_t)m * sizeof(kolejka));

    for (int i = 0; i < m; i++) {
        okienka[i].poczatek = (interesant*) malloc(sizeof(interesant));
        okienka[i].koniec = (interesant*) malloc(sizeof(interesant));

        okienka[i].poczatek->l1 = NULL;
        okienka[i].poczatek->l2 = okienka[i].koniec;
        okienka[i].koniec->l1 = okienka[i].poczatek;
        okienka[i].koniec->l2 = NULL;

        okienka[i].poczatek->numerek = -1;
        okienka[i].koniec->numerek = -2;
    }
    return;
}


void idz_na_koniec(interesant *kto, int k) {
    interesant *koniec_kolejki = okienka[k].koniec;
    interesant *ostatni = koniec_kolejki->l1;

    kto->l1 = ostatni;
    kto->l2 = koniec_kolejki;
    koniec_kolejki->l1 = kto;
    link(ostatni, koniec_kolejki, kto);

    return;
}


interesant *nowy_interesant(int k) {
    interesant *nowy = malloc(sizeof(interesant));

    nowy->numerek = numer;
    numer++;

    idz_na_koniec(nowy, k);

    return nowy;
}


int numerek(interesant *i) {
    return i->numerek;
}


interesant *obsluz(int k) {
    if (okienka[k].poczatek->l2 == okienka[k].koniec)
        return NULL;
    
    interesant *poczatek_kolejki = okienka[k].poczatek;
    interesant *pierwszy = poczatek_kolejki->l2;
    interesant *drugi = pierwszy->l2;

    poczatek_kolejki->l2 = drugi;
    link(drugi, pierwszy, poczatek_kolejki);

    return pierwszy;
}


void zmiana_okienka(interesant *i, int k) {
    link(i->l1, i, i->l2);
    link(i->l2, i, i->l1);

    idz_na_koniec(i, k);

    return;
}


void zamkniecie_okienka(int k1, int k2) {
    if (okienka[k1].poczatek->l2 == okienka[k1].koniec) return;

    interesant *pierwszy = okienka[k1].poczatek->l2;
    interesant *ostatni = okienka[k1].koniec->l1;
    interesant *ostatni2 = okienka[k2].koniec->l1;

    okienka[k1].poczatek->l2 = okienka[k1].koniec;
    okienka[k1].koniec->l1 = okienka[k1].poczatek;

    link(pierwszy, okienka[k1].poczatek, ostatni2);
    link(ostatni2, okienka[k2].koniec, pierwszy);

    link(ostatni, okienka[k1].koniec, okienka[k2].koniec);
    okienka[k2].koniec->l1 = ostatni;

    return;
}


std::vector<interesant *> fast_track(interesant *i1, interesant *i2) {
    // TODO
}


void naczelnik(int k) {
    interesant *pierwszy = okienka[k].poczatek->l2;
    interesant *ostatni = okienka[k].koniec->l1;

    link(pierwszy, okienka[k].poczatek, okienka[k].koniec);
    okienka[k].koniec->l1 = pierwszy;
    link(ostatni, okienka[k].koniec, okienka[k].poczatek);
    okienka[k].poczatek->l2 = ostatni;

    return;
}