#include "RSA.h"
#include "PRNG.h"
#include <iostream>
#include <string>
#include <sstream>
#include <NTL/ZZ.h>
#include "Funciones Matematicas.h"
using namespace NTL;
using namespace std;
void RSA::generar_claves()
{
    ZZ p = ga(5,16,7,3);
    ZZ q = ga(5,16,3,4);
    while(ProbPrime(p,10)!=1)
    {
        p = ga(5,16,7,3);
    }
    while(ProbPrime(q,10)!=1)
    {
        q = ga(5,16,3,4);
    }
    N = p * q;
    ZZ phi_N = (p - 1) * (q - 1);
    ZZ e = ga(7,16,3,2);
    while(e > phi_N || euclides(e, phi_N) != 1)
    {
        e = ga(700,1024,14,8);
    }
    cout <<"Clave publica: " << e << endl;
    clave_publica = e;
    clave_privada = inversa(clave_publica, phi_N);
    cout << "Clave privada: " << clave_privada << endl;
    cout <<"N: " << N << endl;
}
RSA::RSA(ZZ e, ZZ n) //EMISOR
{
    clave_publica = e;
    N = n;
}
RSA::RSA() //RECEPTOR
{
    generar_claves();
}
void RSA::set_n(ZZ n)
{
    N = n;
}
void RSA::set_clave_privada(ZZ key)
{
    clave_privada = key;
}
ZZ RSA::get_N()
{
    return N;
}
ZZ RSA::get_clave_publica()
{
    return clave_publica;
}
std::string RSA::zToString(const ZZ &z) {
    std::stringstream buffer;
    buffer << z;
    return buffer.str();
}
/*vector <string> RSA::cifrar(string mensaje)
{
    vector <string> mensaje_cifrado;
    int posicion;
    ZZ letra_cifrada;
    for(int i = 0; i < mensaje.length(); i++)
    {
        posicion = alfabeto.find(mensaje[i]);
        letra_cifrada = potencia(to_ZZ(posicion), clave_publica, N);
        mensaje_cifrado.push_back(letra_cifrada);
    }
    for(int i = 0; i < mensaje_cifrado.size(); i++)
    {
        cout << mensaje_cifrado[i] << ",";
    }
    return mensaje_cifrado;
}*/
 string RSA::descifrar(vector <ZZ> mensaje)
{
    string mensaje_descifrado;
    ZZ posicion;
    int letra_cifrada;
    for(int i = 0; i < mensaje.size(); i++)
    {
        posicion = mensaje[i];
        letra_cifrada = to_int(potencia(posicion, clave_privada, N));
        mensaje_descifrado += alfabeto[letra_cifrada];
    }
    return mensaje_descifrado;
}
