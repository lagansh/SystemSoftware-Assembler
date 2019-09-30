#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include "Instrukcija.h"
#include "Direktiva.h"
#include "Asembler.h"


Asembler::Asembler() {
    this->loc_cnt = 0;
    this->tabelaSekcija = new TabelaSekcija();
    this->tabelaSimbola = new TabelaSimbola();
    this->trenutnaSekcija = "UND";
    this->popuniMapuNaredbi();
}
void Asembler::popuniMapuNaredbi(){
    sve_naredbe["halt"] = 0x08;
    sve_naredbe["xchg"] = 0x14;
    sve_naredbe["xchgb"] = 0x10;
    sve_naredbe["xchgw"] = 0x14;
    sve_naredbe["int"] = 0x1C;
    sve_naredbe["intb"] = 0x18;
    sve_naredbe["intw"] = 0x1C;
    sve_naredbe["mov"] = 0x24;
    sve_naredbe["movb"] = 0x20;
    sve_naredbe["movw"] = 0x24;
    sve_naredbe["add"] = 0x2C;
    sve_naredbe["addb"] = 0x28;
    sve_naredbe["addw"] = 0x2C;
    sve_naredbe["sub"] = 0x34;
    sve_naredbe["subb"] = 0x30;
    sve_naredbe["subw"] = 0x34;
    sve_naredbe["mul"] = 0x3C;
    sve_naredbe["mulb"] = 0x38;
    sve_naredbe["mulw"] = 0x3C;
    sve_naredbe["div"] = 0x44;
    sve_naredbe["divb"] = 0x40;
    sve_naredbe["divw"] = 0x44;
    sve_naredbe["cmp"] = 0x4C;
    sve_naredbe["cmpb"] = 0x48;
    sve_naredbe["cmpw"] = 0x4C;
    sve_naredbe["not"] = 0x54;
    sve_naredbe["notb"] = 0x50;
    sve_naredbe["notw"] = 0x54;
    sve_naredbe["and"] = 0x5C;
    sve_naredbe["andb"] = 0x58;
    sve_naredbe["andw"] = 0x5C;
    sve_naredbe["or"] = 0x64;
    sve_naredbe["orb"] = 0x60;
    sve_naredbe["orw"] = 0x64;
    sve_naredbe["xor"] = 0x6C;
    sve_naredbe["xorb"] = 0x68;
    sve_naredbe["xorw"] = 0x6C;
    sve_naredbe["test"] = 0x74;
    sve_naredbe["testb"] = 0x70;
    sve_naredbe["testw"] = 0x74;
    sve_naredbe["shl"] = 0x7C;
    sve_naredbe["shlb"] = 0x78;
    sve_naredbe["shlw"] = 0x7C;
    sve_naredbe["shr"] = 0x84;
    sve_naredbe["shrb"] = 0x80;
    sve_naredbe["shrw"] = 0x84;
    sve_naredbe["push"] = 0x8C;
    sve_naredbe["pushb"] = 0x88;
    sve_naredbe["pushw"] = 0x8C;
    sve_naredbe["pop"] = 0x94;
    sve_naredbe["popb"] = 0x90;
    sve_naredbe["popw"] = 0x94;
    sve_naredbe["jmp"] = 0x9C;
    sve_naredbe["jmpb"] = 0x98;
    sve_naredbe["jmpw"] = 0x9C;
    sve_naredbe["jeq"] = 0xA4;
    sve_naredbe["jeqb"] = 0xA0;
    sve_naredbe["jeqw"] = 0xA4;
    sve_naredbe["jne"] = 0xAC;
    sve_naredbe["jneb"] = 0xA8;
    sve_naredbe["jnew"] = 0xAC;
    sve_naredbe["jgt"] = 0xB4;
    sve_naredbe["jgtb"] = 0xB0;
    sve_naredbe["jgtw"] = 0xB4;
    sve_naredbe["call"] = 0xBC;
    sve_naredbe["callb"] = 0xB8;
    sve_naredbe["callw"] = 0xBC;
    sve_naredbe["ret"] = 0xC0;
    sve_naredbe["iret"] = 0xC8;
}

 int Asembler::proveraAdresiranja(string* op, int* adr, bool b) {
	
	 regex imm1("^-?[0-9]+$");
	 regex imm1h("^0[xX][0-9a-fA-F]+$");
	 regex imm2("^&[a-zA-Z]+$");
	 regex regdir("^(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])$");
	 regex regindpom1("^(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])\\[-?\\d+\\]$");
	 regex regindpom2("^(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])\\[[a-zA-Z]+\\]$");
	 regex regind("^\\[(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])\\]$");
	 regex pcrel("^\\$[a-zA-Z]+$");
	 regex memdir1("^[a-zA-Z]+$");
	 regex memdir2("^\\*\\d+$");

	 smatch pom;


	 if (*op == "") {
		 return 0;
	 }

	 regex_search(*op, pom, regdir);
	 if (!pom.empty()) {
		 *adr = Asembler::REG_DIR;
		 return 1;
	 }

	 regex_search(*op, pom, imm1);
	 if (!pom.empty()) {
		 *adr = Asembler::NEPOSREDNO;
		 if (b)
			 return 2;
		 else
			 return 3;
	 }
	 regex_search(*op, pom, imm1h);
	 if(!pom.empty()){
		 *adr = Asembler::NEPOSREDNO;
		 if (b)
			 return 2;
		 else
			 return 3;
	 }
	 regex_search(*op, pom, imm2);
	 if (!pom.empty()) {
		 *adr = Asembler::NEPOSREDNO;
		 if (b)
			 return 2;
		 else
			 return 3;
		 /*
		 regex r("[a-zA-Z]+");
		 regex_search(op, pom, r);
		 if (!pom.empty()) {
			 string st = pom.str();
			 int v = this->tabelaSimbola->getSimbol(st)->getVrednost();
			 if (v < 127 && v > -128) {
				 return 2;
			 }
			 else if (v < 32767 && v > -32768) {
				 return 3;
			 }
			 else {
				 cout << "Operand " << op << " je veci od maksimalnih 2 bajta!" << endl;
				 exit(1);
			 }
		 }
		 else {
			 cout <<"NE POSTOJI ADRESIRANJE " << op << endl;
			 exit(1);
		 }
		 */
	 }
	 
	 regex_search(*op, pom, regindpom1);
	 if (!pom.empty()) {
		 if (b) {
			 *adr = Asembler::REG_INDIR_POM_8;
			 return 2;
		 }
		 else {
			 *adr = Asembler::REG_INDIR_POM_16;
			 return 3;
		 }
		 /*
		 regex m("\\[\\d+\\]");
		 regex_search(op, pom, m);
		 if (!pom.empty()) {
			 regex n("\\d+");
			 string opr = pom.str();
			 regex_search(opr, pom, n);
			 if (!pom.empty()) {
				 opr = pom.str();
				 int vr;
				 stringstream ss(opr);
				 ss >> vr;
				 if (vr < 127 && vr > -128) {
					 *adr = Asembler::REG_INDIR_POM_8;
					 return 2;
				 }
				 else if (vr < 32767 && vr > -32768) {
					 *adr = Asembler::REG_INDIR_POM_16;
					 return 3;
				 }
				 else {
					 cout << "Operand " << op << " je veci od maksimalnih 2 bajta!" << endl;
					 exit(1);
				 }
			 }
			 else {
				 cout << "NE POSTOJI ADRESIRANJE " << op << endl;
				 exit(1);
			 }
		 }
		 else {
			 cout << "NE POSTOJI ADRESIRANJE " << op << endl;
			 exit(1);
		 }
		 */
	 }
	 regex_search(*op, pom, regindpom2);
	 if (!pom.empty()) {
		 if (b) {
			 *adr = Asembler::REG_INDIR_POM_8;
			 return 2;
		 }
		 else {
			 *adr = Asembler::REG_INDIR_POM_16;
			 return 3;
		 }
		 /*
		 regex m("\\[[a-zA-Z]+\\]");
		 regex_search(op, pom, m);
		 if (!pom.empty()) {
			 regex n("[a-zA-Z]+");
			 string opr = pom.str();
			 regex_search(opr, pom, n);
			 if (!pom.empty()) {
				 opr = pom.str();
				 if (this->tabelaSimbola->getSimbol(opr) == 0) {
					 cout << "NE POSTOJI SIMBOL " << opr << " U TABELI SIMBOLA!" << endl;
					 exit(1);
				 }
				 int v = this->tabelaSimbola->getSimbol(opr)->getVrednost();
				 if (v < 127 && v > -128) {
					 *adr = Asembler::REG_INDIR_POM_8;
					 return 2;
				 }
				 else if (v < 32767 && v > -32768) {
					 *adr = Asembler::REG_INDIR_POM_16;
					 return 3;
				 }
				 else {
					 cout << "NE POSTOJI ADRESIRANJE " << op << endl;
					 exit(1);
				 }
			 }
			 else {
				 cout << "NE POSTOJI ADRESIRANJE " << op << endl;
				 exit(1);
			 }
		 }
		 else {
			 cout << "NE POSTOJI ADRESIRANJE " << op << endl;
			 exit(1);
		 }
		 */
	 }
	 regex_search(*op, pom, regind);
	 if (!pom.empty()) {
		 *adr = Asembler::REG_INDIR;
		 return 1;
	 }
	 regex_search(*op, pom, pcrel);
	 if (!pom.empty()) {
		 
		 if (!b) {
			 *adr = Asembler::REG_INDIR_POM_16;
			 return 3;
		 }
		 else {
			 *adr = Asembler::REG_INDIR_POM_8;
			 return 2;
		 }
	 }
	 regex_search(*op, pom, memdir1);
	 if (!pom.empty()) {
		 *adr = Asembler::MEM;
		 return 3;
	 }
	 regex_search(*op, pom, memdir2);
	 if (!pom.empty()) {
		 *adr = Asembler::MEM;
		 return 3;
	 }

	 cout << "Ne postoji adresiranje " << endl;
	 exit(1);
 }

 int Asembler::instrukcije(string linija) {

	 int lc = 0;

	 smatch pom;
	 regex r("[a-zA-Z]{2,5}");
	 regex_search(linija, pom, r);
	 if (!pom.empty()) {

		 string mn = pom.str();
		 if (this->sve_naredbe[mn] == 0) {
			 cout << "Ne postoji naredba " << mn << endl;
			 exit(1);
		 }
		 int adr1 = -1;
		 int adr2 = -1;

		 bool ima_b = false;
		 linija = pom.suffix().str();


		 regex b("b$");
		 regex_search(mn, pom, b);
		 if (!pom.empty())
			 ima_b = true;
		 lc = 1;

		 if (mn == "sub" && ima_b)
			 ima_b = false;

		 char* oprs = new char[linija.length() + 1];
		 strcpy(oprs, linija.c_str());
		 char* stok;

		 string op1 = "", op2 = "";
		 int v1 = -1, v2 = -1;
		 int i = 0;

		 stok = strtok(oprs, " ,");
		 while (stok != NULL) {
			 if (i == 0) {
				 op1 = stok;
				 lc += (v1 = this->proveraAdresiranja(&op1, &adr1, ima_b));
			 }
			 else if (i == 1) {
				 op2 = stok;
				 lc += (v2 = this->proveraAdresiranja(&op2, &adr2, ima_b));
			 }
			 else {
				 cout << "Instrukcija " << pom.prefix().str() << " ima previse operanada!" << endl;
				 exit(1);
			 }
			 i++;
			 stok = strtok(NULL, " ,");
		 }
		 Naredba* n = new Naredba(mn, this->sve_naredbe[mn], op1, op2, adr1, adr2);
		 this->naredbe[n->getRbr()] = n;
		 Naredba* a = (Naredba*) this->naredbe[n->getRbr()];
		 a->setVelicina1(v1);
		 a->setVelicina2(v2);
		 return lc;
	 }
	 return 0;
 }

 int Asembler::drugiProlaz(ifstream& fajl) {
	 string linija;
	 if (fajl.is_open()) {
		 while (getline(fajl, linija)) {
			 smatch pom;
			 regex glbl(".global");
			 regex_search(linija, pom, glbl);
			 if (!pom.empty()) {
				 linija = pom.suffix().str();
				 char* clin = new char[linija.length() + 1];
				 strcpy(clin, linija.c_str());
				 char* stok;
				 Simbol* s;

				 stok = strtok(clin, " ,");
				 while (stok != NULL) {
					 if ((s = this->tabelaSimbola->getSimbol(stok)) != 0)
						 s->setLocalGlobal(true);
					 else
						 this->tabelaSimbola->putSimbol(new Simbol(stok, this->trenutnaSekcija, true, -1), stok);
					 stok = strtok(NULL, " ,");
				 }
				 continue;
			 }
		 }

		 ofstream fajlOut;
		 fajlOut.open("C:\\Users\\Malesevic\\Desktop\\Djordje\\SS\\SSProjekat\\ss_projekat_testovi\\ss_projekat_testovi\\test2\\IzlazDjordje.txt");
		 if (!fajlOut.is_open()) {
			 cout << "Fajl ne moze da se otvori (Output fajl - drugi prolaz)" << endl;
			 exit(1);
		 }

		 fajlOut << "\t\t\tTABELA SEKCIJA" << endl << endl;
		 fajlOut << "Redni br.\t\tIme\t\tVelicina\t\tPrava pristupa" << endl;
		 tabelaSekcija->ispisiTabeluSekcija(fajlOut);

		 fajlOut << endl << endl;

		 fajlOut << "\t\t\tTABELA SIMBOLA" << endl << endl;
		 fajlOut << "Redni br.\t\tIme\t\tSekcija\t\tVrednost\t\tLocal|Global" << endl;
		 tabelaSimbola->ispisiTabelu(fajlOut);

		 fajlOut << "\t\t\tIZLAZNI KOD" << "nazivSekcije"<< " SEKCIJE" <<endl << endl;

		



		 fajlOut.close();
	 }
 }

