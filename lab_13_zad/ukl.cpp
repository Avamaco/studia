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
std::vector<std::vector<int>> plansza;
int wolne_pola;


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
    
    std::sort(wynik.begin(), wynik.end());

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


void init_plansza() {
    wolne_pola = wys * szer;
    std::vector<int> linia (szer, 0);
    for (int i = 0; i < wys; i++)
        plansza.push_back(linia);
    
    return;
}


void drukuj_plansze() {
    for (int i = 0; i < wys; i++) {
        for (int j = 0; j < szer; j++) {
            char znak = plansza[i][j] + 'A' - 1;
            printf("%c", znak);
        }
        printf("\n");
    }
    return;
}


struct ruch {
    ksztalt wstawiany;
    int rzad;
    int kol;
    int numer_klocka;
};


bool czy_pasuje(ruch r) {
    for (int i = 0; i < r.wstawiany.size(); i++) {
        int badany_rzad = r.wstawiany[i].first + r.rzad;
        int badana_kol = r.wstawiany[i].second + r.kol;
        if (badany_rzad >= wys || badana_kol >= szer || plansza[badany_rzad][badana_kol] != 0)
            return false;
    }
    return true;
}


std::vector<ruch> mozliwe_ruchy(int ostatni_klocek) {
    std::vector<ruch> wynik;

    std::vector<std::vector<int>> osiagalne_pola = plansza;

    for (int i = ostatni_klocek + 1; i <= klocki.size(); i++) {
        for(int j = 0; j < klocki[i].size(); j++) {
            for (int rzad = 0; rzad < wys; rzad++) {
                for (int kol = 0; kol < szer; kol++) {
                    ruch r = {klocki[i][j], rzad, kol, i};
                    if (czy_pasuje(r)) {
                        wynik.push_back(r);
                        for (int x = 0; x < r.wstawiany.size(); x++)
                            osiagalne_pola[r.wstawiany[x].first + r.rzad][r.wstawiany[x].second + r.kol] = 1;
                    }
                }
            }
        }
    }

    for (int rzad = 0; rzad < wys; rzad++) {
        for (int kol = 0; kol < szer; kol++) {
            if (osiagalne_pola[rzad][kol] == 0)
                wynik.clear();
        }
    }

    return wynik;
}


void ustaw_klocek(ruch r) {
    wolne_pola -= r.wstawiany.size();
    for (int i = 0; i < r.wstawiany.size(); i++)
        plansza[r.wstawiany[i].first + r.rzad][r.wstawiany[i].second + r.kol] = r.numer_klocka;
    
    return;
}


void usun_klocek(ruch r) {
    wolne_pola += r.wstawiany.size();
    for (int i = 0; i < r.wstawiany.size(); i++)
        plansza[r.wstawiany[i].first + r.rzad][r.wstawiany[i].second + r.kol] = 0;
    
    return;
}


bool szukaj_ustawienia(int ostatni_klocek) {
    if (wolne_pola == 0)
        return true;
    
    std::vector<ruch> nastepne = mozliwe_ruchy(ostatni_klocek);

    for (int i = 0; i < nastepne.size(); i++) {
        ustaw_klocek(nastepne[i]);
        if (szukaj_ustawienia(nastepne[i].numer_klocka))
            return true;
        usun_klocek(nastepne[i]);    
    }

    return false;
}


int main() {
    klocek atrapa;
    klocki.push_back(atrapa); // żeby klocki były ponumerowane od 1

    scanf("%d%d%d", &wys, &szer, &k);
    init_plansza();

    for (int i = 0; i < k; i++) {
        klocki.push_back(generuj_obroty(wczytaj_ksztalt()));
    }

    bool mozliwe = szukaj_ustawienia(0);

    if (mozliwe) {
        printf("TAK\n");
        drukuj_plansze();
    }
    else {
        printf("NIE\n");
    }

    return 0;
}