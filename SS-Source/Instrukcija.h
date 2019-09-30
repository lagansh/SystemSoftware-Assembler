#ifndef INSTRUKCIJA_H
#define INSTRUKCIJA_H
#include <string>

using namespace std;

class Instrukcija {
public:
	static int prbr;
	virtual string ispisiNaredbu() = 0;
};


#endif