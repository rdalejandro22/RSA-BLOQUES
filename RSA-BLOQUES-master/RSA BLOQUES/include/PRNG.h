#ifndef PRNG_H
#define PRNG_H
#include <vector>
#include <NTL/ZZ.h>
#include "Funciones Matematicas.h"
using namespace std;
using namespace NTL;
ZZ convertir_decimal(std::vector <bool> a, int bits_num);
void rotar_derecha(std::vector <bool> &vec, int indice, int elementos, int vueltas);
void rotar_izquierda(std::vector <bool> &vec, int indice, int elementos, int vueltas);
vector <bool> ga2(int bits_seed, int bits_num, int particiones, int vueltas);
ZZ ga(int bits_seed, int bits_num, int particiones, int vueltas);

#endif // PRNG_H
