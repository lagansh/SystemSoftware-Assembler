#ifndef TABELASIMBOLA_H
#define TABELASIMBOLA_H
#include <map>
#include <iostream>
#include <string>
using namespace std;
class Simbol{

    string ime;
    string sekcija;
    int rbr;
    bool global;
    int vrednost;

public:
	string ispisiSimbol() {
		return to_string(this->rbr) + "          \t\t" + this->ime + "           \t\t" + this->sekcija +  "         \t\t"  + to_string(this->vrednost) + "         \t\t" +to_string(this->global);
	}
    static int s_rbr;
    Simbol(string ime, string sek, bool glb, int vr): ime(ime), sekcija(sek), global(glb), vrednost(vr){
        this->rbr = ++s_rbr;
    }

    string getIme(){
        return this->ime;
    }

    string getSekcija(){
        return this->sekcija;
    }

    int getRbr(){
        return this->rbr;
    }

    bool isGlobal(){
        return this->global;
    }

    int getVrednost(){
        return this->vrednost;
    }

    void setLocalGlobal(bool glb){
        this->global = glb;
    }

    void setVrednost(int vr){
        this->vrednost = vr;
    }
	void setRbr(int r) {
		rbr = r;
	}

};

class TabelaSimbola{
    map<string, Simbol*> mapa;
public:
    TabelaSimbola();
    ~TabelaSimbola();

	map<string, Simbol*> getMap() {
		return mapa;
	}

	void ispisiTabelu(ofstream& os);

    void putSimbol(Simbol *s, string key);
    Simbol* getSimbol(string key);

};

#endif // TABELASIMBOLA_H
