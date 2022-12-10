#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <assert.h>

typedef struct {
    double x, y;
}Punkt;

typedef struct {
    Punkt LD, PG;
}Prostokat;

typedef struct {
    Punkt O;
    double r;
}Kolo;

typedef struct {
    int id_ksztaltu;
    Punkt P1, P2;
}Zgiecie;

typedef enum{Pr='P', Ko='K', Zg='Z'} Repr;
typedef union {Prostokat p; Kolo k; Zgiecie z;} Val;

typedef struct {
    Repr repr;
    Val val;
}Ksztalt;

int Czy_w_prostokacie(Punkt X, Prostokat P){
    Punkt LD = P.LD, PG = P.PG;
    return (LD.x <= X.x && X.x <= PG.x && LD.y <= X.y && X.y <= PG.y) ? 1 : 0;
}
int Czy_w_kole(Punkt X, Kolo K){
    Punkt O = K.O;
    double r = K.r;
    return ((X.x - O.x) * (X.x - O.x) + (X.y - O.y) * (X.y - O.y) <= r * r)? 1 : 0;
}

void Przesun_o_P(Punkt *X, Punkt P){
    X->x -= P.x;
    X->y -= P.y;
}

int prawo_lewo(Punkt X, Punkt P1, Punkt P2){
    Przesun_o_P(&P2, P1);
    Przesun_o_P(&X, P1);
    double iloczyn_wektorowy = X.x * P2.y - X.y * P2.x;
    if(iloczyn_wektorowy > 0){
        return 1;
    }
    if(iloczyn_wektorowy < 0){
        return -1;
    }
    return 0;
}

int Ile_w_punkcie(Punkt X, Ksztalt ksztalt, Ksztalt tab[]);

Punkt Odbij(Punkt X, Punkt P1, Punkt P2){
    Przesun_o_P(&P2, P1); //
    Przesun_o_P(&X, P1); // przesuniecie ukladu wspolzednych na srodek
    double complex CP = P2.x + P2.y * I, CX = X.x + X.y * I;
    CX = conj(CX * conj(CP) / cabs(CP)) * cabs(CP) / conj(CP); //odbicie punktu X wzgledem vektora P2
    X.x = creal(CX);
    X.y = cimag(CX);
    P1.x = -P1.x;
    P1.y = -P1.y;
    Przesun_o_P(&X, P1); //przesuniecie ukladu wpsolzednych z powrotem tam bdzie byl
    return X;
}

int Ile_w_zgieciu(Punkt X, Zgiecie Z, Ksztalt tab[]){
    switch (prawo_lewo(X, Z.P1, Z.P2)) {
        case -1:
            return Ile_w_punkcie(X, tab[Z.id_ksztaltu], tab)
                + Ile_w_punkcie(Odbij(X, Z.P1, Z.P2), tab[Z.id_ksztaltu], tab);
        case 0:
            return Ile_w_punkcie(X, tab[Z.id_ksztaltu], tab);
        case 1:
            return 0;
        default:
            assert(0);
    }
}

int Ile_w_punkcie(Punkt X, Ksztalt ksztalt, Ksztalt tab[]){
    switch (ksztalt.repr) {
        case Pr:
            return Czy_w_prostokacie(X, ksztalt.val.p);
        case Ko:
            return Czy_w_kole(X, ksztalt.val.k);
        case Zg:
            return Ile_w_zgieciu(X, ksztalt.val.z, tab);
        default:
            assert(0);
    }
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
    Ksztalt *tab = (Ksztalt*) malloc((unsigned long)(n+1) * sizeof(Ksztalt));
    for(int i = 1; i<=n; i++){
        scanf(" %c", (char*) &(tab[i].repr));
        switch (tab[i].repr) {
            case Pr:
                scanf("%lf %lf %lf %lf", &(tab[i].val.p.LD.x), &(tab[i].val.p.LD.y),
                      &(tab[i].val.p.PG.x), &(tab[i].val.p.PG.y));
                break;
            case Ko:
                scanf("%lf %lf %lf", &(tab[i].val.k.O.x), &(tab[i].val.k.O.y), &(tab[i].val.k.r));
                break;
            case Zg:
                scanf("%d %lf %lf %lf %lf", &(tab[i].val.z.id_ksztaltu), &(tab[i].val.z.P1.x),
                      &(tab[i].val.z.P1.y), &(tab[i].val.z.P2.x), &(tab[i].val.z.P2.y));
                break;
            default:
                assert(0);
        }
    }
    for(int i = 0; i<q; i++){
        int id_ksztaltu;
        Punkt X;
        scanf("%d %lf %lf", &id_ksztaltu, &(X.x), &(X.y));
        printf("%d\n", Ile_w_punkcie(X, tab[id_ksztaltu], tab));
    }
    free(tab);
    return 0;
}
