#ifndef FUNCIONES MATEMATICAS_H
#define FUNCIONES MATEMATICAS_H
#include <vector>
#include <NTL/ZZ.h>
using namespace NTL;
using namespace std;
ZZ modulo(ZZ a, ZZ n);
ZZ euclides(ZZ a, ZZ b);
ZZ mcd(ZZ a, ZZ b);
vector <ZZ> euclides_extendido(ZZ a, ZZ b);
ZZ inversa(ZZ a, ZZ alfabeto);
ZZ potencia(ZZ a, ZZ b, ZZ modu);
#endif // FUNCIONES MATEMATICAS_H
