// Autor: Mateusz Kołpa
// Peer review: Maciej Curulak

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ary.h"

#define EMPTY (wartosc) {.liczba_przedzialow = 0}
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void _swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}

void _swap_wartosci(wartosc* a, wartosc* b){
    wartosc temp = *a;
    *a = *b;
    *b = temp;
}

typedef struct Rozbicie {
    wartosc a, b;
} Rozbicie;

/*
    rozbija wartosc, np. w = (-HUGE_VAL, -1] U [1, HUGE_VAL) (= 1/[-1, 1]) na:
    - wartosc a = (-HUGE_VAL, -1]
    - wartosc b = [1, HUGE_VAL)
*/
Rozbicie _rozbij(wartosc w){
    wartosc a;
    a.liczba_przedzialow = 1;
    a.przedzialy[0] = w.przedzialy[0];

    wartosc b;
    b.liczba_przedzialow = 1;
    b.przedzialy[0] = w.przedzialy[1];

    return (Rozbicie){.a = a, .b = b};
}

bool _iszero(double x) {
    return fabs(x) < 1e-10;
}

/*
    Funkcja pomocnicza do print.
    Usuwa zera po kropce (przecinku):
        2.000 -> 2
        1.1000 -> 1.1
*/
void _trim(char s[350]) {
    int len = strlen(s);
    bool found_dot = false;
    if(!('0' <= s[len - 1] && s[len - 1] <= '9'))
        return;
    while(!found_dot && len >= 0){
        if(s[len - 1] == '0' || s[len - 1] == '.'){
            if(s[len - 1] == '.')
                found_dot = true;
            s[len - 1] = '\0';
        } else {
            break;
        }
        len--;
    }
}

/*
    Wypisuje wartosc w przyjaznej formie na wyjscie
*/
void print(wartosc w){
    char p1_pocz[350], p1_kon[350], p2_pocz[350], p2_kon[350];
    sprintf(p1_pocz, "%.10f", w.przedzialy[0].poczatek);
    sprintf(p1_kon, "%.10f", w.przedzialy[0].koniec);
    sprintf(p2_pocz, "%.10f", w.przedzialy[1].poczatek);
    sprintf(p2_kon, "%.10f", w.przedzialy[1].koniec);
    _trim(p1_pocz);
    _trim(p1_kon);
    _trim(p2_pocz);
    _trim(p2_kon);

    if(w.liczba_przedzialow == 0)
        printf("NaN\n");
    else if(w.liczba_przedzialow == 1){
        if(_iszero(w.przedzialy[0].poczatek - w.przedzialy[0].koniec))
            printf("= %s\n", p1_pocz);
        else
            printf("= [%s, %s]\n", p1_pocz, p1_kon);
    }
    else
        printf("= [%s, %s] U [%s, %s]\n", p1_pocz, p1_kon, p2_pocz, p2_kon);

}

