testGen: build/testLosowy.o build/kol.o
	@g++ build/testLosowy.o build/kol.o -o testGen

build/testLosowy.o: testLosowy.cpp
	@g++ -c testLosowy.cpp -o build/testLosowy.o

build/kol.o: kol.cpp kol.h
	@g++ -c kol.cpp -o build/kol.o

testGenMati: build/testLosowyMati.o build/kolMati.o
	@g++ build/testLosowy.o build/kolMati.o -o testGenMati

build/testLosowyMati.o: mati/testLosowy.cpp
	@g++ -c mati/testLosowy.cpp -o build/testLosowyMati.o

build/kolMati.o: mati/kol.cpp mati/kol.h
	@g++ -c mati/kol.cpp -o build/kolMati.o