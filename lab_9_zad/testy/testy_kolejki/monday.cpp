#include <cassert>
#include <vector>
#include <cstdlib>
#include <cstdio>

#include "kol.h"

//sprawdza, czy wszystkie kolejki sa puste
void puste_kolejki()
{
  for(int i = 0; i < 7; i++)
  {
    assert(obsluz(i) == NULL);
  }
}

//generyczne przychodzenie, wychodzenie z urzedu
void rano()
{
  interesant *pierwsi[10];
  for (int i = 0; i < 4; i++)
  {
    pierwsi[i] = nowy_interesant(4);
    assert(numerek(pierwsi[i]) == i);
  }
  interesant *wolny;
  for (int i = 0; i < 3; i++)
  {
    wolny = obsluz(4);
    assert(numerek(wolny) == i);
    free(wolny);
  }
  pierwsi[4] = nowy_interesant(4);
  pierwsi[5] = nowy_interesant(4);
  assert(numerek(pierwsi[4]) == 4 && numerek(pierwsi[5]) == 5);
  for (int i = 0; i < 3; i++)
  {
    wolny = obsluz(4);
    assert(numerek(wolny) == i + 3);
    free(wolny);
  }
  assert(obsluz(4) == NULL);
  for (int i = 6; i < 10; i++)
  {
    pierwsi[i] = nowy_interesant(4);
    assert(numerek(pierwsi[i]) == i);
  }
  for (int i = 6; i < 10; i++)
  {
    wolny = obsluz(4);
    assert(numerek(wolny) == i);
    free(wolny);
  }
  wolny = obsluz(4);
  assert(wolny == NULL);
  wolny = obsluz(6);
  assert(wolny == NULL);
  puste_kolejki();
}

//assertuje, ze numerek interesanta to nr i zwalnia interesanta
void jn(interesant *i, int nr)
{
  if (numerek(i) != nr)printf("Błąd z interesantem numer %d!!\n Jest on na pozycji interesanta %d:(\n",numerek(i), nr);
  assert(numerek(i) == nr);
  free(i);
}

//testy naczelnika
void naczelnik_przychodzi()
{
  interesant *ofiary[10];
  ofiary[0] = nowy_interesant(1);
  assert(numerek(ofiary[0]) == 10);
  naczelnik(1);
  interesant *wolny;
  wolny = obsluz(1);
  assert(numerek(wolny) == 10);
  free(wolny);
  for (int i = 1; i < 4; i++)
  {
    ofiary[i] = nowy_interesant(0);
    assert(numerek(ofiary[i]) == i + 10);
  }
  naczelnik(0);
  for (int i = 3; i > 0; i--)
  {
    wolny = obsluz(0);
    //printf("%d, %d\n", numerek(wolny), i + 10);
    assert(numerek(wolny) == i + 10);
    free(wolny);
  }
  assert(obsluz(0) == NULL);

  for (int i = 4; i < 10; i++)
  {
    ofiary[i] = nowy_interesant(2);
    naczelnik(2);
    assert(numerek(ofiary[i]) == i + 10);
  }
  interesant *dead[6];
  for (int i = 0; i < 6; i++)
  {
    dead[i] = obsluz(2);
  }
  jn(dead[0], 19);
  jn(dead[1], 17);
  jn(dead[2], 15);
  jn(dead[3], 14);
  jn(dead[4], 16);
  jn(dead[5], 18);
  puste_kolejki();
}

//sprawdza zmiene okienek
void zle_kolejki()
{
  interesant *zle[10];
  zle[0] = nowy_interesant(3);
  zle[1] = nowy_interesant(2);
  zmiana_okienka(zle[0], 2);
  interesant *wolny;
  wolny = obsluz(2);
  jn(wolny, 21);
  wolny = obsluz(2);
  jn(wolny, 20);
  puste_kolejki();
  for (int i = 2; i < 6; i++)
  {
    zle[i] = nowy_interesant(6);
    assert(numerek(zle[i]) ==  i + 20);
  }
  zmiana_okienka(zle[4], 1);
  zmiana_okienka(zle[5], 2);
  zmiana_okienka(zle[2], 0);
  zmiana_okienka(zle[3], 2);
  wolny = obsluz(0);
  jn(wolny, 22);
  wolny = obsluz(2);
  jn(wolny, 25);
  wolny = obsluz(2);
  jn(wolny, 23);
  wolny = obsluz(1);
  jn(wolny, 24);
  puste_kolejki();
  for (int i = 6; i < 10; i++)
  {
    zle[i] = nowy_interesant(3);
    assert(numerek(zle[i]) == i + 20);
  }
  naczelnik(3);
  for (int i = 6; i < 10; i++)
  {
    zmiana_okienka(zle[i], 1);
  }
  for (int i = 26; i < 30; i++)
  {
    wolny = obsluz(1);
    jn(wolny, i);
  }
  puste_kolejki();
}

