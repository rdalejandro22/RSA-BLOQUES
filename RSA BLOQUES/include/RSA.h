#ifndef RSA_H
#define RSA_H
#include <string>
#include <NTL/ZZ.h>
#include <vector>
#include <sstream>
using namespace NTL;
class RSA
{
    std::string alfabeto = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,;";
    ZZ clave_publica, clave_privada, N;
    void generar_claves();
    public:
        RSA(ZZ e, ZZ n);
        RSA();
        void set_n(ZZ n);
        void set_clave_privada(ZZ key);
        ZZ get_N();
        ZZ get_clave_publica();
        std::string zToString(const ZZ &z);
        std::vector <ZZ> cifrar(std::string mensaje);
        std::string descifrar(std::vector <ZZ> mensaje);
};
#endif // RSA_H
