#include <stdio.h>
#include <stdlib.h>

int main(){
    int n;
    scanf("%d", &n);
    int * liczby = malloc(n * sizeof(n));

    for (int i = 0; i < n; i++)
        scanf("%d", &liczby[i]);

    int szukana;
    scanf("%d", &szukana);

    int left = 0, right = n - 1;

    while(left < right){
        int mid = (left + right) / 2;
        if (liczby[mid] < szukana){
            left = mid + 1;
        }
        else{
            right = mid;
        }
    }

    printf("Szukanie zakończone na pozycji %d. Jest to liczba %d.\n", left, liczby[left]);

    free(liczby);

    return 0;
}