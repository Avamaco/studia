// Autor: Maciej Curulak
// Code review: Jacek Kajdan
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <utility>
#include <bitset>
#include <cstring>

typedef std::pair<int, int> poz;

typedef std::bitset<100> maska;

struct ksztalt {
    std::vector<poz> czesci; // pozycje wszystkich części kształtu
    std::vector<std::vector<maska>> maski_bitowe; // maski bitowe kształtu we wszystkich możliwych miejscach na planszy
    int wys;
    int szer;
};

// przechowuje wszystkie możliwe obroty kształtu
typedef std::vector<ksztalt> klocek;

struct ruch {
    ksztalt &wstawiany;
    int numer_klocka;
    int rzad;
    int kol;
};

int wys, szer; // wymiary planszy
std::vector<klocek> klocki; // wszystkie wczytane klocki
std::bitset<10> dostepne; // czy dany klocek jeszcze nie został użyty

maska plansza; // maska bitowa zajętych pól
std::vector<ruch> uklad_klockow; // wszystkie wykonane po kolei ruchy w danym rozwiązaniu
int wolne_pola; // liczba pustych pól. Jeśli =0 to znaczy, że plansza została wypełniona


void drukuj_plansze() {
    std::vector<std::vector<char>> do_druku;
    std::vector<char> rzad;
    for (size_t i = 0; i < (size_t)szer; i++)
        rzad.push_back('.');
    for (size_t i = 0; i < (size_t)wys; i++) 
        do_druku.push_back(rzad);
    
    for (size_t i = 0; i < uklad_klockow.size(); i++) {
        ruch &r = uklad_klockow[i];
        for (size_t j = 0; j < r.wstawiany.czesci.size(); j++) {
            do_druku[r.wstawiany.czesci[j].first + r.rzad][r.wstawiany.czesci[j].second + r.kol] = (char)(r.numer_klocka + 'A');
        }
    }

    for (size_t i = 0; i < (size_t)wys; i++) {
        for (size_t j = 0; j < (size_t)szer; j++)
            printf("%c", do_druku[i][j]);
        printf("\n");
    }
    
    return;
}

// ustawia kształt w lewym górnym rogu (czyli będzie zajmował co najmniej jedno pole w 0-wej kolumnie i 0-wym wierszu)
void ustaw_w_rogu(ksztalt &k) {
    int margines_v = wys + 1;
    int margines_h = szer + 1;
    
    for (size_t i = 0; i < k.czesci.size(); i++) {
        margines_v = (margines_v < k.czesci[i].first) ? margines_v : k.czesci[i].first;
        margines_h = (margines_h < k.czesci[i].second) ? margines_h : k.czesci[i].second;
    }

    for (size_t i = 0; i < k.czesci.size(); i++) {
        k.czesci[i].first -= margines_v;
        k.czesci[i].second -= margines_h;
    }
    
    std::sort(k.czesci.begin(), k.czesci.end());

    return;
}

// oblicza wysokość i szerokość kształtu
void policz_wymiary(ksztalt &k) {
    int wys_ksztaltu = 0;
    int szer_ksztaltu = 0;
    
    for (size_t i = 0; i < k.czesci.size(); i++) {
        wys_ksztaltu = (wys_ksztaltu > k.czesci[i].first) ? wys_ksztaltu : k.czesci[i].first;
        szer_ksztaltu = (szer_ksztaltu > k.czesci[i].second) ? szer_ksztaltu : k.czesci[i].second;
    }
    
    k.wys = wys_ksztaltu + 1;
    k.szer = szer_ksztaltu + 1;

    return;
}


void obroc_o_180(ksztalt &k) {
    for (size_t i = 0; i < k.czesci.size(); i++) {
        k.czesci[i].first = wys - k.czesci[i].first - 1;
        k.czesci[i].second = szer - k.czesci[i].second - 1;
    }
    ustaw_w_rogu(k);

    return;
}

// obraca o 90 stopni zgodnie z ruchem wskazówek zegara. Zwraca true jeśli się udało, false jeśli się nie udało.
bool obroc_o_90(ksztalt &k) {
    if (k.szer > szer || k.wys > wys)
        return false;
    
    for (size_t i = 0; i < k.czesci.size(); i++)
        k.czesci[i] = std::make_pair(k.czesci[i].second, szer - k.czesci[i].first - 1);
    
    int temp = k.wys;
    k.wys = k.szer;
    k.szer = temp;

    ustaw_w_rogu(k);

    return true;
}

// generuje maski bitowe wszystkich położeń kształtu k
void generuj_maski(ksztalt &k) {
    k.maski_bitowe.clear();
    std::vector<maska> maska_rzedu;
    maska pierwsza = 0;
    for (size_t i = 0; i < k.czesci.size(); i++) {
        pierwsza[k.czesci[i].first * szer + k.czesci[i].second] = 1;
    }
    
    maska_rzedu.push_back(pierwsza);
    for (size_t i = 1; i < (size_t)szer; i++)
        maska_rzedu.push_back(maska_rzedu[i - 1] << 1);
    
    k.maski_bitowe.push_back(maska_rzedu);
    for (size_t i = 1; i < (size_t)wys; i++) {
        for (size_t j = 0; j < (size_t)szer; j++)
            maska_rzedu[j] = maska_rzedu[j] << szer;
        k.maski_bitowe.push_back(maska_rzedu);
    }

    return;
}