int Asembler::prviProlaz(ifstream& fajl) {
    string linija;
    if (fajl.is_open()) {
        while (getline(fajl, linija)) {
			
			smatch pom;

			regex lbl("[a-zA-Z0-9]+:");
			regex_search(linija, pom, lbl);
			if (!pom.empty()) {
				string hlp = pom.str();
				
				linija = pom.suffix().str();

				char* clin = new char[hlp.length() + 1];
				strcpy(clin, hlp.c_str());
				char* stok;
				stok = strtok(clin, " :");
				if (stok != NULL) {
					Simbol* s;
					if ((s = this->tabelaSimbola->getSimbol(stok)) != 0) {
						if (!s->isGlobal())
							s->setVrednost(this->loc_cnt);
					}
					else
						this->tabelaSimbola->putSimbol(new Simbol(stok, this->trenutnaSekcija, false, this->loc_cnt), stok);
				}
				else
					exit(1);
			}

            regex end(".end");
            regex_search(linija, pom, end);
			if (!pom.empty()) {
				this->tabelaSekcija->getSekcija(this->trenutnaSekcija)->setKraj(this->loc_cnt);
				break;
			}


            regex textSek(".text");
            regex_search(linija, pom, textSek);
            if (!pom.empty()) {
				if (this->trenutnaSekcija != "UND") {
					if (this->tabelaSekcija->getSekcija(this->trenutnaSekcija) != 0)
						this->tabelaSekcija->getSekcija(this->trenutnaSekcija)->setKraj(this->loc_cnt);
					else {
						cout << "Ne postoji sekcija u tabeli" << endl;
						exit(1);
					}
				}
                this->trenutnaSekcija = "text";
                tabelaSekcija->putSekcija(new Sekcija(this->trenutnaSekcija, "rwx"), this->trenutnaSekcija);
				this->loc_cnt = 0;
                continue;
            }
            regex dataSek(".data");
            regex_search(linija, pom, dataSek);
            if (!pom.empty()) {
				if (this->trenutnaSekcija != "UND") {
					if (this->tabelaSekcija->getSekcija(this->trenutnaSekcija) != 0)
						this->tabelaSekcija->getSekcija(this->trenutnaSekcija)->setKraj(this->loc_cnt);
					else {
						cout << "Ne postoji sekcija u tabeli" << endl;
						exit(1);
					}
				}
                this->trenutnaSekcija = "data";
                tabelaSekcija->putSekcija(new Sekcija(this->trenutnaSekcija, "rw"), this->trenutnaSekcija);
				this->loc_cnt = 0;
                continue;
            }

            regex bssSek(".bss");
            regex_search(linija, pom, bssSek);
            if (!pom.empty()) {
				if (this->trenutnaSekcija != "UND") {
					if (this->tabelaSekcija->getSekcija(this->trenutnaSekcija) != 0)
						this->tabelaSekcija->getSekcija(this->trenutnaSekcija)->setKraj(this->loc_cnt);
					else {
						cout << "Ne postoji sekcija u tabeli" << endl;
						exit(1);
					}
				}
                this->trenutnaSekcija = "bss";
                tabelaSekcija->putSekcija(new Sekcija(this->trenutnaSekcija, "rw"), this->trenutnaSekcija);
				this->loc_cnt = 0;
                continue;
            }

            regex cusSek(".section");
            regex_search(linija, pom, cusSek);
            if (!pom.empty()) {
				if (this->trenutnaSekcija != "UND") {
					if (this->tabelaSekcija->getSekcija(this->trenutnaSekcija) != 0)
						this->tabelaSekcija->getSekcija(this->trenutnaSekcija)->setKraj(this->loc_cnt);
					else {
						cout << "Ne postoji sekcija u tabeli" << endl;
						exit(1);
					}
				}
                linija = pom.suffix().str();
                regex sekName("[a-zA-Z0-9]+");
                regex_search(linija, pom, sekName);
                if (!pom.empty()) {
                    string novaSek = pom.str();
                    string pravaPristupa = "rw";
                    linija = pom.suffix().str();
                    regex zarez(",");
                    regex_search(linija, pom, zarez);
                    if (!pom.empty()) {
                        regex pravapris("[a-zA-Z]{1,3}");
                        regex_search(linija, pom, pravapris);
                        if (!pom.empty()) {
                            pravaPristupa = pom.str();
                            tabelaSekcija->putSekcija(new Sekcija(novaSek, pravaPristupa), novaSek);
                            this->trenutnaSekcija = novaSek;
                            continue;
                        } else {
                            cout << "Nepravilna prava pristupa za .section naredbu " << endl;
                            exit(1);
                        }

                    } else {
                        tabelaSekcija->putSekcija(new Sekcija(novaSek, pravaPristupa), novaSek);
                        this->trenutnaSekcija = novaSek;
						this->loc_cnt = 0;
                        continue;
                    }
                } else {
                    cout << "Nekorektna instrukcija .section ..." << endl;
                    exit(1);
                }
            }

            regex skip(".skip");
            regex_search(linija, pom, skip);
            if (!pom.empty()) {
                regex skip_bajtovi("[0-9]+");
				Direktiva* d = new Direktiva(pom.str());
                linija = pom.suffix().str();
                regex_search(linija, pom, skip_bajtovi);
                if (!pom.empty()) {
                    stringstream ss(pom.str());
                    int off = 0;
                    ss >> off;
					for(int i = 0; i < off; i++)
						d->dodaj(0);
                    this->loc_cnt += off;
					this->naredbe[d->getRbr()] = d;
                    continue;
                } else {
                    cout << "Nepravilna instrukcija .skip, nedostaje broj bajtova" << endl;
                    exit(1);
                }

            }

            regex byt(".byte");
            regex_search(linija, pom, byt);
            if (!pom.empty()) {
				Direktiva* d = new Direktiva(pom.str());
                linija = pom.suffix().str();
				char* clin = new char[linija.length() + 1];
				strcpy(clin, linija.c_str());
				char* stok;
				int i = 0;

				stok = strtok(clin, " ,");
				while (stok != NULL) {
					++i;
					stringstream ss(stok);
					int off = -999999;
					ss >> off;
					if (stok != "0" && off == 0) {
						d->simboli.push_back(stok);
					}
					else
						d->dodaj(off);
					stok = strtok(NULL, " ,");
				}
				if (i == 0) {
					cout << "Problem sa instrukcijom .byte" << endl;
					exit(1);
				}
				this->naredbe[d->getRbr()] = d;
                this->loc_cnt += i;
                continue;
            }
            
            regex wrd(".word");
            regex_search(linija, pom, wrd);
            if (!pom.empty()) {
				Direktiva* d = new Direktiva(pom.str());
				linija = pom.suffix().str();
				char* clin = new char[linija.length() + 1];
				strcpy(clin, linija.c_str());
				char* stok;
				int i = 0;

				stok = strtok(clin, " ,");
				while (stok != NULL) {
					++i;
					stringstream ss(stok);
					int off = -999999;
					ss >> off;
					if (stok != "0" && off == 0) {
						d->simboli.push_back(stok);
					}
					else
						d->dodaj(off);
					stok = strtok(NULL, " ,");
				}
				if (i == 0) {
					cout << "Problem sa instrukcijom .word" << endl;
					exit(1);
				}
				this->naredbe[d->getRbr()] = d;
                this->loc_cnt += i*2;
                continue;
            }

			regex extrn(".extern");
			regex_search(linija, pom, extrn);
			if (!pom.empty()) {
				linija = pom.suffix().str();
				char* clin = new char[linija.length() + 1];
				strcpy(clin, linija.c_str());
				char* stok;
				Simbol* s;

				stok = strtok(clin, " ,");
				while (stok != NULL) {
					if ((s = this->tabelaSimbola->getSimbol(stok)) != 0) {
						cout << stok << " - promenljiva je definisana kao extern i kao labela, no no" << endl;
						exit(1);
					}
					else
						this->tabelaSimbola->putSimbol(new Simbol(stok, "UND" , true, -9999), stok);
					stok = strtok(NULL, " ,");
				}
				continue;
			}

			regex glbl(".global");
			regex_search(linija, pom, glbl);
			if (!pom.empty()) {
			/*	linija = pom.suffix().str();
				char* clin = new char[linija.length() + 1];
				strcpy(clin, linija.c_str());
				char* stok;
				Simbol* s;

				stok = strtok(clin, " ,");
				while (stok != NULL) {
					if ((s = this->tabelaSimbola->getSimbol(stok)) != 0) 
						s->setLocalGlobal(true);
					else
						this->tabelaSimbola->putSimbol(new Simbol(stok, this->trenutnaSekcija, true, -1), stok);
					stok = strtok(NULL, " ,");
				} */
				continue;
			}

			regex equ(".equ");
			regex_search(linija, pom, equ);
			if (!pom.empty()) {
				int i = 0;
				linija = pom.suffix().str();

				char* clin = new char[linija.length() + 1];
				strcpy(clin, linija.c_str());
				char* stok;
				Simbol* s;
				
				string prom;

				stok = strtok(clin, " ,");
				regex chr("[a-zA-Z]+");
				while (stok != NULL) {
					if (i == 0) {
						prom = stok;
						regex_search(prom, pom, chr);
						if (!pom.empty()) {
							if ((s = this->tabelaSimbola->getSimbol(stok)) == 0) {
								this->tabelaSimbola->putSimbol(new Simbol(stok, this->trenutnaSekcija, false, -1), stok);
							}
							else {
								cout << "Simbol " << stok << " je vec ranije definisan! GRESKA!" << endl;
								//exit(1);
							}

						}
						else {
							cout << "Greska u naredbi equ (prvi parametar mora biti promenljiva ne konstanta)" << endl;
							exit(1);
						}
						i++;
					}
					else if(i == 1) {
						string m = stok;
						regex num("-?[0-9]+");
						regex_search(m, pom, num);
						if (!pom.empty()) {
							int vr = 0;
							stringstream ss(stok);
							ss >> vr;
							this->tabelaSimbola->getSimbol(prom)->setVrednost(vr);
						}
						else {
							cout << "Greska u naredbi equ (drugi parametar mora biti broj ne literal!)" << endl;
							exit(1);
						}
						i++;
					}
					else {
						cout << "Nekorektna instrukcija .equ (previse argumenata)" << endl;
						exit(1);
					}
					stok = strtok(NULL, " ,");
				}
				continue;
			}

			regex align(".align");
			regex_search(linija, pom, align);
			if (!pom.empty()) {
				linija = pom.suffix().str();
				regex broj("[0-9]+");
				regex_search(linija, pom, broj);
				if (!pom.empty()) {
					string br = pom.str();
					int brr = atoi(br.c_str());
					float logaritam = log2(brr);
					if (floor(logaritam) == logaritam) { //ako je log2 ceo broj onda jeste stepen dvojke
						float n = (float)loc_cnt / (float)brr;
						int c = ceil(n);  //gornji ceo deo
						loc_cnt = (c * brr);
						continue;
					}
					else {
						cout << "Greska:broj nije stepen dvojke" << endl;
					}
				}
				else {
					cout << "Greska, nakon .align nema broja" << endl;
					exit(1);
				}
			}
			this->loc_cnt += this->instrukcije(linija);

        }
    } else {
        cout << "NO NO" << endl;
    }
	this->azurirajRedneBrojeveSimbola();

	map<int, Instrukcija*>::iterator it;
	for (it = this->naredbe.begin(); it != this->naredbe.end(); it++) {
		Instrukcija* s = it->second;
		if (Naredba * n = dynamic_cast<Naredba*>(s)) {
			n->updateOp1Im(n->getAdrOp1(), n->getVelicina1(), n->getOp1p(), tabelaSimbola, n->getOp1Im1(), n->getOp1Im2());
			n->updateOp1Im(n->getAdrOp2(), n->getVelicina2(), n->getOp2p(), tabelaSimbola, n->getOp2Im1(), n->getOp2Im2());
		}
		else if(Direktiva* d = dynamic_cast<Direktiva*>(s)) {

		}
	}

	this->ispisiNaredbe();

}

void Asembler::ispisiNaredbe() {
	cout << "...................NAREDBE....................." << endl;
	map<int, Instrukcija*>::iterator it;
	for (it = this->naredbe.begin(); it != this->naredbe.end(); it++) {
		Instrukcija* s = it->second;
		cout << s->ispisiNaredbu() << endl;
	}
}

void Asembler::azurirajRedneBrojeveSimbola() {
	map<string, Simbol*> mapa = this->tabelaSimbola->getMap();
	map<string, Simbol*>::iterator it;
	for (it = mapa.begin(); it != mapa.end(); it++) {
		Simbol* s = it->second;
		int nv = s->getRbr() + Sekcija::srbr;
		s->setRbr(nv);
	}
}