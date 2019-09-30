#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include "TabelaSekcija.h"
#include "Asembler.h"
#include "TabelaSimbola.h"
#include "Naredbe.h"
#include "Instrukcija.h"
using namespace std;

int Instrukcija::prbr = 0;
int Sekcija::srbr = 0;
int Simbol::s_rbr = 0;

int main() {
    
	Asembler* as = new Asembler();
    
    ifstream instr;
    instr.open("C:\\Users\\Malesevic\\Desktop\\Djordje\\SS\\SSProjekat\\ss_projekat_testovi\\ss_projekat_testovi\\test2\\ulaz.txt");
    if(instr.is_open()){
		as->prviProlaz(instr);
		as->drugiProlaz(instr);
    }
    else{
        cout << "Greska pri citanju fajla ulaz.txt" << endl;
        exit(1);
    }
	instr.close();
		return 0;
}
