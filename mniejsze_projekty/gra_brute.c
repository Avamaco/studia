#include <stdio.h>

int NWD(int a, int b){
    if (a % b == 0)
        return b;
    else
        return NWD(b ,a % b);
}

int czyWygryw(int a, int b){

    int przeg = 1;
    int nwd = 0;
    if (a > 1){
        nwd = NWD(a - 1, b);
        przeg *= czyWygryw((a - 1) / nwd, b / nwd);
    }
    if (b > 1){
        nwd = NWD(a, b - 1);
        przeg *= czyWygryw(a / nwd, (b - 1) / nwd);
    }
    
    return 1 - przeg;
    
}

void jedenTest(){
    int a, b;
    scanf("%d", &a);
    scanf("%d", &b);

    int n = czyWygryw(a, b);
    printf("%d", n);
    return;
}



int main(){
    int x;
    scanf("%d", &x);
    for (int i = 1; i <= x; i++){
        for (int j = 1; j <= x; j++){
            int w = czyWygryw(i, j);
            printf("%d", w);
        }
        printf("\n");
    }

    return 0;
}