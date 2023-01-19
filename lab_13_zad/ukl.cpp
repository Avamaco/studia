#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>

// przechowuje współrzędne wszystkich pól zajętych przez kształt
typedef std::vector<std::pair<int, int>> ksztalt; 

// przechowuje wszystkie możliwe obroty danego klocka
typedef std::vector<ksztalt> klocek;

int wys, szer, k;
std::vector<klocek> klocki;


void drukuj_ksztalt(ksztalt xd) {
    printf("\n");

    std::vector<std::vector<int>> macierz;
    std::vector<int> linia (szer, 0);
    for (int i = 0; i < wys; i++)
        macierz.push_back(linia);

    for(int i = 0; i < xd.size(); i++)
        macierz[xd[i].first][xd[i].second] = 1;

    for (int i = 0; i < wys; i++) {
        for (int j = 0; j < szer; j++) {
            if (macierz[i][j] == 0)
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
    int margines_v = wys + 1;
    int margines_h = szer + 1;
    
    for (int i = 0; i < xd.size(); i++) {
        margines_v = (margines_v < xd[i].first) ? margines_v : xd[i].first;
        margines_h = (margines_h < xd[i].second) ? margines_h : xd[i].second;
    }

    ksztalt wynik;
    for (int i = 0; i < xd.size(); i++)
        wynik.push_back(std::make_pair(xd[i].first - margines_v, xd[i].second - margines_h));

    return wynik;
}


ksztalt obroc_o_180(ksztalt xd) {
    ksztalt wynik;
    for (int i = 0; i < xd.size(); i++)
        wynik.push_back(std::make_pair(wys - xd[i].first - 1, szer - xd[i].second - 1));

    wynik = ustaw_w_rogu(wynik);

    return wynik;
}


ksztalt obroc_o_90(ksztalt xd) {
    ksztalt wynik;
    for (int i = 0; i < xd.size(); i++) {
        // sprawdzanie czy po obróceniu kształt w ogóle mieści się na planszy
        if (xd[i].first >= szer || xd[i].second >= wys) {
            wynik[0].first = -1; // flaga
            return wynik; 
        }

        // obrót o 90 stopni jest złożeniem odbicia symetrycznego wzdłuż prostej ukośnej i wzdłuż prostej pionowej
        wynik.push_back(std::make_pair(xd[i].second, xd[i].first));
        wynik[i].second = szer - wynik[i].second - 1;
    }
    wynik = ustaw_w_rogu(wynik);

    return wynik;
}

// tworzy strukturę ze wszystkimi możliwymi obrotami
klocek generuj_obroty(ksztalt xd) {
    xd = ustaw_w_rogu(xd);
    klocek wynik;
    wynik.push_back(xd);

    ksztalt obrot = obroc_o_180(xd);
    if (obrot == xd) { // symetria względem punktu
        obrot = obroc_o_90(xd);
        if (obrot[0].first != -1 && obrot != xd)
            wynik.push_back(obrot);
    }
    else {
        wynik.push_back(obrot);

        obrot = obroc_o_90(xd);
        if (obrot[0].first != -1) {
            wynik.push_back(obrot);

            obrot = obroc_o_180(obrot);
            wynik.push_back(obrot);
        }
    }

    return wynik;
}


ksztalt wczytaj_ksztalt() {
    ksztalt wynik;
    for (int rzad = 0; rzad < wys; rzad++) {
        for (int kol = 0; kol < szer; kol++) {
            char znak;
            scanf(" %c", &znak);
            if (znak != '.')
                wynik.push_back(std::make_pair(rzad, kol));
        }
    }
    return wynik;
}


int main() {
    scanf("%d%d%d", &wys, &szer, &k);

    for (int i = 0; i < k; i++) {
        ksztalt ksz = wczytaj_ksztalt();
        klocek klo = generuj_obroty(ksz);
        for (int i = 0; i < klo.size(); i++) {
            drukuj_ksztalt(klo[i]);
        }
    }

    

    return 0;
}