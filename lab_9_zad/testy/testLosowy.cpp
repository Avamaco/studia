#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "kol.h"

struct klient {
    interesant *kto;
    int kol;
};

int znajdzBiedaka(std::vector<klient> klienci, interesant *biedak) {
    for (int i = 0; i < klienci.size(); i++){
        if (klienci[i].kto == biedak)
            return i;
    }
    return -1;
}



int main() {

    int okienka, operacje, ludzie_na_start, seed;

    scanf("%d %d %d %d", &okienka, &operacje, &ludzie_na_start, &seed);

    if (okienka < 2) {
        printf("ZA MAŁO OKIENEK!\n");
        return 0;
    }

    srand(seed);

    otwarcie_urzedu(okienka);

    std::vector<klient> klienci;
    interesant * pierwszy = nowy_interesant(0);
    klient prierwszy_klient = {pierwszy, 0};
    klienci.push_back(prierwszy_klient);
    printf("nowy_interesant(0)\n");

    for (int i = 0; i < ludzie_na_start; i++) {
        int gdzie = rand() % okienka;
        printf("nowy_interesant(%d)\n", gdzie);
        interesant * temp = nowy_interesant(gdzie);
        klient temp2 = {temp, gdzie};
        klienci.push_back(temp2);
    }

    for(int i = 0; i < operacje; i++) {
        int polecenie = rand() % 5;
        if (polecenie == 0) {
            int gdzie = rand() % okienka;
            printf("nowy_interesant(%d)\n", gdzie);
            interesant * temp = nowy_interesant(gdzie);
            klient temp2 = {temp, gdzie};
            klienci.push_back(temp2);
        }
        if (polecenie == 1) {
            int los = rand() % okienka;
            printf("obsluz(%d)\n", los);
            interesant *biedak = obsluz(los);
            if (!biedak) {
                printf("NULL ");
            }
            else {
            //printf("%d ", numerek(biedak));
            int ktory = znajdzBiedaka(klienci, biedak);
            klienci[ktory].kol = -1;
            }
        }
        if (polecenie == 2) {
            int los1 = rand() % okienka;
            int los2 = (los1 + (rand() % (okienka - 1))) % okienka;
            printf("zamkniecie_okienka(%d, %d)\n", los1, los2);
            zamkniecie_okienka(los1, los2);
        }
        if (polecenie == 3) {
            int los = rand() % okienka;
            printf("naczelnik(%d)\n", los);
            naczelnik(los);
        }
        if (polecenie == 4) {
            int los = rand() % klienci.size();
            if (klienci[los].kol != -1) {
            int dokad = (klienci[los].kol + (rand() % (okienka - 1))) % okienka;
            printf("zmiana_okienka(pan z numerem %d, %d)\n", numerek(klienci[los].kto), dokad);
            zmiana_okienka(klienci[los].kto, dokad);
            klienci[los].kol = dokad;
            }
        }
    }

    printf("Zamykanie urzedu...\n");
    std::vector<interesant *> koniec = zamkniecie_urzedu();
    printf("Zamykanie zakonczone!\n");

    for (int i = 0; i < koniec.size(); i++) {
        printf("%d ", numerek(koniec[i]));
    }
    printf("\n");

    for (int i = 0; i < klienci.size(); i++) {
        free(klienci[i].kto);
    }

    return 0;
}