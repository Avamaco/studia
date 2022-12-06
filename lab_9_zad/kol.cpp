#include <cstdlib>
#include <vector>

#include "kol.h"


int numer;
int liczba_okienek;


typedef struct kolejka {
    interesant *poczatek, *koniec;
} kolejka;

kolejka *okienka;


void link(interesant *kto, interesant *old, interesant *nw) {
    if (kto->l1 == old)
        kto->l1 = nw;
    else
        kto->l2 = nw;
}


void otwarcie_urzedu(int m) {
    numer = 0;
    liczba_okienek = m;
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
    interesant *nowy = (interesant*)malloc(sizeof(interesant));

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

// pomaga iterować się po kolejnych interesantach
interesant *nastepny(interesant *obecny, interesant *poprzedni) {
    if (obecny->l1 == poprzedni)
        return obecny->l2;
    return obecny->l1;
}

// szuka w czasie liniowym, w którym kierunku względem i1 stoi i2
//  wynik 0 oznacza że to ta sama osoba
//  wynik 1 oznacza że stoi w kierunku l1
//  wynik 2 oznacza że stoi w kierunku l2
int szukajKierunku(interesant *i1, interesant *i2) {
    if (i1 == i2)
        return 0;

    interesant *lewo1 = i1;
    interesant *lewo2 = i1->l1;
    interesant *prawo1 = i1;
    interesant *prawo2 = i1->l2;

    while (true) {
        if (!lewo2)
            return 2;
        if (lewo2 == i2)
            return 1;
        if (!prawo2)
            return 1;
        if (prawo2 == i2)
            return 2;
        
        interesant *temp = lewo1;
        lewo1 = lewo2;
        lewo2 = nastepny(lewo2, temp);

        temp = prawo1;
        prawo1 = prawo2;
        prawo2 = nastepny(prawo2, temp);
    }
}


std::vector<interesant *> fast_track(interesant *i1, interesant *i2) {
    int strona = szukajKierunku(i1, i2);

    if (strona == 0) {
        std::vector<interesant *> wynik;
        wynik.push_back(i1);
        link(i1->l1, i1, i1->l2);
        link(i1->l2, i1, i1->l1);
        return wynik;
    }

    interesant *kolejny = (strona == 1) ? i1->l1 : i1->l2;
    
    interesant *temp1 = i1;
    interesant *temp2 = i1;

    std::vector<interesant *> wynik;
    wynik.push_back(i1);
    
    while (kolejny != i2) {
        wynik.push_back(kolejny);
        temp2 = temp1;
        temp1 = kolejny;
        kolejny = nastepny(kolejny, temp2);
    }

    wynik.push_back(i2);

    interesant *scal1, *scal2;
    scal1 = (strona == 1) ? i1->l2 : i1->l1;
    scal2 = nastepny(kolejny, temp1);

    link(scal1, i1, scal2);
    link(scal2, i2, scal1);

    return wynik;
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


std::vector<interesant *> zamkniecie_urzedu() {
    std::vector<interesant *> wynik;

    for (int i = 0; i < liczba_okienek; i++) {
        interesant *temp1 = okienka[i].poczatek;
        interesant *temp2 = okienka[i].poczatek;
        interesant *kolejny = okienka[i].poczatek->l2;

        while (kolejny != okienka[i].koniec) {
            wynik.push_back(kolejny);
            temp2 = temp1;
            temp1 = kolejny;
            kolejny = nastepny(kolejny, temp2);
        }

        free(okienka[i].poczatek);
        free(okienka[i].koniec);
    }

    free(okienka);

    return wynik;
}