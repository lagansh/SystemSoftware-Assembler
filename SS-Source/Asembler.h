#ifndef ASEMBLER_H
#define ASEMBLER_H
#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include "TabelaSimbola.h"
#include "TabelaSekcija.h"
#include "Naredbe.h"
#include "Instrukcija.h"
class Asembler{
	friend class Naredba;
    map<string, char> sve_naredbe;
	
    long int loc_cnt;
    string trenutnaSekcija;
    void popuniMapuNaredbi();
	int instrukcije(string linija);
	
	
public:
	TabelaSimbola* tabelaSimbola;
	TabelaSekcija* tabelaSekcija;
	map<int, Instrukcija*> naredbe;
    enum Adresiranje {NEPOSREDNO = 0, REG_DIR, REG_INDIR, REG_INDIR_POM_8, REG_INDIR_POM_16, MEM, PC_REL };
    Asembler();
    ~Asembler();
	void ispisiNaredbe();
	int proveraAdresiranja(string* op, int* adr1, bool b);
    int prviProlaz(ifstream & fajl);
    int drugiProlaz(ifstream & fajl);
	void azurirajRedneBrojeveSimbola();
};

#endif // ASEMBLER_H_INCLUDED
