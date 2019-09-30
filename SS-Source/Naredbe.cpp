#include "Naredbe.h"
#include "Asembler.h"
#include "TabelaSimbola.h"
#include <string>
#include <sstream>
#include <iostream>
#include <regex>
using namespace std;

void Naredba::updateOp1Im(int adr_op, int velicina, string* op, TabelaSimbola* ts, char* opIm1, char* opIm2) {
	smatch pom;
	regex imm1("^-?[0-9]+$");
	regex imm2("^&[a-zA-Z]+$");

	regex regindpom1("^(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])\\[-?\\d+\\]$");
	regex regindpom2("^(r[0-7]([lL]|[hH])?|[pP][sS][wW]|[pP][cC]|[sS][pP])\\[[a-zA-Z]+\\]$");
	regex pcrel("^\\$[a-zA-Z]+$");
	regex memdir1("^[a-zA-Z]+$");
	regex memdir2("^\\*\\d+$");

	int vrednost = -99999;

	if (adr_op == Asembler::NEPOSREDNO || adr_op == Asembler::MEM || adr_op == Asembler::REG_INDIR_POM_16 || adr_op == Asembler::REG_INDIR_POM_8) {

		regex_search(*op, pom, imm1);
		if (!pom.empty()) {
			stringstream ss(pom.str());
			int off = 0;
			ss >> off;
			vrednost = off;
		}

		regex_search(*op, pom, pcrel);
		if (!pom.empty()) {
			regex imm2s("[a-zA-Z]+");
			regex_search(*op, pom, imm2s);
			if (!pom.empty()) {
				Simbol* s = ts->getSimbol(pom.str());
				if (s == 0) {
					cout << "Ne postoji simbol " << pom.str() << endl;
					exit(1);
				}
				else {
					if (s->isGlobal())
						vrednost = 0;
					else
						vrednost = s->getVrednost();
				}
			}
			else {
				cout << "Greska u funkciji updateOp1Im1 u if grani Neposrednog adresiranja" << endl;
				exit(1);
			}
		}

		regex_search(*op, pom, regindpom2);
		if (!pom.empty()) {
			regex r("\\[[a-zA-Z]+\\]$");
			regex_search(*op, pom, r);
				if (!pom.empty()) {
				string l = pom.str();
				regex ri("[a-zA-Z]+");
				regex_search(l, pom, ri);
				if (!pom.empty()) {
					Simbol* s = ts->getSimbol(pom.str());
					if (s == 0) {
						cout << "Ne postoji simbol " << pom.str() << endl;
						exit(1);
					}
					else {
						if (s->isGlobal())
							vrednost = 0;
						else
							vrednost = s->getVrednost();
					}
				}
			}
			else {
				cout << "Greska! U funkciji Update*opIm" << endl;
				exit(1);
			}
		}

		regex_search(*op, pom, regindpom1);
		if(!pom.empty()){
			regex r("\\[-?\\d+\\]$");
			regex_search(*op, pom, r);
			if (!pom.empty()) {
				string l = pom.str();
				regex ri("-?[0-9]+");
				regex_search(l, pom, ri);
				if (!pom.empty()) {
					stringstream ss(pom.str());
					int off = 0;
					ss >> off;
					vrednost = off;
				}
				else {
					cout << "Greska! U funkciji UpdateOp1Im" << endl;
					exit(1);
				}
			}
		}

		regex_search(*op, pom, memdir2);
		if (!pom.empty()) {
			regex r("[0-9]+$");
			regex_search(*op, pom, r);
			if (!pom.empty()) {
				stringstream ss(pom.str());
				int off = 0;
				ss >> off;
				vrednost = off;
			}
			else {
				cout << "Greska! U funkciji Update*opIm" << endl;
				exit(1);
			}
		}

		regex_search(*op, pom, memdir1);
		if (!pom.empty()) {
			Simbol* s = ts->getSimbol(pom.str());
			if (s == 0) {
				cout << "Ne postoji simbol " << pom.str() << endl;
				exit(1);
			}
			else {
				if (s->isGlobal())
					vrednost = 0;
				else
					vrednost = s->getVrednost();
			}
		}

		regex_search(*op, pom, imm2);
		if (!pom.empty()) {
			regex imm2s("[a-zA-Z]+");
			regex_search(*op, pom, imm2s);
			if (!pom.empty()) {
				Simbol* s = ts->getSimbol(pom.str());
				if (s == 0) {
					cout << "Ne postoji simbol " << pom.str() << endl;
					exit(1);
				}
				else {
					if (s->isGlobal())
						vrednost = 0;
					else
						vrednost = s->getVrednost();
				}
			}
			else {
				cout << "Greska u funkciji updateOp1Im1 u if grani Neposrednog adresiranja" << endl;
				exit(1);
			}
		}
	}

	if (vrednost != -99999) {
		if (velicina == 2) {
			*opIm1 = vrednost;
		}
		else if (velicina == 3) {
			int vrcpy = vrednost;
			vrcpy = vrcpy << 16;
			vrcpy = vrcpy >> 24;
			vrednost = vrednost << 24;
			vrednost = vrednost >> 24;
			*opIm1 = vrednost;
			*opIm2 = vrcpy;
		}
		else {
			cout << "GRESKA!!! Funkcija : updateOp1Im1();" << endl;
			exit(1);
		}
	}
}

