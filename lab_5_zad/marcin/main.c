#include <stdio.h>
#include <stdlib.h>

typedef struct hotele{
  int odl;
  int typ;
}hotele;

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int min(int a, int b) {
  if (a < b)
    return a;
  return b;
}

void swap(int * a, int * b) {
  int temp = * a;
  * a = * b;
  * b = temp;
}

// zwraca największą różnicę trzech wyrazów
int max_roznica(int a, int b, int c) {
  return max((c - b), (b - a));
}

// najpierw sortuje wyrazy, następnie zwraca ich najmnieszą różnicę
int min_roznica(int a, int b, int c) {
  int najw = max(max(a, b), c);
  int najm = min(min(a, b), c);
  int sr;
  if (a != najw && a != najm)
    sr = a;
  else if (b != najw && b != najm)
    sr = b;
  else
    sr = c;
  return min((najw - sr), (sr - najm));
}

int policz_max_abc(int A[], int B[], int C[], int i, int j, int k, int max_roz) {
  int ile_razy, obecna_roznica;
  ile_razy = j + k + 1;
  while (ile_razy--) {
    obecna_roznica = min_roznica(A[i], B[j], C[k]);
    max_roz = max(max_roz, obecna_roznica);

    if ((A[i] - B[j] < B[j] - C[k]) && j != 0)
      j--;
    else if (k != 0)
      k--;
  }
  return max_roz;
}

int max_hotele(int A[], int B[], int C[], int i, int j, int k) {
  if (i == -1 || j == -1 || k == -1)
    return 0;

  int max_roz = 0;

  max_roz = min_roznica(A[i], B[j], C[k]);

  // Wykonam trzy testy najpierw jako element najwiekszy wezmę A[i], następnie B[j] i na konie C[k]
  // W każdym teście rozpatrzę 2 przypadki

  // Najpierw A >= B >= C
  max_roz = max(max_roz, policz_max_abc(A, B, C, i, j, k, max_roz));
  // A >= C >= B
  max_roz = max(max_roz, policz_max_abc(A, C, B, i, k, j, max_roz));
  // B >= A >= C
  max_roz = max(max_roz, policz_max_abc(B, A, C, j, i, k, max_roz));
  // B >= C >= A
  max_roz = max(max_roz, policz_max_abc(B, C, A, j, k, i, max_roz));
  // C >= A >= B
  max_roz = max(max_roz, policz_max_abc(C, A, B, k, i, j, max_roz));
  // C >= B >= A
  max_roz = max(max_roz, policz_max_abc(C, B, A, k, j, i, max_roz));

  return max_roz;
}

