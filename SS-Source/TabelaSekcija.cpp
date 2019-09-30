#include "TabelaSekcija.h"
#include <fstream>
using namespace std;

TabelaSekcija::TabelaSekcija(){
	this->putSekcija(new Sekcija("UND", ""), "UND");
}
TabelaSekcija::~TabelaSekcija(){}

void TabelaSekcija::putSekcija(Sekcija* s, string key){
    Sekcija *pom = mapa[key];

    if(pom != 0){
        cout<<"Sekcija: "<<key<<" vec postoji! Naveliste u izvornom kodu vise puta isto ime sekcije"<<endl;
        Sekcija::srbr = Sekcija::srbr - 1;
        return;
    }
    mapa[key] = s;
}

Sekcija* TabelaSekcija::getSekcija(string key){
    Sekcija* s = mapa[key];

    if(s == 0){
        cout << "Ne postoji sekcija: "<< key << endl;
        return 0;
    }

    return s;
}


void TabelaSekcija::ispisiTabeluSekcija(ofstream& os) {
	map<string, Sekcija*>::iterator it;
	for (it = this->mapa.begin(); it != this->mapa.end(); it++) {
		Sekcija* s = it->second;
		string str = s->ispisiSekciju();
		os << str << endl;
	}
}
