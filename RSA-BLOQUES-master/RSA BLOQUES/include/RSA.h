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
    string alfabeto = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";//"abcdefghijklmnñopqrstuvwxyz ABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789.,;:-_()@{}+/";//"0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ZZ e, d, N, p ,q, e_r, N_r;
    int tam_firma;
    void generar_claves(int bits);
    public:
        string firmar(string firma);
        RSA(int bits, ZZ e, ZZ n);
        RSA(int bits);
        void set_N(ZZ n);
        void set_e(ZZ E);
        void set_d(ZZ D);
        void set_p(ZZ P);
        void set_q(ZZ Q);
        int get_tamfirma();
        ZZ get_e();
        ZZ get_n();
        ZZ resto_chino(ZZ c);
        string bloques(string mensaje, ZZ exponente, ZZ N);
        string cifrar_firma(string firma);
        string cifrar(string mensaje, string firma);
        string descifrar(string mensaje, int tam_firma_cifrada, int tam_firma_original, ZZ e_firma, ZZ N_firma);
};
#endif // RSA_H
