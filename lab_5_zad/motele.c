#include <stdio.h>
#include <stdlib.h>


typedef struct motel{
    int num;    // który z kolei jest to motel
    int siec;   // do jakiej sieci należy
    int poz;    // odległość od początku autostrady
} motel;

// przechowuje po dwa motele z innych sieci przed i za konkretnym motelem. Motel numer -1 oznacza brak motelu.
typedef struct inne_sieci{
    motel przed[2];
    motel za[2];
} inne_sieci;


int main(){
    int n;
    scanf("%d", &n);

    // jest mniej niż 3 motele więc warunki zadania na pewno nie są spełnione
    if (n < 3){
        printf("0 0");
        return 0;
    }
    
    motel * motele = malloc(n * sizeof(motel));

    for (int i = 0; i < n; i++){
        scanf("%d%d", &motele[i].siec, &motele[i].poz);
        motele[i].num = i;
    }

    // bliscy[i] - dwa najbliższe motele z innych sieci przed motelem [i] i dwa najbliższe motele z innych sieci za motelem [i]
    inne_sieci * bliscy = malloc(n * sizeof(inne_sieci));

    // znajduję 3 najbliższe początku autostrady motele z różnych sieci
    motel m1, m2, m3;
    m1 = motele[0];
    for (int i = 0; motele[i].siec == m1.siec; i++){
        if (motele[i].siec == m1.siec && i == n - 1){   // jest mniej niż 3 różne sieci
            printf("0 0");
            return 0;
        }
    }
    for (int i = m2.num; motele[i].siec == m2.siec; i++){
        if (motele[i].siec == m2.siec && i == n - 1){   // jest mniej niż 3 różne sieci
            printf("0 0");
            return 0;
        }
    }



    free(bliscy);
    free(motele);

    return 0;
}