/*
    Dodaje do liczby nowy przedzial.
    W przypadku, gdy: koniec < poczatek <-> swap(koniec, poczatek)

    Jest to suma przedziałów ((a,b) U (c,d))
*/
void _dodaj_przedzial_double(wartosc* w, double poczatek, double koniec){
    if(poczatek > koniec)
        _swap(&poczatek, &koniec);

    przedzial nowy = {.poczatek = poczatek, .koniec = koniec};
    if(w->liczba_przedzialow == 0){
        w->liczba_przedzialow++;
        w->przedzialy[0] = nowy;
    } else if(w->liczba_przedzialow == 1){
        /*
            Bloki reprezentują przedzialy na osi liczbowej.
            Stary przedzial u góry, nowy na dole.
        */
        if(w->przedzialy[0].poczatek <= koniec && koniec <= w->przedzialy[0].koniec){
            /*
                   [--------]
                +
                      -]
            */
            w->przedzialy[0].poczatek = MIN(w->przedzialy[0].poczatek, poczatek);
        }
        if(w->przedzialy[0].poczatek <= poczatek && poczatek <= w->przedzialy[0].koniec){
            /*
                [-------]
                +
                   [-
            */
            w->przedzialy[0].koniec = MAX(w->przedzialy[0].koniec, koniec);
        }
        if(w->przedzialy[0].koniec < poczatek){
            /*
                [----]
                +
                        [-----]
            */
            w->liczba_przedzialow++;
            w->przedzialy[1] = nowy;
        }
        if(koniec < w->przedzialy[0].poczatek) {
            /*
                        [-----]
                +
                [----]
            */
            w->liczba_przedzialow++;
            w->przedzialy[1] = w->przedzialy[0];
            w->przedzialy[0] = nowy;
        }
    } else {
        // W tym przypadku probujemy dodać do liczby, która ma już 2 przedziały
        // kolejny przedzial. Moze sie tak wydarzyc i jest to edge case.
        // Wtedy nowy przedzial na pewno pokrywa sie z
        // ktoryms z istniejacych (patrz funkcja _wartosc_odwrotna)
        Rozbicie roz = _rozbij(*w);
        _dodaj_przedzial_double(&roz.a, poczatek, koniec);
        _dodaj_przedzial_double(&roz.b, poczatek, koniec);

        /*
            ktorys z przedzialow z robicia dalej ma 1 przedzial, co oznacza,
            ze udalo sie je polaczyc.
        */
        if(roz.a.liczba_przedzialow == 1 && roz.b.liczba_przedzialow == 1){
            /*
                Byc moze nastapil edge case:
                [-----]     [-----]
                +
                    [----------]

            */
            w->liczba_przedzialow = 1;
            w->przedzialy[0].poczatek = MIN(roz.a.przedzialy[0].poczatek, roz.b.przedzialy[0].poczatek);
            w->przedzialy[0].koniec = MAX(roz.a.przedzialy[0].koniec, roz.b.przedzialy[0].koniec);
        } else if(roz.a.liczba_przedzialow == 1){
            /*
                oznacza to, ze pierwszy przedzial sie polaczyl
                [-------]        [-----]
                +
                    [------]

            */

            w->przedzialy[0] = roz.a.przedzialy[0];
        } else {
            // a to, ze drugi
            w->przedzialy[1] = roz.b.przedzialy[0];
        }
    }
}

void _dodaj_przedzialy(wartosc* w, wartosc nowa){
    for(int i = 0; i < nowa.liczba_przedzialow; ++i)
        _dodaj_przedzial_double(w, nowa.przedzialy[i].poczatek, nowa.przedzialy[i].koniec);
}

/*
    Funkcja pomocnicza, służy do wykonywania operacji na liczbach,
    z ktorych co najmniej jedna sklada sie z dwoch przedzialow.
    Rozbija ona liczbe na przedzialy i wykonuje na nich operacje.
*/
void _wykonaj_operacje(wartosc* result, wartosc a, wartosc b, wartosc (*operacja)(wartosc, wartosc)) {
    if(a.liczba_przedzialow == 1 && b.liczba_przedzialow == 2)
        _swap_wartosci(&a, &b);

    Rozbicie rozbite = _rozbij(a);
    _dodaj_przedzialy(result, (*operacja)(rozbite.a, b));
    _dodaj_przedzialy(result, (*operacja)(rozbite.b, b));
}

// zwraca 1/w
wartosc _wartosc_odwrotna(wartosc w) {
    wartosc result = EMPTY;

    if(w.liczba_przedzialow == 2) {
        Rozbicie roz = _rozbij(w);
        _dodaj_przedzialy(&result, _wartosc_odwrotna(roz.a));
        _dodaj_przedzialy(&result, _wartosc_odwrotna(roz.b));
        // jest mozliwosc, ze po rozbiciu dalej bedzie kilka przedzialow zwroconych,
        // np. a = 1/[-1/4, 1] = (−HUGE, −4] U [1, +HUGE)
        // np. b = [1, 2]
        // (a - b) = (−HUGE, −5] U [−1, +HUGE)
        // Gdy robimy 1 / (a-b), to trzeba robić (−HUGE, −5] U [−1, +HUGE)
        // i wynikiem 1/[−1, +HUGE) są 2 przedziały (-HUGE, -1] U [0, HUGE)

    } else if (w.liczba_przedzialow == 1) {
        double minv = min_wartosc(w);
        double maxv = max_wartosc(w);
        bool zero_in_range = in_wartosc(w, 0);

        if(isinf(-minv) && isinf(maxv))
            return wartosc_od_do(-HUGE_VAL, HUGE_VAL);

        if(zero_in_range) {
            // dzielimy przez wartosci bliskie zeru
            if(_iszero(minv) && _iszero(maxv))
                return EMPTY;

            if(!_iszero(maxv))
                _dodaj_przedzial_double(&result, 1/maxv, HUGE_VAL); // np. 1/[0, a] = [1/a, HUGE_VAL)

            if(!_iszero(minv))
                _dodaj_przedzial_double(&result, -HUGE_VAL, 1/minv); // np. 1/[-5, 0] = (-HUGE_VAL, -1/5]


        } else {
            _dodaj_przedzial_double(&result, 1/maxv, 1/minv); // np. 1 / [0.5, 11], lub 1/[-5, -4]
        }
    }
    return result;
}

