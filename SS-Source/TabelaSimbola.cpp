#include "TabelaSimbola.h"
#include <fstream>
TabelaSimbola::TabelaSimbola(){}

TabelaSimbola::~TabelaSimbola(){}

Simbol* TabelaSimbola::getSimbol(string key){
    Simbol* s = mapa[key];

    if(s == 0){
        return 0;
    }

    return s;
}

void TabelaSimbola::putSimbol(Simbol* s, string key){

    Simbol *pom = mapa[key];

    if(pom != 0){
        cout<<"Simbol vec postoji!"<<endl;
        return;
    }
    mapa[key] = s;
}

void TabelaSimbola::ispisiTabelu(ofstream& os) {
	map<string, Simbol*>::iterator it;
	for (it = this->mapa.begin(); it != this->mapa.end(); it++) {
		Simbol* s = it->second;
		os << s->ispisiSimbol() << endl;
	}
}
