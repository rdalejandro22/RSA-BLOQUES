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
    p = ga(5,16,7,3);
    q = ga(5,16,3,4);
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
    e = ga(7,16,3,2);
    while(e > phi_N || euclides(e, phi_N) != 1)
    {
        e = ga(7,16,4,5);
    }
    cout <<"Clave publica: " << e << endl;
    d = inversa(e, phi_N);
    cout << "Clave privada: " << d << endl;
    cout <<"N: " << N << endl;
}
RSA::RSA(ZZ e, ZZ n) //EMISOR
{
    this->e = e;
    N = n;
}
RSA::RSA() //RECEPTOR
{
    generar_claves();
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
        numero_letra = alfabeto.find(mensaje[i]);
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
            mensaje_temporal += to_string(to_ZZ(alfabeto.find("#")));
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
                for(int i = 0; i < digitos_N.length() - string_letra_cifrada.length(); i++)
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
    string mensaje_descifrado;
    ZZ digitos_N = to_ZZ(to_string(N).length());
    ZZ posicion;
    int letra_cifrada;
    for(int i = 0; i < mensaje.size(); i++)
    {
        posicion = mensaje[i];
        letra_cifrada = to_int(potencia(posicion, d, N));
        mensaje_descifrado += alfabeto[letra_cifrada];
    }
    return mensaje_descifrado;
}
