#ifndef FUNCIONES MATEMATICAS_H
#define FUNCIONES MATEMATICAS_H
#include <vector>
#include <sstream>
#include <NTL/ZZ.h>
using namespace NTL;
using namespace std;
ZZ modulo(ZZ a, ZZ n);
ZZ euclides(ZZ a, ZZ b);
ZZ mcd(ZZ a, ZZ b);
vector <ZZ> euclides_extendido(ZZ a, ZZ b);
ZZ inversa(ZZ a, ZZ alfabeto);
ZZ potencia(ZZ a, ZZ b, ZZ modu);
string to_string(ZZ x);
ZZ string_toZZ(string str);
#endif // FUNCIONES MATEMATICAS_H
