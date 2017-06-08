#include "RSA.h"
#include <fstream>
#include "PRNG.h"
#include <iostream>
#include <string>
#include <sstream>
#include <NTL/ZZ.h>
#include "Funciones Matematicas.h"
using namespace NTL;
using namespace std;
void RSA::generar_claves(int bits)
{
    p = ga(200,bits,7,3);
    q = ga(200,bits,3,4);
    while(ProbPrime(p,10)!=1)
    {
        p = ga(200,bits,7,3);
    }
    while(ProbPrime(q,10)!=1)
    {
        q = ga(200,bits,3,4);
    }
    N = p * q;
    ZZ phi_N = (p - 1) * (q - 1);
    e = ga(200,bits,3,2);
    while(e > phi_N || euclides(e, phi_N) != 1)
    {
        e = ga(200,bits,4,5);
    }
    d = inversa(e, phi_N);
}
RSA::RSA(ZZ e, ZZ n) //EMISOR
{
    this->e = e;
    N = n;
}
RSA::RSA(int bits) //RECEPTOR
{
    generar_claves(bits);
    ofstream claves;
    claves.open ("claves_mias.txt");
    claves << "e: " << e << endl;
    claves << "d: " << d << endl;
    claves << "N: " << N << endl;
    claves << "p: " << p << endl;
    claves << "q: " << q << endl;
    claves.close();
    ofstream claves_o;
    claves_o.open ("claves.txt");
    claves_o << "e: " << e << endl;
    claves_o << "N: " << N << endl;
    claves_o.close();
}
void RSA::set_N(ZZ n)
{
    N = n;
}
void RSA::set_e(ZZ E)
{
    e = E;
}
void RSA::set_d(ZZ D)
{
    d = D;
}
void RSA::set_p(ZZ P)
{
    p = P;
}
void RSA::set_q(ZZ Q)
{
    q = Q;
}
void RSA::get()
{
    ofstream claves;
    claves.open ("get.txt");
    claves << "e: " << e << endl;
    claves << "d: " << d << endl;
    claves << "N: " << N << endl;
    claves << "p: " << p << endl;
    claves << "q: " << q << endl;
    claves.close();
}
ZZ RSA::resto_chino(ZZ c)
{
    ZZ P = p * q;
    ZZ dp = modulo(d, p - 1);
    ZZ dq = modulo(d, q - 1);
    ZZ a_1 = potencia(modulo(c, p), dp, p);
    ZZ a_2 = potencia(modulo(c, q), dq, q);
    ZZ p_1 = P/p;
    ZZ p_2 = P/q;
    ZZ q_1 = inversa(p_1, p);
    ZZ q_2 = inversa(p_2, q);
    ZZ d_0 = modulo(modulo((a_1 * p_1 * q_1), P) + modulo((a_2 * p_2 * q_2), P), P);
    return d_0;
}
string RSA::cifrar(string mensaje)
{
    string mensaje_cifrado, mensaje_temporal;
    string letra_significativa = to_string(to_ZZ(alfabeto.length() - 1));
    ZZ digitos_letra_significativa = to_ZZ(letra_significativa.length());
    string digitos_N = to_string(N);
    int bloques = digitos_N.length() - 1;
    ZZ numero_letra;
    string string_letra, string_letra_temp;
    for(int i = 0; i < mensaje.length(); i++)
    {
        string_letra_temp = "";
        numero_letra = to_ZZ(alfabeto.find(mensaje[i]));
        string_letra = to_string(numero_letra);
        if(string_letra.length() < digitos_letra_significativa) //llenar con 0 si es que tiene menos digitos 2 -> 02
        {
            for(int i = 0; i < digitos_letra_significativa - string_letra.length(); i++)
            {
                string_letra_temp += "0";
            }
            string_letra_temp += string_letra;
            mensaje_temporal += string_letra_temp;
        }
        else
            mensaje_temporal += string_letra;
    }
    while(mensaje_temporal.length()%(bloques)!=0)
    {
            mensaje_temporal += to_string(to_ZZ(alfabeto.find("w")));
    }
    string aux;
    string string_letra_cifrada;
    int count = 0;
    for(int i = 0; i < mensaje_temporal.length(); i++)
    {
        aux += mensaje_temporal[i];
        count += 1;
        if(count == bloques)
        {
            ZZ letra_cifrada = string_toZZ(aux);
            string_letra_cifrada = to_string(potencia(letra_cifrada, e, N));
            if(string_letra_cifrada.length() < digitos_N.length())
            {
                for(int i = 0; i < (digitos_N.length() - string_letra_cifrada.length()); i++)
                {
                    mensaje_cifrado += "0";
                }
                mensaje_cifrado += string_letra_cifrada;
            }
            else
                mensaje_cifrado += string_letra_cifrada;
            aux = "";
            string_letra_cifrada = "";
            count = 0;
        }
    }
    return mensaje_cifrado;
}
string RSA::descifrar(string mensaje)
{
    string mensaje_descifrado, mensaje_temporal;
    ZZ digitos_N = to_ZZ(to_string(N).length());
    string string_letra_sig = to_string(to_ZZ(alfabeto.length()-1));
    ZZ digitos_letra_significativa = to_ZZ(string_letra_sig.length());
    ZZ letra_descifrada;
    string aux;
    int count = 0;
    for(int i = 0; i < mensaje.size(); i++)
    {
        aux += mensaje[i];
        count += 1;
        if(count == digitos_N)
        {
            letra_descifrada = resto_chino(string_toZZ(aux));
            if(to_string(letra_descifrada).length() < digitos_N - 1)
            {
                for(int i = 0; i < digitos_N - to_string(letra_descifrada).length() - 1; i++)
                {
                    mensaje_temporal += "0";
                }
                mensaje_temporal += to_string(letra_descifrada);
            }
            else
                mensaje_temporal += to_string(letra_descifrada);
            aux = "";
            count = 0;
            letra_descifrada = 0;
        }
    }
    ZZ letra_reemplazada;
    for(int i = 0; i < mensaje_temporal.size(); i++)
    {
        aux += mensaje_temporal[i];
        count += 1;
        if(count == digitos_letra_significativa)
        {
            letra_reemplazada = string_toZZ(aux);
            cout << "Esta es la letra: " << aux << endl;
            mensaje_descifrado += alfabeto[to_int(letra_reemplazada)];
            count = 0;
            aux = "";
        }
    }
    return mensaje_descifrado;
}



