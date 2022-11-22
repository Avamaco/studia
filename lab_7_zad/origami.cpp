#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <stack>

using namespace std;


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
    double kwadrat_odl = (p.x - a.p1.x) * (p.x - a.p1.x) + (p.y - a.p1.y) + (p.y - a.p1.y);
    if (kwadrat_odl < a.r * a.r || equals(kwadrat_odl, a.r * a.r))
        return 1;
    return 0;
}


int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    kartka * kartki = (kartka*)malloc((size_t)(n + 1) * sizeof(kartka));

    // wczytywanie opisów kartek
    for (int i = 1; i < n + 1; i++) {
        scanf("%s", &kartki[i].typ);
        switch (kartki[i].typ) {
          case 'P':
            scanf("%lf %lf %lf %lf", &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].p2.x, &kartki[i].p2.y);
            break;
          case 'K':
            scanf("%lf %lf %lf", &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].r);
            break;
          case 'Z':
            scanf("%d %lf %lf %lf %lf", &kartki[i].k, &kartki[i].p1.x, &kartki[i].p1.y, &kartki[i].p2.x, &kartki[i].p2.y);
            break;
          default:
            break;
        }
    }

    // wykonywanie zapytań
    for (int i = 0; i < q; i++) {
        stack <punkt> dziury;
        int k;
        punkt p;

        scanf("%d %lf %lf", &k, &p.x, &p.y);
        dziury.push(p);

        while (kartki[k].typ == 'Z') {
            kartka badana = kartki[k];
            stack <punkt> temp;

            while (!dziury.empty()) {
                p = dziury.top();
                dziury.pop();

                int side = strona(p, badana.p1, badana.p2);
                switch (side) {
                  case 1: // jeśli dziura jest po lewej stronie zgięcia to po odgieciu będzie po obu stronach zgięcia
                    temp.push(p);
                    temp.push(odbicie(p, badana.p1, badana.p2));
                    break;
                  case 0: // jeśli dziura jest na zgięciu to na nim pozostanie
                    temp.push(p);
                    break;
                  default: // jeśli dziura jest po prawej stronie zgięcia to w ogóle nie trafiła w kartkę
                    break;
                }
            }

            dziury = temp;
            k = badana.k;
        }

        int suma = 0;

        while (!dziury.empty()) {
            p = dziury.top();
            dziury.pop();

            if (kartki[k].typ == 'P')
                suma += czyNaProstokacie(p, kartki[k]);
            else
                suma += czyNaKole(p, kartki[k]);
        }

        printf("%d\n", suma);
    }

    free(kartki);

    return 0;
}