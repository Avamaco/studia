#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int>> ksztalt;

struct klocek {
    int ile_obrotow;
    std::vector<ksztalt> obroty;
};

int wys, szer, k;
std::vector<klocek> klocki;


ksztalt init_ksztalt() {
    ksztalt wynik;
    std::vector<int> linia (szer, 0);
    for (int i = 0; i < wys; i++) {
        wynik.push_back(linia);
    }
    return wynik;
}


void drukuj_ksztalt(ksztalt xd) {
    for (int i = 0; i < wys; i++) {
        for (int j = 0; j < szer; j++) {
            if (xd[i][j] == 0)
                printf(".");
            else
                printf("X");
        }
        printf("\n");
    }
    return;
}

// ustawia kształt w lewym górnym rogu (czyli będzie zajmował co najmniej jedno pole w 0-wej kolumnie i 0-wym wierszu)
ksztalt ustaw_w_rogu(ksztalt xd) {
    int margines_h = -1;
    int margines_v = -1;

    for (int i = 0; i < wys; i++) {
        for (int j = 0; j < szer; j++) {
            if (xd[i][j] != 0) {
                margines_v = i;
                break;
            }
        }
        if (margines_v > -1)
            break;
    }
    for (int j = 0; j < szer; j++) {
        for (int i = 0; i < wys; i++) {
            if (xd[i][j] != 0) {
                margines_h = j;
                break;
            }
        }
        if (margines_h > -1)
            break;
    }

    ksztalt wynik = init_ksztalt();

    for (int i = margines_v; i < wys; i++) {
        for (int j = margines_h; j < szer; j++) {
            wynik[i - margines_v][j - margines_h] = xd[i][j];
        }
    }

    return wynik;
}


ksztalt obroc_o_180(ksztalt xd) {
    for (int i = 0; i < wys / 2; i++) {
        for (int j = 0; j < szer; j++) {
            int temp = xd[i][j];
            xd[i][j] = xd[wys - i - 1][szer - j - 1];
            xd[wys - i - 1][szer - j - 1] = temp;
        }
    }
    xd = ustaw_w_rogu(xd);
    return xd;
}


klocek generuj_obroty(ksztalt xd) {
    xd = ustaw_w_rogu(xd);
    klocek wynik;
    wynik.ile_obrotow = 1;
    wynik.obroty.push_back(xd);


}


ksztalt wczytaj_ksztalt() {
    ksztalt wynik = init_ksztalt();
    for (int rzad = 0; rzad < wys; rzad++) {
        for (int kol = 0; kol < szer; kol++) {
            char znak;
            scanf(" %c", &znak);
            if (znak != '.')
                wynik[rzad][kol] = 1;
        }
    }
    return wynik;
}


int main() {
    scanf("%d%d%d", &wys, &szer, &k);

    for (int i = 0; i < k; i++) {
        ksztalt ksz = wczytaj_ksztalt();
        drukuj_ksztalt(ksz);
        ksz = ustaw_w_rogu(ksz);
        drukuj_ksztalt(ksz);
        drukuj_ksztalt(obroc_o_180(ksz));
    }

    

    return 0;
}