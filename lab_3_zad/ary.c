// Autor: Maciej Curulak
// Peer review: Mateusz Kołpa

#include <stdio.h>
#include <math.h>

#include "stdbool.h"

#include "ary.h"


// zamienia miejscami w.dol i w.gora tak, aby zawsze w.dol <= w.gora
wartosc popraw_dol_i_gore(wartosc w){
    if (w.dol > w.gora){
        double temp = w.dol;
        w.dol = w.gora;
        w.gora = temp;
    }
    return w;
}

// może się zdarzyć że powstanie antyprzedział z nan. Wtedy wolę mieć zamiast tego zwykły przedział z nan.
wartosc popraw_nan(wartosc w){
    if (isnan(w.dol) || isnan(w.gora))
        return wartosc_od_do(NAN, NAN);
    return w;
}

// sprawdza czy x = 0 z dokladnoscia do 1e-10
bool iszero(double x) { return fabs(x) < 1e-10; }


wartosc wartosc_dokladnosc(double x, double p){
    wartosc wynik;

    wynik.dol = x * ((100 - p) / 100);
    wynik.gora = x * ((100 + p) / 100);
    wynik.czy_anty = false;

    wynik = popraw_dol_i_gore(wynik);

    return wynik;
}


wartosc wartosc_od_do(double x, double y){
    wartosc wynik;

    wynik.dol = x;
    wynik.gora = y;
    wynik.czy_anty = false;

    wynik = popraw_dol_i_gore(wynik);

    return wynik;
}


wartosc wartosc_dokladna(double x){
    return wartosc_od_do(x, x);
}


bool in_wartosc(wartosc w, double x){
    if (w.czy_anty){
        if (x > w.dol && x < w.gora)
            return false;
        else
            return true;
    }
    else{
        if (x >= w.dol && x <= w.gora)
            return true;
        else
            return false;
    }
}


double min_wartosc(wartosc w){
    if (w.czy_anty)
        return -HUGE_VAL;
    else
        return w.dol;
}


double max_wartosc(wartosc w){
    if (w.czy_anty)
        return HUGE_VAL;
    else
        return w.gora;
}


double sr_wartosc(wartosc w){
    if (w.czy_anty || (w.dol <= -HUGE_VAL && w.gora >= HUGE_VAL))
        return NAN;
    else
        return (w.dol + w.gora)/2.0;
}


wartosc plus(wartosc a, wartosc b){
    wartosc wynik;

    if (!a.czy_anty && !b.czy_anty)
        wynik = wartosc_od_do(a.dol + b.dol, a.gora + b.gora);
    else if (a.czy_anty && b.czy_anty)
        wynik = wartosc_od_do(-HUGE_VAL, HUGE_VAL);
    else{
        if (b.czy_anty){    //żeby nie rozbijać na 2 przypadki, chcę żeby 'a' było antyprzedziałem a 'b' było przedziałem.
            wartosc temp = a;
            a = b;
            b = temp;
        }

        wynik.dol = a.dol + b.gora;
        wynik.gora = a.gora + b.dol;
        wynik.czy_anty = true;

        if (wynik.dol >= wynik.gora)
            return wartosc_od_do(-HUGE_VAL, HUGE_VAL);
    }

    wynik = popraw_nan(wynik);

    return wynik;
}

// zwraca wartość przeciwną do w, tzn. -w
wartosc przeciwna(wartosc w){
    wartosc wynik;

    wynik.dol = -w.gora;
    wynik.gora = -w.dol;
    wynik.czy_anty = w.czy_anty;

    return wynik;
}

// odejmowanie to inaczej dodawanie wartości przeciwnej
wartosc minus(wartosc a, wartosc b){
    return plus(a, przeciwna(b));
}


double min(double x, double y){
    if (x < y) return x;
    return y;
}


double max(double x, double y){
    if (x > y) return x;
    return y;
}

// zwraca wartość odwrotną do w, tzn. 1/w
wartosc odwrotna(wartosc w){
    wartosc wynik;

    if (iszero(w.dol)){
        if (iszero(w.gora))
            wynik = wartosc_od_do(NAN, NAN);
        else if (w.czy_anty)
            wynik = wartosc_od_do(-HUGE_VAL, 1.0/w.gora);
        else
            wynik = wartosc_od_do(1.0/w.gora, HUGE_VAL);
    }
    else if (iszero(w.gora)){
        if (w.czy_anty)
            wynik = wartosc_od_do(1.0/w.dol, HUGE_VAL);
        else
            wynik = wartosc_od_do(-HUGE_VAL, 1.0/w.dol);
    }
    else{
        wynik.dol = 1.0/w.gora;
        wynik.gora = 1.0/w.dol;
        if (w.dol * w.gora > 0)
            wynik.czy_anty = w.czy_anty;
        else
            wynik.czy_anty = !w.czy_anty;

        // może się zdarzyć że dwa końce antyprzedziału się skleją, tworząc zbiór wszystkich liczb rzeczywistych
        if (wynik.czy_anty && iszero(wynik.dol - wynik.gora))
            wynik = wartosc_od_do(-HUGE_VAL, HUGE_VAL);
    }

    wynik = popraw_nan(wynik);
    wynik = popraw_dol_i_gore(wynik);

    return wynik;
}

// poszerza krańce przedziału tak, aby zwierał on d. Używam tylko w funkcji razy_zwykle
wartosc poszerz_o_double(wartosc w, double d){
    if (!isnan(d)){
        w.dol = min(w.dol, d);
        w.gora = max(w.gora, d);
    }
    return w;
}

