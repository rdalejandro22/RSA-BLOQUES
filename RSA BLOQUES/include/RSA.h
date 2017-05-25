#ifndef RSA_H
#define RSA_H
#include <string>
#include <NTL/ZZ.h>
#include <vector>
#include <sstream>
using namespace NTL;
using namespace std;
class RSA
{
    std::string alfabeto = "abcdefghijklmnopqrstuvwxyz# ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    ZZ e, d, N, p ,q;
    void generar_claves();
    public:
        RSA(ZZ e, ZZ n);
        RSA();
        ZZ resto_chino(ZZ c);
        string cifrar(string mensaje);
        string descifrar(string mensaje);
};
#endif // RSA_H
