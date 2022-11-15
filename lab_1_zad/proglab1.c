#include <stdlib.h>
#include <stdio.h>

int main(){
    int n;
    scanf("%d", &n);

    int* liczby = malloc(n * sizeof(int));
    int* dzielne = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        scanf("%d", &liczby[i]);
    }

    int maxdl = 0;
    for (int i = 0; i < n; i++){
        int j = i - 1;
        while (j >= 0){
            if (liczby[j] == 0)
                break;
            if (liczby[i] % liczby[j] != 0)
                break;
            j -= dzielne[j];
        }

        int dl = i - j;
        dzielne[i] = dl;

        if (dl > maxdl)
            maxdl = dl;
    }

    printf ("%d", maxdl);

    free(liczby);
    free(dzielne);

    return 0;
}