// zwraca x * y, ale jeśli jeden czynnik = 0, to zwróci 0 nawet jeśli drugi jest +-HUGE_VAL. Zwraca NAN tylko jeśli x lub y to NAN.
double iloczyn_bezpieczny(double x, double y){
    if (isnan(x) || isnan(y))
        return NAN;
    if (iszero(x) || iszero(y))
        return 0.0;
    return x * y;
}

// zwraca iloczyn dwóch zwykłych przedziałów (czyli nie antyprzedziałów)
wartosc razy_zwykle(wartosc a, wartosc b){
    wartosc wynik;

    //wynik.dol = min(min(min(a.dol * b.dol, a.gora * b.gora), a.gora * b.dol), a.dol * b.gora);    // niepoprawne bo nan wszystko psuje
    //wynik.gora = max(max(max(a.dol * b.dol, a.gora * b.gora), a.gora * b.dol), a.dol * b.gora);   // kod poniżej robi praktycznie to samo ale sprawdza też nan

    wynik.dol = HUGE_VAL;
    wynik.gora = -HUGE_VAL;
    wynik.czy_anty = false;

    wynik = poszerz_o_double(wynik, iloczyn_bezpieczny(a.dol, b.dol));
    wynik = poszerz_o_double(wynik, iloczyn_bezpieczny(a.dol, b.gora));
    wynik = poszerz_o_double(wynik, iloczyn_bezpieczny(a.gora, b.dol));
    wynik = poszerz_o_double(wynik, iloczyn_bezpieczny(a.gora, b.gora));

    // może się zdarzyć gdy wszystkie cztery iloczyny = nan
    if (wynik.dol > wynik.gora)
        wynik = wartosc_od_do(NAN, NAN);

    return wynik;
}

// zwraca przedział c = a U b. Używam tej funkcji tylko dla antyprzedziałów i przedziałów typu [-HUGE_VAL, x] lub [x, HUGE_VAL]
wartosc suma_zbiorow(wartosc a, wartosc b){
    wartosc wynik;

    if (a.czy_anty && b.czy_anty){
        wynik.dol = max(a.dol, b.dol);
        wynik.gora = min(a.gora, b.gora);
        wynik.czy_anty = true;
    }
    else if (a.czy_anty){
        wynik = a;
        if(b.gora >= HUGE_VAL)
            wynik.gora = min(b.dol, a.gora);
        else
            wynik.dol = max(b.gora, a.dol);
    }
    else if (b.czy_anty){
        wynik = b;
        if(a.gora >= HUGE_VAL)
            wynik.gora = min(a.dol, b.gora);
        else
            wynik.dol = max(a.gora, b.dol);
    }
    else{
        if((a.gora >= HUGE_VAL && b.gora >= HUGE_VAL) || (a.dol <= -HUGE_VAL && b.dol <= -HUGE_VAL))
            wynik = wartosc_od_do(min(a.dol, b.dol), max(a.gora, b.gora));
        else {
            wynik.dol = min(a.gora, b.gora);
            wynik.gora = max(a.dol, b.dol);
            wynik.czy_anty = true;
        }
    }

    // może się zdarzyć że dwie części antyprzedziału się pokryją, tworząc zbiór wszystkich liczb rzeczywistych
    if (wynik.dol >= wynik.gora)
        wynik = wartosc_od_do(-HUGE_VAL, HUGE_VAL);

    wynik = popraw_nan(wynik);

    return wynik;
}


wartosc razy(wartosc a, wartosc b){
    wartosc wynik;

    if (!a.czy_anty && !b.czy_anty){
        wynik = razy_zwykle(a, b);
    }
    else if (a.czy_anty && b.czy_anty){
        // rozbija antyprzedziały a i b na po dwa podprzedziały
        wartosc przedzial1a = wartosc_od_do(-HUGE_VAL, a.dol);
        wartosc przedzial2a = wartosc_od_do(a.gora, HUGE_VAL);

        wartosc przedzial1b = wartosc_od_do(-HUGE_VAL, b.dol);
        wartosc przedzial2b = wartosc_od_do(b.gora, HUGE_VAL);

        // wykonuje mnożenia na 4 możliwych parach podprzedziałów a i b, następnie łączy je w jeden przedział
        wartosc razy1 = razy_zwykle(przedzial1a, przedzial1b);
        wartosc razy2 = razy_zwykle(przedzial1a, przedzial2b);
        wartosc razy3 = razy_zwykle(przedzial2a, przedzial1b);
        wartosc razy4 = razy_zwykle(przedzial2a, przedzial2b);

        wynik = suma_zbiorow(suma_zbiorow(suma_zbiorow(razy1, razy2), razy3), razy4);
    }
    else{
        if (b.czy_anty){    //żeby nie rozbijać na 2 przypadki, chcę żeby 'a' było antyprzedziałem a 'b' było przedziałem.
            wartosc temp = a;
            a = b;
            b = temp;
        }
        // rozbija antyprzedział na dwa podprzedziały
        wartosc przedzial1a = wartosc_od_do(-HUGE_VAL, a.dol);
        wartosc przedzial2a = wartosc_od_do(a.gora, HUGE_VAL);

        // wykonuje mnożenia na 2 możliwych parach podprzedziałów a, następnie łączy je w jeden przedział
        wartosc razy1 = razy_zwykle(przedzial1a, b);
        wartosc razy2 = razy_zwykle(przedzial2a, b);

        wynik = suma_zbiorow(razy1, razy2);
    }

    return wynik;
}

// dzielenie to inaczej mnożenie przez liczbę odwrotną
wartosc podzielic(wartosc a, wartosc b){
    return razy(a, odwrotna(b));
}