void Naredba::updateOp1Descr() {
	if (this->adr_op1 == -1)
		return;
	this->Op1Descr = adr_op1 << 5;
	if (adr_op1 == Asembler::REG_DIR || adr_op1 == Asembler::REG_INDIR || adr_op1 == Asembler::REG_INDIR_POM_16 || adr_op1 == Asembler::REG_INDIR_POM_8) {

		stringstream ss;
		int v = 0;
		smatch pom;
		smatch pompc;
		smatch pomsp;
		smatch pompsw;
		smatch pompcrel;

		regex pc("[pP][cC]");
		regex psw("[pP][sS][wW]");
		regex sp("[sS][pP]");
		regex pcrel("^\\$[a-zA-Z]+$");
		regex a("[\\d]");

		regex_search(op1, pompc, pc);
		regex_search(op1, pompsw, psw);
		regex_search(op1, pomsp, sp);
		regex_search(op1, pom, a);

		regex_search(op1, pompcrel, pcrel);
		if (!pompcrel.empty()) {
			v = 7;
			v = v << 1;

			this->Op1Descr = this->Op1Descr | (char)v | 0;
		}
		else {
			if (!pompc.empty()) {
				v = 7;
				v = v << 1;
			}
			else if (!pomsp.empty()) {
				v = 6;
				v = v << 1;
			}
			else if (!pompsw.empty()) {
				v = 5;
				v = v << 1;
			}
			else if (!pom.empty()) {
				ss.str(pom.str());
				ss >> v;
				v = v << 1;
			}

			regex lhpp("r[0-9]([lL]|[hH])");
			regex lh("([lL]|[hH])");
			char lowhigh = 0;

			regex_search(op1, pom, lhpp);
			if (!pom.empty()) {
				string hlp = pom.str();
				regex_search(hlp, pom, lh);
				if (!pom.empty()) {
					if (pom.str() == "h" || pom.str() == "H")
						lowhigh = 1;
				}
			}

			this->Op1Descr = this->Op1Descr | (char)v | lowhigh;
		}
	}
}

void Naredba::updateOp2Descr() {
	if (this->adr_op2 == -1)
		return;
	this->Op2Descr = adr_op2 << 5;
	if (adr_op2 == Asembler::REG_DIR || adr_op2 == Asembler::REG_INDIR || adr_op2 == Asembler::REG_INDIR_POM_16 || adr_op2 == Asembler::REG_INDIR_POM_8) {

		stringstream ss;
		int v = 0;
		smatch pom;
		smatch pompc;
		smatch pomsp;
		smatch pompsw;
		smatch pompcrel;

		regex pc("[pP][cC]");
		regex psw("[pP][sS][wW]");
		regex sp("[sS][pP]");
		regex pcrel("^\\$[a-zA-Z]+$");
		regex a("[\\d]");

		regex_search(op2, pompc, pc);
		regex_search(op2, pompsw, psw);
		regex_search(op2, pomsp, sp);
		regex_search(op2, pom, a);

		regex_search(op1, pompcrel, pcrel);
		if (!pompcrel.empty()) {
			v = 7;
			v = v << 1;

			this->Op1Descr = this->Op1Descr | (char)v | 0;
		}
		else {
			if (!pompc.empty()) {
				v = 7;
				v = v << 1;
			}
			else if (!pomsp.empty()) {
				v = 6;
				v = v << 1;
			}
			else if (!pompsw.empty()) {
				v = 5;
				v = v << 1;
			}
			else if (!pom.empty()) {
				ss.str(pom.str());
				ss >> v;
				v = v << 1;
			}

			regex lhpp("r[0-9]([lL]|[hH])");
			regex lh("([lL]|[hH])");
			char lowhigh = 0;

			regex_search(op1, pom, lhpp);
			if (!pom.empty()) {
				string hlp = pom.str();
				regex_search(hlp, pom, lh);
				if (!pom.empty()) {
					if (pom.str() == "h" || pom.str() == "H")
						lowhigh = 1;
				}
			}
			this->Op2Descr = this->Op2Descr | (char)v | lowhigh;
		}
	}
}