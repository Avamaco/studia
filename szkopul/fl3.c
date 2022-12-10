#include <stdio.h>
#include <stdlib.h>
void _zakres(int i, char* is, int M, char *funkcja){
    if (! (0 <= i && i < M)) {
        fprintf(stderr, "Przekroczenie zakresu w %s: argument %s to %d, miało być 0 <= %s < %d\n",
                funkcja, is, i, is, M); exit(1); } }

int _wartosc(int i, int t[], int N){
    _zakres(i, "i", 2*N, "Wartosc"); return t[i]; }

void _zamien(int i, int j, int t[], int N){
    _zakres(i, "i", 2*N, "Zamien"); _zakres(j, "j", 2*N, "Zamien");
    int tmp = t[i]; t[i] = t[j]; t[j] = tmp; }

int main(){
    int N; scanf("%d", &N);
    int t[2*N]; for(int i = 0; i < 2*N; i++) scanf("%d", &t[i]);
    #define Wartosc(i) _wartosc(i,t,N)
    #define Zamien(i,j) _zamien(i,j,t,N)
// Mamy ustalone N i zawartość urn
// -------------------------------------
// Poniżej wpisz swój fragment programu:
// -------------------------------------

    int uje = 0;
    int dod = 1;

    while (uje < 2 * N){
        while (uje < 2 * N){
            if (Wartosc(uje) >= 0)
                break;
            uje += 2;
        }

        while (dod < 2 * N){
            if (Wartosc(dod) < 0)
                break;
            dod += 2;
        }

        if (uje < 2 * N)
            Zamien(uje, dod);
    }

    dod = 1;
    int zera = 1;

    while (dod < 2 * N){
        if (Wartosc(dod) == 0){
            Zamien(dod, zera);
            zera += 2;
        }
        dod += 2;
    }

    for(int i=0; i<2*N; i++) printf("%d ", t[i]);
    printf("\n");
    return 0;
}