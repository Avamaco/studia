#include <cassert>
#include <vector>
#include <cstdio>

#include "kol.h"

int main()
{
  printf("Witaj w urzędzie.\n");
  otwarcie_urzedu(10);
  std::vector<interesant *> rip = zamkniecie_urzedu();
  assert(rip.size() == 0);
  printf("Niestety dzisiaj został już on zamknięty zanim jakikolwiek interesant przyszedł.\n");
  printf("Nie nastąpiły na szczęście żadne problemy. Miłego dnia.\n");
  return 0;
}
