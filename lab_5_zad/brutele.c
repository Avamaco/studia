#include <stdio.h>
#include <stdlib.h>

int main(){
    int n;
    scanf("%d", &n);

    int * s = malloc(n * sizeof(int));
    int * t = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        scanf("%d%d", &s[i], &t[i]);
    }

    int minimum = t[n - 1] + 1;
    int maximum = 0;

    for (int m1 = 0; m1 < n; m1++){
        for(int m2 = m1 + 1; m2 < n; m2++){
            for(int m3 = m2 + 1; m3 < n; m3++){
                if (s[m1] != s[m2] && s[m2] != s[m3] && s[m1] != s[m3]){
                    int d1 = t[m2] - t[m1];
                    int d2 = t[m3] - t[m2];
                    int mind = d1<d2 ? d1 : d2;
                    int maxd = d1>d2 ? d1 : d2;
                    minimum = minimum<maxd ? minimum : maxd;
                    maximum = maximum>mind ? maximum : mind;
                }
            }
        }
    }

    if (minimum == t[n - 1] + 1) minimum = 0;

    printf("%d %d\n", minimum, maximum);

    free(s);
    free(t);

    return 0;
}