#include <bits/stdc++.h>
#include "kol.h"

namespace kol {
	struct urzad {
		interesant *poczatek;
		interesant *koniec;
	};

	int numerek;
	std::vector<urzad> kolejki;

	void link(interesant *el, interesant *nowy, interesant *stary) {
		if (el == NULL)
			return;

		if (el->i1 == stary)
			el->i1 = nowy;
		else
			el->i2 = nowy;
	}

	void dodaj_interesanta(interesant *i, int k) {
		i->i1 = kolejki[k].koniec;
		i->i2 = kolejki[k].koniec->i2;
		link(kolejki[k].koniec->i2, i, kolejki[k].koniec);
		kolejki[k].koniec->i2 = i;
	}

	void usun_interesanta(interesant *i) {
		if (i == NULL)
			return;

		interesant *lewy = i->i1;
		interesant *prawy = i->i2;
		link(i->i1, prawy, i);
		link(i->i2, lewy, i);
	}
	
	interesant *nastepny_interesant(interesant *el, interesant *poprzedni) {
		if (el == NULL)
			return NULL;
		
		if (el->i1 == poprzedni)
			return el->i2;
		else
			return el->i1;
	}
}

void otwarcie_urzedu(int m) {
	kol::kolejki.resize(m);
	kol::numerek = 0;

	for (int i = 0; i < m; i++) {
		interesant *poczatek = (interesant*) malloc(sizeof(interesant));
		interesant *koniec = (interesant*) malloc(sizeof(interesant));
		poczatek->i2 = koniec;
		koniec->i2 = poczatek;
		poczatek->i1 = NULL;
		koniec->i1 = NULL;

		kol::kolejki[i].poczatek = poczatek;
		kol::kolejki[i].koniec = koniec;
	}
}

interesant* nowy_interesant(int k) {
	interesant *nowy = (interesant*) malloc(sizeof(interesant));

	nowy->numerek = kol::numerek++;
	kol::dodaj_interesanta(nowy, k);
	return nowy;
}

int numerek (interesant *i) {
	return i->numerek;
}

interesant *obsluz(int k) {
	if (kol::kolejki[k].poczatek->i2 == kol::kolejki[k].koniec)
		return NULL;

	interesant *i = kol::kolejki[k].poczatek->i2;
	kol::usun_interesanta(i);
	return i;
}

void zmiana_okienka(interesant *i, int k) {
	kol::usun_interesanta(i);
	kol::dodaj_interesanta(i, k);
}

void zamkniecie_okienka(int k1, int k2) {
	interesant *pierwszy = kol::kolejki[k1].poczatek->i2;
	interesant *ostatni = kol::kolejki[k1].koniec->i2;
	// std::cout << "zamknc_ok: " << pierwszy << "\n";

	kol::link(pierwszy, kol::kolejki[k2].koniec->i2, kol::kolejki[k1].poczatek);
	kol::link(kol::kolejki[k2].koniec->i2, pierwszy, kol::kolejki[k2].koniec);

	kol::link(ostatni, kol::kolejki[k2].koniec, kol::kolejki[k1].koniec);
	kol::kolejki[k2].koniec->i2 = ostatni;

	kol::kolejki[k1].poczatek->i2 = kol::kolejki[k1].koniec;
	kol::kolejki[k1].koniec->i2 = kol::kolejki[k1].poczatek;
}

void xd() {
	// std::cout << "xddmalsdnkajndkjlasndkl " << kol::kolejki[0].poczatek->i1 << "\n";
}

std::vector<interesant*> fast_track(interesant *i1, interesant *i2) {
	std::vector<interesant*> res1, res2;
	res1.push_back(i1);
	res2.push_back(i1);

	interesant *lewo = i1->i1;
	interesant *prawo = i1->i2;
	interesant *poprz_lewo = i1;
	interesant *poprz_prawo = i1;

	while (lewo != i2 && prawo != i2) {
		if (lewo != NULL)
			res1.push_back(lewo);
		if (prawo != NULL)
			res2.push_back(prawo);

		interesant *tmp = lewo;
		lewo = kol::nastepny_interesant(lewo, poprz_lewo);
		poprz_lewo = tmp;

		tmp = prawo;
		prawo = kol::nastepny_interesant(prawo, poprz_prawo);
		poprz_prawo = tmp;
	}
	
	if (lewo == i2) {
		// std::cout << "lewo\n";
		res1.push_back(lewo);

		interesant *nastepny = kol::nastepny_interesant(lewo, poprz_lewo);
		kol::link(i1->i2, nastepny, i1);
		kol::link(nastepny, i1->i2, i2);

		return res1;
	}
	else { // prawo == i2
		// std::cout << "prawo " << (prawo == i2) << "\n";
		res2.push_back(prawo);

		interesant *nastepny = kol::nastepny_interesant(prawo, poprz_prawo);
		// std::cout << (nastepny == kol::kolejki[0].koniec) << " asdhashjgdalk " << prawo << " " << kol::kolejki[0].koniec->i2 << " " << i1->i1 << " " << nastepny->i1 << "\n";
		kol::link(i1->i1, nastepny, i1);
		kol::link(nastepny, i1->i1, i2);
		// std::cout << "asdhashjgdalk " << kol::kolejki[0].koniec->i2 << "\n";

		return res2;
	}
}

void naczelnik(int k) {
	// std::cout << "naczelnik" << k << " " << kol::kolejki[0].poczatek->i2 << "\n";
	interesant* tmp = kol::kolejki[k].poczatek;
	kol::kolejki[k].poczatek = kol::kolejki[k].koniec;
	kol::kolejki[k].koniec = tmp;
	// std::cout << "naczelnikPO" << k << " " << kol::kolejki[0].poczatek->i2 << "\n";
}

std::vector<interesant*> zamkniecie_urzedu() {
	std::vector<interesant*> res;

	for (unsigned int k = 0; k < kol::kolejki.size(); k++) {
		// std:: cout << k << ": ";
		while (true) {
			interesant *i = obsluz(k);
			// std::cout << i << " ";
			if (i == NULL)
				break;

			res.push_back(i);
		}
		// std::cout << std::endl;

		free(kol::kolejki[k].poczatek);
		free(kol::kolejki[k].koniec);
	}

	kol::kolejki.clear();
	return res;
}