// Autor: Maciej Curulak
// Peer review: Stanisław Struzik

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>


typedef struct punkt {
    double x;
    double y;
}   punkt;


typedef struct kartka {
    char typ;
    int k;
    punkt p1;
    punkt p2;
    double r;
} kartka;

// sprawdza czy x = y z dokladnoscia do 1e-10
bool equals(double x, double y) {
    return fabs(x - y) < 1e-10;
}

// zwraca po której stronie prostej l (przechodzącej przez l1 i l2) leży punkt p, patrząc z l1 na l2.
// 1 - po lewej, 0 - na, -1 - po prawej
int strona(punkt p, punkt l1, punkt l2) {
    double w = (l2.x - l1.x) * (p.y - l1.y) - (l2.y - l1.y) * (p.x - l1.x); // korzystam ze wzoru na iloczyn wektorowy

    if (equals(w, 0)) return 0;
    if (w > 0) return 1;
    return -1;
}

// zwraca punkt będący odbiciem symetrycznym punktu p względem prostej l (przechodzącej przez l1 i l2).
punkt odbicie(punkt p, punkt l1, punkt l2) {
    punkt spodek, wynik; // spodek - spodek wysokości opuszczonej z wierzchołka p trójkąta (l1 l2 p)

    // a2, b2, c2 - kwadraty długości odcinków p-l1, l1-l2, l2-p
    double a2 = (p.x - l1.x) * (p.x - l1.x) + (p.y - l1.y) * (p.y - l1.y);
    double b2 = (l1.x - l2.x) * (l1.x - l2.x) + (l1.y - l2.y) * (l1.y - l2.y);
    double c2 = (l2.x - p.x) * (l2.x - p.x) + (l2.y - p.y) * (l2.y - p.y);

    // wzór na współrzędne spodka wysokości wyprowadziłem używając twierdzenia cosinusów
    spodek.x = l1.x + (l2.x - l1.x) * (a2 + b2 - c2) / (2 * b2);
    spodek.y = l1.y + (l2.y - l1.y) * (a2 + b2 - c2) / (2 * b2);

    // wynik można uzyskać poprzez przesunięcie punktu spodek o wektor (p -> spodek)
    wynik.x = 2 * spodek.x - p.x;
    wynik.y = 2 * spodek.y - p.y;

    return wynik;
}

// zwraca 1 punkt leży na prostokącie (włącznie z brzegiem), 0 w.p.p.
int czyNaProstokacie(punkt p, kartka a) {
    if ((p.x > a.p1.x || equals(p.x, a.p1.x))
        && (p.x < a.p2.x || equals(p.x, a.p2.x))
        && (p.y > a.p1.y || equals(p.y, a.p1.y))
        && (p.y < a.p2.y || equals(p.y, a.p2.y)))
        return 1;
    return 0;
}

// zwraca 1 punkt leży na kole (włącznie z brzegiem), 0 w.p.p.
int czyNaKole(punkt p, kartka a) {
    double kwadrat_odl = (p.x - a.p1.x) * (p.x - a.p1.x) + (p.y - a.p1.y) * (p.y - a.p1.y);
    if (kwadrat_odl < a.r * a.r || equals(kwadrat_odl, a.r * a.r))
        return 1;
    return 0;
}

// liczy ile dziur powstanie w kartce po wbiciu pinezki w punkt p
int policzDziury(kartka * kartki, kartka badana, punkt p) {
    int str;
    switch (badana.typ) {
      case 'P':
        return czyNaProstokacie(p, badana);
        break;
      case 'K':
        return czyNaKole(p, badana);
        break;
      case 'Z': // w tym przypadku trzeba zbadać gdzie będą dziury po odgięciu tego zagięcia
        str = strona(p, badana.p1, badana.p2);
        if (str == 1)
            return policzDziury(kartki, kartki[badana.k], p) + policzDziury(kartki, kartki[badana.k], odbicie(p, badana.p1, badana.p2));
        if (str == 0)
            return policzDziury(kartki, kartki[badana.k], p);
        return 0;
        break;
      default:
        break;
    }
    return 0;
}


void drukujError() {
    printf("Błąd - niepoprawne dane na wejściu!\n");
    return;
}


int main() {
    int n, q;
    int ile_wczytano;
    ile_wczytano = scanf("%d %d", &n, &q);

    if (ile_wczytano < 2) drukujError();

    kartka * kartki = (kartka*)malloc((size_t)(n + 1) * sizeof(kartka));

    // wczytywanie opisów kartek
    for (int i = 1; i < n + 1; i++) {
        ile_wczytano = scanf("%s", &kartki[i].typ);
        if (ile_wczytano == 0) drukujError();

        switch (kartki[i].typ) {
          case 'P':
            ile_wczytano = scanf("%lf %lf %lf %lf", &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].p2.x, &kartki[i].p2.y);
            if (ile_wczytano < 4) drukujError();
            break;
          case 'K':
            ile_wczytano = scanf("%lf %lf %lf", &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].r);
            if (ile_wczytano < 3) drukujError();
            break;
          case 'Z':
            ile_wczytano = scanf("%d %lf %lf %lf %lf", &kartki[i].k, &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].p2.x, &kartki[i].p2.y);
            if (ile_wczytano < 5) drukujError();
            break;
          default:
            drukujError();
            break;
        }
    }

    // wykonywanie zapytań
    for (int i = 0; i < q; i++) {
        int k; // indeks kartki, którą badamy
        punkt p;

        ile_wczytano = scanf("%d %lf %lf", &k, &p.x, &p.y);
        if (ile_wczytano < 3) drukujError();

        int suma;

        suma = policzDziury(kartki, kartki[k], p);

        printf("%d\n", suma);
    }

    free(kartki);

    return 0;
}