//sprawdza zamykanie okienek
void zamykanie_kolejek()
{
  zamkniecie_okienka(6, 1);
  puste_kolejki();
  interesant *pechowcy[10];
  for (int i = 0; i < 3; i++)
  {
    pechowcy[i] = nowy_interesant(4);
    assert(numerek(pechowcy[i]) == i + 30);
  }
  zamkniecie_okienka(1, 4);
  zamkniecie_okienka(4, 5);
  interesant *wolny;
  for (int i = 30; i < 33; i++)
  {
    wolny = obsluz(5);
    jn(wolny, i);
  }
  puste_kolejki();
  for (int i = 3; i < 7; i++)
  {
    pechowcy[i] = nowy_interesant(2);
    assert(numerek(pechowcy[i]) == i + 30);
    naczelnik(2);
  }
  for (int i = 7; i < 10; i++)
  {
    pechowcy[i] = nowy_interesant(1);
    assert(numerek(pechowcy[i]) == i + 30);
    naczelnik(1);
  }
  interesant *wolne;
  zamkniecie_okienka(2, 1);
  wolne = obsluz(1);
  jn(wolne, 39);
  wolne = obsluz(1);
  jn(wolne, 37);
  wolne = obsluz(1);
  jn(wolne, 38);
  wolne = obsluz(1);
  jn(wolne, 36);
  wolne = obsluz(1);
  jn(wolne, 34);
  wolne = obsluz(1);
  jn(wolne, 33);
  wolne = obsluz(1);
  jn(wolne, 35);
  puste_kolejki();
}

//sprawdza fast_track
void szybkie_okienko()
{
  interesant *fast[20];
  fast[0] = nowy_interesant(3);
  assert(numerek(fast[0]) == 40);
  std::vector<interesant *> lucky;
  lucky = fast_track(fast[0], fast[0]);
  assert(lucky.size() == 1U);
  assert(numerek(lucky[0]) == 40);
  free(fast[0]);
  lucky.clear();
  puste_kolejki();
  for (int i = 1; i < 5; i++)
  {
    fast[i] = nowy_interesant(1);
    assert(numerek(fast[i]) == i + 40);
    naczelnik(1);
  }
  fast[5] = nowy_interesant(0);
  assert(numerek(fast[5]) == 45);
  for (int i = 6; i < 10; i++)
  {
    fast[i] = nowy_interesant(5);
    assert(numerek(fast[i]) == 40 + i);
    naczelnik(5);
  }
  zmiana_okienka(fast[5], 5);
  zamkniecie_okienka(1, 5);
  lucky = fast_track(fast[2], fast[2]);
  assert(lucky.size() == 1U);
  assert(numerek(lucky[0]) == 42);
  free(lucky[0]);
  lucky.clear();
  lucky = fast_track(fast[6], fast[1]);
  assert(lucky.size() == 5U);
  jn(lucky[0], 46);
  jn(lucky[1], 48);
  jn(lucky[2], 45);
  jn(lucky[3], 44);
  jn(lucky[4], 41);
  lucky.clear();
  interesant *wolny;
  wolny = obsluz(5);
  jn(wolny, 49);
  wolny = obsluz(5);
  jn(wolny, 47);
  wolny = obsluz(5);
  jn(wolny, 43);
  puste_kolejki();
  for (int i = 10; i < 13; i++)
  {
    fast[i] = nowy_interesant(3);
    assert(numerek(fast[i]) == i + 40);
    naczelnik(3);
  }
  for (int i = 13; i < 16; i++)
  {
    fast[i] = nowy_interesant(4);
    assert(numerek(fast[i]) == i + 40);
    naczelnik(4);
  }
  for (int i = 16; i < 19; i++)
  {
    fast[i] = nowy_interesant(2);
    assert(numerek(fast[i]) == i + 40);
    naczelnik(2);
  }
  fast[19] = nowy_interesant(5);
  assert(numerek(fast[19]) == 59);
  zamkniecie_okienka(4, 6);
  zmiana_okienka(fast[19], 6);
  naczelnik(6);
  zamkniecie_okienka(2, 6);
  naczelnik(6);
  zamkniecie_okienka(3, 6);
  naczelnik(6);
  lucky = fast_track(fast[11], fast[17]);
  assert(lucky.size() == 10U);
  jn(lucky[0], 51);
  jn(lucky[1], 50);
  jn(lucky[2], 52);
  jn(lucky[3], 59);
  jn(lucky[4], 54);
  jn(lucky[5], 53);
  jn(lucky[6], 55);
  jn(lucky[7], 58);
  jn(lucky[8], 56);
  jn(lucky[9], 57);
  lucky.clear();
  puste_kolejki();
}

