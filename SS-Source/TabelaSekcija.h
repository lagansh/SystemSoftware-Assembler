#ifndef TABELASEKCIJA_H
#define TABELASEKCIJA_H
#include <map>
#include <string>
#include <iostream>
using namespace std;
class Sekcija{
    string ime;
    int rbr;
    int pocetak;
    int kraj;
    int velicina;
    string pravaPristupa;
public:
	
	string ispisiSekciju() {
		return to_string(rbr) + "         \t\t" + ime + "     \t\t" + to_string(kraj) + "         \t\t" + pravaPristupa;
	}

    static int srbr;
    Sekcija(string i, string pp):ime(i), pravaPristupa(pp){
        rbr = ++srbr;
		pocetak = 0;
		kraj = 0;
    };
    void setKraj(int k){
        if(k > 0){
            kraj = k;
            velicina = kraj - pocetak;
        }
    }
    string getIme(){
        return this->ime;
    }
};


class TabelaSekcija{
    map<string, Sekcija*> mapa;
public:
    TabelaSekcija();
    ~TabelaSekcija();
	void ispisiTabeluSekcija(ofstream& os);
    void putSekcija(Sekcija *s, string key);
    Sekcija* getSekcija(string key);
};

#endif // TABELASEKCIJA_H_INCLUDED
