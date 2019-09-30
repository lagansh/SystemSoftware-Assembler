#ifndef DIREKTIVA_H
#define DIREKTIVA_H
#include <string>
#include <vector>
#include "Instrukcija.h"
using namespace std;

class Direktiva :public Instrukcija{
public:
	int getRbr() {
		return rbr;
	}
	string mnemonik;
	vector<int> parametri;
	vector<string> simboli;
	Direktiva(string mn) :mnemonik(mn) {
		rbr = ++Instrukcija::prbr;
	};
	void dodaj(int br) {
		parametri.push_back(br);
	}
	string ispisiNaredbu() {
		return this->mnemonik;
	}
	int getVelicina() {
		if(mnemonik == ".word")
			return 2 * (parametri.size() + simboli.size());
		else
			return parametri.size() + simboli.size();
	}
private:
	int rbr;
};


#endif
