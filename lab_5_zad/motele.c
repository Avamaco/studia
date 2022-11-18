//#define NDEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define NOT_FOUND -1


typedef struct motel{
    int num;    // który z kolei jest to motel
    int siec;   // do jakiej sieci należy
    int poz;    // odległość od początku autostrady
} motel;


void drukuj(motel m){
    printf("Numer: %d  Sieć: %d  Pozycja: %d\n", m.num, m.siec, m.poz);
    return;
}

// nic nie robi. Jest po to by niepotrzebna zmienna nie wywoływała błędu.
void nic(int x) {
    x++;
    return;
}


int min(int a, int b){
    if (a < b) return a;
    return b;
}


int max(int a, int b){
    if (a > b) return a;
    return b;
}

// maksymalna odległość w trójce moteli
int wieksza_odl(motel m1, motel m2, motel m3){
    return max(abs(m2.poz - m1.poz), abs(m3.poz - m2.poz));
}

// Znajduje i zwraca najbliższy motel z innej sieci niż motel "inny".
// t - tablica przeszukiwanych moteli; n - liczba przeszukiwanych moteli; start - skąd zaczynamy szukać; inny - motel którego sieć już mamy.
// Jeśli czy_malejaco = true, numer szukanych moteli maleje. Jeśli = false, numer szukanych moteli rośnie.
// Zwraca {-1, -1, -1} jeśli nie udało się znaleźć.
motel szukajInnego(motel * t, int n, motel start, motel inny, bool czy_malejaco){
    int skok = czy_malejaco ? -1 : 1;

    while (start.siec == inny.siec){
            if ((start.num == n - 1 && !czy_malejaco)||(start.num == 0 && czy_malejaco)){    // nie udało się znaleźć motelu spełniającego wymagania
                motel wynik = {NOT_FOUND, NOT_FOUND, NOT_FOUND};
                return wynik;
            }
            start = t[start.num + skok];
    }
    return start;
}

// Tak jak szukajInnego(), ale mamy już 2 różne sieci. inny2 - drugi motel którego sieć już mamy
motel szukajInnegoOd2(motel * motele, int n, motel start, motel inny, motel inny2, bool czy_malejaco){
    int skok = czy_malejaco ? -1 : 1;

    while (start.siec == inny.siec || start.siec == inny2.siec){
            if ((start.num == n - 1 && !czy_malejaco)||(start.num == 0 && czy_malejaco)){    // nie udało się znaleźć motelu spełniającego wymagania
                motel wynik = {NOT_FOUND, NOT_FOUND, NOT_FOUND};
                return wynik;
            }
            start = motele[start.num + skok];
    }
    return start;
}


int main(){
    int n;
    int no_errors;  // funkcja scanf zwraca wartość której nie potrzebuję, ale trzeba ją wykorzystać żeby nie było błędu

    no_errors = scanf("%d", &n);
    
    motel * motele = malloc((size_t)n * sizeof(motel));

    for (int i = 0; i < n; i++){
        no_errors = scanf("%d%d", &motele[i].siec, &motele[i].poz);
        motele[i].num = i;
    }

    nic(no_errors);

    // znajduję 3 najbliższe początku autostrady motele z różnych sieci
    motel m1, m2, m3;
    m1 = motele[0];

    // niezmiennik: wszystkie motele między m1 a m2 są z tej samej sieci co m1
    m2 = szukajInnego(motele, n, m1, m1, false);
    if (m2.num == NOT_FOUND){  // nie ma 3 roznych sieci
        printf("0 0");
        return 0;
    }

    m1 = motele[m2.num - 1];

    // niezmiennik: wszystkie motele między m2 a m3 są z tej samej sieci co m1 lub m2
    m3 = szukajInnegoOd2(motele, n, m2, m1, m2, false);
    if (m3.num == NOT_FOUND){  // nie ma 3 roznych sieci
        printf("0 0");
        return 0;
    }

    // minimum z maksimów odległości trzech moteli należących do różnych sieci
    int najblizsze = wieksza_odl(m1, m2, m3);

    while (true)
    {
        motel oldm2 = m2;
        m2 = motele[m2.num + 1];

        if (oldm2.siec != m2.siec){
            m1 = oldm2;
        }

        // niezmiennik: wszystkie motele między m2 a m3 są z tej samej sieci co m1 lub m2
        m3 = szukajInnegoOd2(motele, n, m3, m1, m2, false);
        if (m3.num == NOT_FOUND) break;    // skończyły się trójki
        
        int maxodl = wieksza_odl(m1, m2, m3);

        najblizsze = min(najblizsze, maxodl);
    }

    printf("%d ", najblizsze);

    motel lewe[3];
    lewe[0] = motele[0];
    lewe[1] = szukajInnego(motele, n, lewe[0], lewe[0], false);
    lewe[2] = szukajInnegoOd2(motele, n, lewe[1], lewe[0], lewe[1], false);
    assert(lewe[1].num != NOT_FOUND && lewe[2].num != NOT_FOUND);    // już wiemy, że istnieją przynajmniej 3 sieci

    motel prawe[3];
    prawe[0] = motele[n - 1];
    prawe[1] = szukajInnego(motele, n, prawe[0], prawe[0], true);
    prawe[2] = szukajInnegoOd2(motele, n, prawe[1], prawe[0], prawe[1], true);
    assert(prawe[1].num != NOT_FOUND && prawe[2].num != NOT_FOUND);    // już wiemy, że istnieją przynajmniej 3 sieci

    // maksimum z minimów odległości trzech moteli należących do różnych sieci
    int najdalsze = 0;

    for (int i = 0; i < n; i++){
        m2 = motele[i];

        if (lewe[0].siec != m2.siec) m1 = lewe[0];
        else m1 = lewe[1];
        
        if (prawe[0].siec != m1.siec && prawe[0].siec != m2.siec) m3 = prawe[0];
        else if (prawe[1].siec != m1.siec && prawe[1].siec != m2.siec) m3 = prawe[1];
        else m3 = prawe[2];

        int minodl = min(m3.poz - m2.poz, m2.poz - m1.poz); // może wyjść < 0 co jest błędne, ale zostanie to zignorowane bo jest mniejsze od "najdalsze"
        najdalsze = max(najdalsze, minodl);

        if (prawe[0].siec != m2.siec) m3 = prawe[0];
        else m3 = prawe[1];
        
        if (lewe[0].siec != m3.siec && lewe[0].siec != m2.siec) m1 = lewe[0];
        else if (lewe[1].siec != m3.siec && lewe[1].siec != m2.siec) m1 = lewe[1];
        else m1 = lewe[2];

        minodl = min(m3.poz - m2.poz, m2.poz - m1.poz);
        najdalsze = max(najdalsze, minodl);
    }

    printf("%d", najdalsze);
    
    free(motele);

    return 0;
}