//zamyka urzad i sprawdza, czy interesanci zostali zwroceni w odpowiedniej kolejnosci
void zapada_noc()
{
  interesant *nieobsluzeni[14];
  for (int i = 0; i < 5; i++)
  {
    nieobsluzeni[i] = nowy_interesant(1);
    assert(numerek(nieobsluzeni[i]) == 60 + i);
  }
  for (int i = 5; i < 7; i++)
  {
    nieobsluzeni[i] = nowy_interesant(2);
    assert(numerek(nieobsluzeni[i]) == 60 + i);
  }
  for (int i = 7; i < 12; i++)
  {
    nieobsluzeni[i] = nowy_interesant(4);
    assert(numerek(nieobsluzeni[i]) == 60 + i);
  }
  for (int i = 12; i < 14; i++)
  {
    nieobsluzeni[i] = nowy_interesant(6);
    assert(numerek(nieobsluzeni[i]) == 60 + i);
  }
  zamkniecie_okienka(6, 5);
  zamkniecie_okienka(4, 6);
  zamkniecie_okienka(5, 4);
  naczelnik(2);
  zamkniecie_okienka(2, 1);
  zmiana_okienka(nieobsluzeni[4], 3);
  zmiana_okienka(nieobsluzeni[9], 3);
  zmiana_okienka(nieobsluzeni[5], 3);
  zmiana_okienka(nieobsluzeni[0], 1);
  std::vector<interesant *> au_revoir = zamkniecie_urzedu();
  assert(au_revoir.size() == 14U);
  jn(au_revoir[0], 61);
  jn(au_revoir[1], 62);
  jn(au_revoir[2], 63);
  jn(au_revoir[3], 66);
  jn(au_revoir[4], 60);
  jn(au_revoir[5], 64);
  jn(au_revoir[6], 69);
  jn(au_revoir[7], 65);
  jn(au_revoir[8], 72);
  jn(au_revoir[9], 73);
  jn(au_revoir[10], 67);
  jn(au_revoir[11], 68);
  jn(au_revoir[12], 70);
  jn(au_revoir[13], 71);
  au_revoir.clear();
}

int main()
{
  printf("Witaj w urzędzie.\n");
  otwarcie_urzedu(7);
  rano();
  printf("\"Rano\" nie nastąpiły żadne problemy.\n");
  naczelnik_przychodzi();
  printf("\"Naczelnik\" nie wywołał zbyt dużego chaosu.\n");
  zle_kolejki();
  printf("Pomimo \"zmian okienek\" spowodowanych \"złymi kolejkami\", urząd pracował sprawnie\n");
  zamykanie_kolejek();
  printf("\"Zamknięcia okienek\" nie przysporzyło żadnych problemów\n");
  szybkie_okienko();
  printf("\"Fast-tracki\" nie rozwaliły porzadku pracy urzedu\n");
  zapada_noc();
  printf("Wszytkie operacje zostały pomyślnie zakończone, urząd został już zamknięty, nie wystąpiły żadne problemy, wszystko gra :)!!!\n");
}
