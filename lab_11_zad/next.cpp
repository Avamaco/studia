#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>


typedef struct node *bin_tree;
struct node {
    long long range_min;
    long long range_max;
    long long first_in_range;
    bin_tree left;
    bin_tree right;
};


std::vector<bin_tree> korzenie;
int rozmiar;
std::vector<int> snd;
std::vector<bin_tree> wskazniki;

// rezerwuje pamięć dla wierzchołka w drzewie oraz dodaje go do wektora wszystkich mallocowanych wskaźników dla łatwego usunięcia na koniec
bin_tree utworz_wierzcholek() {
    bin_tree wynik = (bin_tree)malloc(sizeof(node));
    wskazniki.push_back(wynik);
    return wynik;
}


std::vector<int> sort_no_duplicates(const std::vector<int> &x) {
    std::vector<int> y = x;
    std::sort(y.begin(), y.end());
    y.erase(std::unique(y.begin(), y.end()), y.end());
    return y;
}


int przenumeruj(int x) {
    int wynik = std::lower_bound(snd.begin(), snd.end(), x) - snd.begin();
    return wynik;
}


int nastepna_potega_dwojki(int x) {
    int wynik = 1;
    while (wynik < x)
        wynik *= 2;
    return wynik;
}


bin_tree puste_drzewo_przedzialowe(int size, int min_wartosc) {
    bin_tree wynik = utworz_wierzcholek();
    wynik->first_in_range = rozmiar;
    wynik->range_min = min_wartosc;
    wynik->range_max = min_wartosc + size - 1;
    
    if(size == 1) {
        wynik->left = NULL;
        wynik->right = NULL;
        return wynik;
    }

    wynik->left = puste_drzewo_przedzialowe(size / 2, min_wartosc);
    wynik->right = puste_drzewo_przedzialowe(size / 2, min_wartosc + size / 2);

    return wynik;
}

// Tworzy drzewo, które część wierzchołków ma wspólną z poprzednim drzewem. Tylko na jednej ścieżce do liścia zmieniają się wartości.
void nowy_korzen(int numer_drzewa, int liczba) {
    int size = nastepna_potega_dwojki(rozmiar);
    korzenie[numer_drzewa] = utworz_wierzcholek();
    bin_tree stare_drzewo = korzenie[numer_drzewa + 1];
    bin_tree nowe_drzewo = korzenie[numer_drzewa];
    int temp = 0;
    while (size > 1) {
        nowe_drzewo->first_in_range = numer_drzewa;
        nowe_drzewo->range_min = temp;
        nowe_drzewo->range_max = temp + size - 1;

        if (liczba >= temp + size / 2) {
            nowe_drzewo->left = stare_drzewo->left;
            nowe_drzewo->right = utworz_wierzcholek();
            temp += size / 2;
            stare_drzewo = stare_drzewo->right;
            nowe_drzewo = nowe_drzewo->right;
        }
        else {
            nowe_drzewo->left = utworz_wierzcholek();
            nowe_drzewo->right = stare_drzewo->right;
            stare_drzewo = stare_drzewo->left;
            nowe_drzewo = nowe_drzewo->left;
        }
        size /= 2;
    }

    // tworzenie liścia
    nowe_drzewo->first_in_range = numer_drzewa;
    nowe_drzewo->range_min = temp;
    nowe_drzewo->range_max = temp;
    nowe_drzewo->left = NULL;
    nowe_drzewo->right = NULL;

    return;
}

// tworzy tyle drzew przedziałowych ile liczb ma wektor x. Te drzewa mają wspólne wierzchołki.
void init(const std::vector<int> &x) {
    snd = sort_no_duplicates(x);
    
    rozmiar = x.size();
    korzenie.resize(rozmiar + 1);

    korzenie[rozmiar] = puste_drzewo_przedzialowe(nastepna_potega_dwojki(rozmiar), 0);
    for (int i = rozmiar - 1; i >= 0; i--)
        nowy_korzen(i, przenumeruj(x[i]));

    return;
}


int _nextInRange(bin_tree t, int a, int b) {
    if (t->range_max < a || t->range_min > b)
        return rozmiar + 1;

    if (t->range_min >= a && t->range_max <= b)
        return t->first_in_range;
    
    int min1 = _nextInRange(t->left, a, b);
    int min2 = _nextInRange(t->right, a, b);

    if (min1 < min2)
        return min1;
    else
        return min2;
}

// przechodzi po drzewie przedziałowym zawierającym tylko dane od i do końca wektora.
int nextInRange(int i, int a, int b) {
    int a_po_przenumerowaniu = przenumeruj(a);

    int b_po_przenumerowaniu = przenumeruj(b);
    // jeśli górny koniec zakresu nie występuje wśród liczb to porównujemy do ostatniej mniejszej a nie pierwszej większej
    if (b != snd[b_po_przenumerowaniu])
        b_po_przenumerowaniu--;
    
    int wynik = _nextInRange(korzenie[i], a_po_przenumerowaniu, b_po_przenumerowaniu);

    if(wynik >= rozmiar)
        return -1;
    
    return wynik;
}

// zwalnia zarezerwowaną pamięć
void done() {
    while (wskazniki.size() > 0) {
        free(wskazniki.back());
        wskazniki.pop_back();
    }
    return;
}