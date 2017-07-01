#ifndef FUNCIONES MATEMATICAS_H
#define FUNCIONES MATEMATICAS_H
#include "PRNG.h"
#include <string.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <NTL/ZZ.h>
using namespace NTL;
using namespace std;

ZZ getBase10(string bin);
string rotarizquierda(string num, int vueltas);
string volver_56bits(string bits);
string volver_48bits(string bits);
ZZ des(int bits);

ZZ modulo(ZZ a, ZZ n);
ZZ euclides(ZZ a, ZZ b);
vector <ZZ> euclides_extendido(ZZ a, ZZ b);
ZZ inversa(ZZ a, ZZ alfabeto);
vector <bool> ZZtoBinary(ZZ num);
ZZ potencia(ZZ a, ZZ b, ZZ n);
ZZ raiz_primitiva(ZZ p);
string to_string(ZZ x);
ZZ string_toZZ(string str);
int obtener_digitos(ZZ num);
#endif // FUNCIONES MATEMATICAS_H
