#include <iostream>
#include <vector>
#include <string>
using namespace std;

__extension__ typedef __int128 int128;

struct shape
{
    // reprezentacja ksztaltu i jego obrotow w formacie jak na wejsciu
    vector<string> uklad[4];

    // ilosc pol ksztaltu
    int roz;

    // szerokosc i wysokosc ksztaltu
    int x[4];
    int y[4];

    // ksztalt jako bity
    vector<int> maski[4];

    // ksztalt jako bity rozlozony w jeden wiersz (liczbe)
    int128 maska[4];
};

// kombinacje puzzli o dobresj sumie
vector<bool> poss_comb;

vector<shape> shapes;

// obrot ksztaltu o 90 stopni
vector<string> rotate_shape(vector<string> old)
{
    vector<string> tab(old[0].size());
    for(int i = 0;i<(int)tab.size();++i){
        tab[i].resize(old.size());
    }
    for (int i = 0; i < (int)old[0].size(); ++i)
    {
        for (int j = 0; j < (int)old.size(); ++j)
        {
            tab[old[0].size() - 1 - i][j] = old[j][i];
        }
    }
    return tab;
}
// usuniecie pustych rzedow i kolumn
void cut(vector<string> &tab)
{
    int min_x = (int)tab.size();
    int min_y = (int)tab[0].size();
    int max_x = -1, max_y = -1;
    for (int i = 0; i < (int)tab.size(); ++i)
    {
        for (int j = 0; j < (int)tab[0].size(); ++j)
        {
            if (tab[i][j] != '.')
            {
                min_x = min(min_x, i);
                max_x = max(max_x, i);
                min_y = min(min_y, j);
                max_y = max(max_y, j);
            }
        }
    }
    vector<string> odp(max_x - min_x + 1);
    for (int i = 0; i <= max_x - min_x; ++i)
    {
        odp[i].resize(max_y-min_y+1);
        for (int j = 0; j <= max_y - min_y; ++j)
        {
            odp[i][j] = tab[min_x + i][min_y + j];
        }
    }
    tab.clear();
    tab = odp;
}
// ilosc pol w danym ksztalcie
int count_blocks(shape s, int n, int m)
{
    int odp = 0;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (s.uklad[0][i][j] != '.')
            {
                ++odp;
            }
        }
    }
    return odp;
}
void create_masks(shape &s, int m)
{
    for (int r = 0; r < 4; ++r)
    {
        s.maski[r].resize(s.x[r]);
        for (int i = 0; i < s.x[r]; ++i)
        {
            for (int j = 0; j < s.y[r]; ++j)
            {
                if (s.uklad[r][i][j] != '.')
                {
                    s.maski[r][i] += (1 << j);
                }
            }
        }
        s.maska[r] = 0;
        for (int i = s.x[r] - 1; i >= 0; --i)
        {
            s.maska[r] = (s.maska[r] << m) + s.maski[r][i];
        }
    }
}

// stworzenie ksztaltu i jego rotacji
// i reprezentacja ich jako maski bitowe
void create_shape(shape &s, int n, int m)
{
    // ilosc pol
    s.roz = count_blocks(s, n, m);
    if (s.roz==0) return;
    // obrot
    for (int r = 1; r < 4; ++r)
    {
        s.uklad[r] = rotate_shape(s.uklad[r - 1]);
        
    }

    // przyciecie
    for (int r = 0; r < 4; ++r)
    {
        cut(s.uklad[r]);
        s.x[r] = (int)s.uklad[r].size();
        s.y[r] = (int)s.uklad[r][0].size();
    }

    // usuniecie zlych ksztaltow
    for (int r = 1; r < 4; ++r)
    {
        if (s.x[r] > n || s.y[r] > m)
        {
            s.uklad[r].clear();
            s.x[r] = 0;
            s.y[r] = 0;
        }
    }

    create_masks(s, m);
}
// proba dopasowania danej kombinacji do planszy
bool fit(int n, int m, int id, int comb,
         int128 maska, vector<string> &wynik)
{
    // zla kombinacja
    if (!poss_comb[comb])
    {
        return false;
    }
    // koniec dostepnych ksztaltow
    if (id == (int)shapes.size())
    {
        return maska == (((int128)1 << (n * m)) - 1);
    }
    if (fit(n, m, id + 1, comb, maska, wynik))
    {
        return true;
    }
    for (int r = 0; r < 4; ++r)
    {
        if (shapes[id].x[r])
        {
            // dx,dy - przesuniecia ksztaltu na planszy
            for (int dx = 0; dx <= n - shapes[id].x[r]; ++dx)
            {
                for (int dy = 0; dy <= m - shapes[id].y[r]; ++dy)
                {
                    int128 shift = (shapes[id].maska[r] << (m * dx + dy));
                    if ((maska & shift) == 0) // brak kolizji
                    {
                        if (fit(n, m, id + 1, comb | (1 << id), maska | shift, wynik))
                        {
                            // znalezione dobre ustawienie puzzli
                            // wstawienie ksztaltu do wyniku
                            for (int i = 0; i < shapes[id].x[r]; ++i)
                            {
                                for (int j = 0; j < shapes[id].y[r]; ++j)
                                {
                                    if (shapes[id].uklad[r][i][j] != '.')
                                    {
                                        wynik[dx + i][dy + j] = shapes[id].uklad[r][i][j];
                                    }
                                }
                            }
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
// znalezienie kombinacji puzzli o dobrej sumie pol
void find_poss_comb(int k, int n, int m)
{
    poss_comb.resize(1 << k);
    for (int mask = 0; mask < (1 << k); ++mask)
    {
        int suma = 0;
        for (int i = 0; i < k; ++i)
        {
            if (mask & (1 << i))
            {
                suma += shapes[i].roz;
            }
        }
        if (suma == n * m)
        {
            for (int i = 0; i <= k; ++i)
            {
                poss_comb[mask & ((1 << i) - 1)] = true;
            }
        }
    }
}
int main()
{
    int n, m, k;
    cin >> n >> m >> k;
    shapes.resize(k);
    for (int i = 0; i < k; ++i)
    {
        shapes[i].uklad[0].resize(n);
        for (int j = 0; j < n; ++j)
        {
            cin >> shapes[i].uklad[0][j];
        }
        create_shape(shapes[i], n, m);
    }

    find_poss_comb(k, n, m);
    int128 maska = 0;
    vector<string> wynik(n);
    for (int i = 0; i < n; ++i)
    {
        wynik[i].append(m, '.');
    }

    if (fit(n, m, 0, 0, maska, wynik))
    {
        cout << "TAK" << endl;
        for (int i = 0; i < n; ++i)
        {
            cout << wynik[i] << endl;
        }
        return 0;
    }

    cout << "NIE" << endl;
    return 0;
}
