#ifndef ELGAMAL_H
#define ELGAMAL_H
#include "Funciones Matematicas.h"
#include "PRNG.h"
#include "vector"
#include "fstream"
#include "string.h"
using namespace std;
using namespace NTL;
class ElGamal
{
    string alfabeto = "0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";//"0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";//"abcdefghijklmnñopqrstuvwxyz ABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789.;:-_()@{}+/";
    ZZ P, e_1, e_2, d, C, K, r;
    void generar_claves_receptor(int bits);
    void generar_claves_emisor(int bits);
    public:
        ElGamal(ZZ e_1, ZZ e_2, ZZ P, int bits); //Emisor
        ElGamal(int bits); //Receptor
        ZZ get_e1();
        ZZ get_e2();
        ZZ get_P();
        ZZ get_C1();
        void set_d(ZZ d);
        void set_P(ZZ P);
        //ZZ resto_chino(ZZ c);
        string cifrar(string mensaje);
        string descifrar(string mensaje);
};

#endif // ELGAMAL_H
