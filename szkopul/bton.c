#include <stdio.h>
#include <stdlib.h>

int main(){
  int N; scanf("%d", &N);
  int A[N]; for(int i = 0; i < N; i++) scanf("%d", &A[i]);
  int wynik;
  // Mamy ustalone N, zawartość tablicy A oraz zadeklarowaną zmienną wynik 
  // ---------------------------------------------------------------------
  // Poniżej wpisz swój fragment programu:

  wynik = 1;

  int i;
  for (i = 1; i < N; i++){
    if (A[i] <= A[i - 1])
      break;
  }
  while (i < N){
    if (A[i] >= A[i - 1])
      wynik = 0;
    i++;
  }

  if (N == 0) wynik = 0;
       
  // ---------------------------------------------------------------------
  printf("%d\n", wynik);
  return 0;

}