wartosc _wartosc_przeciwna(wartosc w) {
    wartosc result = EMPTY;
    if(w.liczba_przedzialow == 0)
        return result;

    if(w.liczba_przedzialow == 1)
        _dodaj_przedzial_double(&result, -w.przedzialy[0].koniec, -w.przedzialy[0].poczatek);

    if(w.liczba_przedzialow == 2){
        Rozbicie rozbite = _rozbij(w);
        _dodaj_przedzialy(&result, _wartosc_przeciwna(rozbite.a));
        _dodaj_przedzialy(&result, _wartosc_przeciwna(rozbite.b));
    }
    return result;
}

wartosc wartosc_od_do(double x, double y) {
    wartosc result = EMPTY;
    _dodaj_przedzial_double(&result, x, y);
    return result;
}

wartosc wartosc_dokladnosc(double x, double p) {
    p /= 100;
    return wartosc_od_do(x * (1 - p), x * (1 + p));
}


wartosc wartosc_dokladna(double x) {
    return wartosc_od_do(x, x);
}

bool in_wartosc(wartosc w, double x) {
    for(int i = 0; i < w.liczba_przedzialow; ++i)
        if(w.przedzialy[i].poczatek <= x && x <= w.przedzialy[i].koniec)
            return true;

    return false;
}

double min_wartosc(wartosc w) {
    if(w.liczba_przedzialow == 0)
        return NAN;
    return w.przedzialy[0].poczatek;
}

double max_wartosc(wartosc w) {
    if(w.liczba_przedzialow == 0)
        return NAN;
    return w.przedzialy[w.liczba_przedzialow - 1].koniec;
}

double sr_wartosc(wartosc w) {
    double minv = min_wartosc(w);
    double maxv = max_wartosc(w);
    if(isinf(-minv) && isinf(maxv))
        return NAN;
    return (minv + maxv) / 2;
}

wartosc plus(wartosc a, wartosc b){
    wartosc result = EMPTY;

    if(a.liczba_przedzialow == 0 || b.liczba_przedzialow == 0)
        return EMPTY;
    if(a.liczba_przedzialow == 1 && b.liczba_przedzialow == 1)
        _dodaj_przedzial_double(&result, min_wartosc(a) + min_wartosc(b), max_wartosc(a) + max_wartosc(b));
    else
        _wykonaj_operacje(&result, a, b, &plus);

    return result;
}

wartosc minus(wartosc a, wartosc b) {
    return plus(a, _wartosc_przeciwna(b));
}

wartosc razy(wartosc a, wartosc b){
    wartosc result = EMPTY;
    if(a.liczba_przedzialow == 0 || b.liczba_przedzialow == 0)
        return result;
    if(a.liczba_przedzialow == 1 && b.liczba_przedzialow == 1){
        double minv_a = min_wartosc(a);
        double minv_b = min_wartosc(b);
        double maxv_a = max_wartosc(a);
        double maxv_b = max_wartosc(b);
        // najmniejsza z tego to będzie minimum
        // a największa to maksimum
        // np. [-10, 10] * [-1, 15] = [-150, 150]
        // i -150 = minv_a * maxv_b, ale to nie jest zasada
        // jesli inf * 0, to będzie gdzieś NaN
        double possible_vals[] = {
            minv_a * minv_b,
            minv_a * maxv_b,

            maxv_a * minv_b,
            maxv_a * maxv_b,
        };

        double new_minv = HUGE_VAL, new_maxv = -HUGE_VAL;
        bool initialized_values = false;
        for(int i = 0; i < 4; ++i){
            if(isnan(possible_vals[i]))
                continue;

            if(!initialized_values) {
                new_minv = possible_vals[i];
                new_maxv = possible_vals[i];
                initialized_values = true;
            }
            else {
                new_minv = MIN(new_minv, possible_vals[i]);
                new_maxv = MAX(new_maxv, possible_vals[i]);
            }

        }
        if(initialized_values == false)
            return EMPTY;

        _dodaj_przedzial_double(&result, new_minv, new_maxv);
    } else {
        _wykonaj_operacje(&result, a, b, &razy);
    }
    return result;
}

wartosc podzielic(wartosc a, wartosc b){
    return razy(a, _wartosc_odwrotna(b));
}