ksztalt wczytaj_ksztalt() {
    ksztalt wynik;
    for (size_t rzad = 0; rzad < (size_t)wys; rzad++) {
        for (size_t kol = 0; kol < (size_t)szer; kol++) {
            char znak;
            int czyok = scanf(" %c", &znak);
            if (czyok == 0)
                printf("Błędne wejście\n");
            if (znak != '.')
                wynik.czesci.push_back(std::make_pair(rzad, kol));
        }
    }

    ustaw_w_rogu(wynik);
    policz_wymiary(wynik);

    return wynik;
}


klocek generuj_klocek(ksztalt k) {
    klocek wynik;
    ustaw_w_rogu(k);
    wynik.push_back(k);

    ksztalt obrot = k;
    obroc_o_180(obrot);

    if (obrot.czesci == k.czesci) {
        bool obracalny = obroc_o_90(obrot);
        if (obracalny && obrot.czesci != k.czesci)
            wynik.push_back(obrot);
    }
    else {
        wynik.push_back(obrot);
        bool obracalny = obroc_o_90(obrot);
        if (obracalny) {
            wynik.push_back(obrot);
            obroc_o_180(obrot);
            wynik.push_back(obrot);
        }
    }

    for (size_t i = 0; i < wynik.size(); i++)
        generuj_maski(wynik[i]);
    
    return wynik;
}

///////////////////////////////////////////////////////////////////////////////////////

bool czy_pasuje(ruch &r) {
    if (r.rzad < 0 || r.kol < 0 || r.rzad + r.wstawiany.wys > wys || r.kol + r.wstawiany.szer > szer)
        return false; // wychodzi poza planszę
    
    return (plansza & r.wstawiany.maski_bitowe[r.rzad][r.kol]).none();
}

// znajduje pierwsze niezajęte pole (idąc z góry na dół, od lewej do prawej)
poz nastepne_wolne() {
    plansza.flip();
    int p = (int)plansza._Find_first();
    plansza.flip();
    return std::make_pair(p / szer, p % szer);
}


void mozliwe_ruchy(poz pierwsze_wolne, std::vector<ruch> &wynik) {
    for (size_t i = 0; i < klocki.size(); i++) {
        if (dostepne[i] == false)
            continue;
        for (size_t j = 0; j < klocki[i].size(); j++) {
            ksztalt &badany_ksztalt = klocki[i][j];
            // dzięki posortowaniu części w kształcie, pierwszy fragment kształtu jest maksymalnie do góry i na lewo.
            // offset liczy jak daleko od lewej krawędzi ten element się znajduje.
            // przykłady: XX               .X               ..X
            //            XX - offset 0    XX - offset 1    XXX - offset 2
            int offset = badany_ksztalt.czesci[0].second;
            ruch r = {badany_ksztalt, (int)i, pierwsze_wolne.first, pierwsze_wolne.second - offset};

            if (czy_pasuje(r))
                wynik.push_back(r);
        }
    }

    return;
}


void ustaw_klocek(ruch &r) {
    plansza |= r.wstawiany.maski_bitowe[r.rzad][r.kol];
    uklad_klockow.push_back(r);
    wolne_pola -= (int)r.wstawiany.czesci.size();
    dostepne[r.numer_klocka] = false;
    return;
}


void usun_klocek(ruch &r) {
    plansza ^= r.wstawiany.maski_bitowe[r.rzad][r.kol];
    uklad_klockow.pop_back();
    wolne_pola += (int)r.wstawiany.czesci.size();
    dostepne[r.numer_klocka] = true;
    return;
}

// backtrack.
bool szukaj_ustawienia() {
    if (wolne_pola == 0)
        return true;
    
    std::vector<ruch> nastepne;
    mozliwe_ruchy(nastepne_wolne(), nastepne);

    for (size_t i = 0; i < nastepne.size(); i++) {
        ustaw_klocek(nastepne[i]);
        if (szukaj_ustawienia())
            return true;
        usun_klocek(nastepne[i]);    
    }

    return false;
}


int main() {
    // Wczytywanie danych
    int k;
    int czyok = scanf("%d%d%d", &wys, &szer, &k);
    if (czyok != 3) {
        printf("Błędne wejście\n");
        return 0;
    }

    plansza = 0;
    wolne_pola = wys * szer;

    for (int i = 0; i < k; i++) {
        ksztalt ksz = wczytaj_ksztalt();
        klocki.push_back(generuj_klocek(ksz));
        dostepne[i] = true;
    }

    // Uruchomienie backtracka
    bool mozliwe = szukaj_ustawienia();

    // Drukowanie wyniku
    if (mozliwe) {
        printf("TAK\n");
        drukuj_plansze();
    }
    else {
        printf("NIE\n");
    }

    return 0;
}