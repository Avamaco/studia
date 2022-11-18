#!/bin/bash
tests=0
while [ 1 ]; do
  tests=$(($tests+1))  # inkrementacja zmiennej
  echo -n "Test #$tests... "  # echo -n wypisuje bez newline
  echo "$tests $1 $2 $3" | ./gen >/tmp/p.in  # podajemy numer testu i parametry wywołania skryptu do generatora
  ./brutele </tmp/p.in >/tmp/p1.out
  ./main </tmp/p.in >/tmp/p2.out
  check=`diff -w -q /tmp/p1.out /tmp/p2.out`  # diff sprawdza, czy pliki sie roznia;
                                    # opcja -w ignoruje biale znaki; opcja -q wypisuje tylko czy pliki sie roznia
  if [ "$check" != "" ]; then  # czy diff niepusty
    echo "FAILED"
    exit
  fi
  echo -n "OK "
  cat /tmp/p1.out | head -n 1  # Wypisuje pierwszy (tu akurat jedyny) wiersz wyniku bruta
done