int main() {
  // n - liczba hoteli
  int n;
  scanf("%d", & n);

  int *jedynki;
  jedynki = (int*)malloc((size_t)n * sizeof(n));

  int *dwojki;
  dwojki = (int*)malloc((size_t)n * sizeof(int));

  int *trojki;
  trojki = (int*)malloc((size_t)n * sizeof(int));

  //j, k, l - faktyczna wielkość tablic odpowiednio: jedynki, dwojki, trojki
  int j = 0, k = 0, l = 0;
  int min_odl = 0, max_odl = 0, min_odl2 = 0, max_odl2 = 0;

  hotele *hotel;
  hotel = (hotele*)malloc((size_t)n * sizeof(hotel));
  

  for (int i = 0; i < n; i++)
      scanf("%d %d", & hotel[i].typ, & hotel[i].odl);

  int ile = n - 1;
  int pierwszy = hotel[0].typ;
  int ostatni = hotel[ile].typ;
  int czy_sprawdzac = 0;

  // Można zauważyć, że do policzenia maxa na pewno będziemy korzystać z hoteli typów takich, jak pierwszy i ostatni.
  // Wtedy możemy rozwiązać pierwszą część zadania sprowadzając je do przypadku, w którym mamy tylko trzy typy hoteli:
  // jedynki - typ pierwszy, dwojki - typ drugi, trojki - pozostałe
  // Na początku zbieram dane do structa
  // Dzięki temu mam łatwy dostęp do odległości hotelu od początku autostrady i do typu hotelu.

  while (ostatni == pierwszy) {
    ostatni = hotel[--ile].typ;
    czy_sprawdzac = 1;
  }

  for (int i = 0; i < n; i++) {
    if (hotel[i].typ == pierwszy)
      jedynki[j++] = hotel[i].odl;
    else if (hotel[i].typ == ostatni)
      dwojki[k++] = hotel[i].odl;
    else
      trojki[l++] = hotel[i].odl;
  }

  max_odl = max_hotele(jedynki, dwojki, trojki, j - 1, k - 1, l - 1);

  // Może się zdarzyć że jakiś typ hotelu występuje na początku i na końcu, wtedy musimy sprawdzić dwie opcje drugiego elementu

  if (czy_sprawdzac) {
    pierwszy = hotel[0].typ;
    ostatni = hotel[n - 1].typ;

    j = 0, k = 0, l = 0;
    int licznik = 0;
    while (ostatni == pierwszy)
      pierwszy = hotel[++licznik].typ;

    for (int i = 0; i < n; i++) {
      if (hotel[i].typ == pierwszy)
        jedynki[j++] = hotel[i].odl;
      else if (hotel[i].typ == ostatni)
        dwojki[k++] = hotel[i].odl;
      else
        trojki[l++] = hotel[i].odl;
    }
    max_odl2 = max_hotele(jedynki, dwojki, trojki, j - 1, k - 1, l - 1);
  }
  
  max_odl = max(max_odl, max_odl2);

  free(jedynki);
  free(dwojki);
  free(trojki);

  // Do liczenia minimum zastosuję metodę gąsienicy
  // Najpierw ustawiam początkowe wartości
  // Będę je przechowywał w formacie:
  // jeden - pierwsza wartość, dwa - tablica możliwych wartości drugiego hotelu, trzy - trzecia wartość

  int jeden, typ1, typ2, trzy = -1, typ3 = -1, licznik = 1, licznik2 = 0;
  int *dwa;
  dwa = (int*)malloc((size_t)n * sizeof(int));

  jeden = hotel[0].odl;
  typ1 = hotel[0].typ;


  while (hotel[licznik].typ == hotel[licznik-1].typ) {
    jeden = hotel[licznik++].odl;
  }

  if(licznik + 1 < n){
    dwa[licznik2++] = hotel[licznik].odl;
    typ2 = hotel[licznik++].typ;

    while (hotel[licznik - 1].typ == hotel[licznik].typ) {
        dwa[licznik2++] = hotel[licznik].odl;
        licznik++;
    }
  }else
    licznik++;

  while (hotel[licznik].typ == typ1 && licznik < n) {
    jeden = dwa[licznik2 - 1];
    swap( & typ2, & typ1);
    licznik2 = 0;
    dwa[licznik2++] = hotel[licznik].odl;
    licznik++;

    while (hotel[licznik - 1].typ == hotel[licznik].typ && licznik < n) {
      dwa[licznik2++] = hotel[licznik].odl;
      licznik++;
    }
  }

  // Liczę minimalną odległość osobno dla każdej możliwej wartości dwójki
  if (licznik < n) {
    trzy = hotel[licznik].odl;
    typ3 = hotel[licznik++].typ;
    min_odl = max_roznica(jeden, dwa[0], trzy);
    for (int i = 1; i < licznik2; i++) {
      min_odl2 = max_roznica(jeden, dwa[i], trzy);
      min_odl = min(min_odl, min_odl2);
    }
  }

  // Następnie gąsienica przechodzi przez wszystkie hotele
  int nast_typ, licznik3 = licznik;
  while (1) {

    licznik3 = licznik + 1;
    while (hotel[licznik3].typ == typ3 && licznik3 < n)
      licznik3++;
    if (licznik3 - 2 <= n)
      nast_typ = hotel[licznik3 - 2].typ;
    else
      nast_typ = 0;

    if ((nast_typ != typ1 && nast_typ != typ2) || nast_typ == typ1) {
      typ1 = typ2;
      jeden = dwa[licznik2 - 1];
      licznik2 = 0;
      dwa[licznik2++] = trzy;
      typ2 = typ3;

      while (hotel[licznik].typ == typ3 && licznik < n + 1)
        dwa[licznik2++] = hotel[licznik++].odl;
      trzy = hotel[licznik].odl;
      typ3 = hotel[licznik++].typ;

    } else if (nast_typ == typ2) {
      while (nast_typ == typ2 && licznik < n && nast_typ) {
        while (hotel[licznik++].typ == typ2);

        licznik2 = 0;

        dwa[licznik2++] = hotel[licznik - 1].odl;
        typ2 = hotel[licznik - 1].typ;

        if (licznik >= n)
          break;

        trzy = hotel[licznik].odl;
        typ3 = hotel[licznik++].typ;
        licznik3 = licznik;

        while (hotel[licznik3].typ == typ3 && licznik3 < n)
          licznik3++;
        nast_typ = hotel[licznik3 - 1].typ;

      }
      licznik3 = licznik - 1;
      while (hotel[licznik3].typ == typ2 || hotel[licznik3].typ == typ3)
        licznik3--;
      jeden = hotel[licznik3].odl;
      typ1 = hotel[licznik3].typ;
    }
    if (licznik > n)
      break;

    // Pod koniec każdej pętli obliczam minimalną odległość
    for (int i = 0; i < licznik2; i++) {
      if (typ1 != typ2 && typ2 != typ3 && typ1 != typ3 && typ3 != -1 && nast_typ) {
        min_odl2 = max_roznica(jeden, dwa[i], trzy);
        min_odl = min(min_odl, min_odl2);
      }
    }

    if (licznik >= n)
      break;
  }

  free(dwa);
  free(hotel);

  printf("%d %d\n", min_odl, max_odl);
  
  return 0;
}
