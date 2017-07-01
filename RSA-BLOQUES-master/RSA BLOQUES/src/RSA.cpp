#include <RSA.h>
#include <fstream>
#include <PRNG.h>
#include <iostream>
#include <string>
#include <sstream>
#include <NTL/ZZ.h>
#include <Funciones Matematicas.h>
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
string RSA::firmar(string firma)
{
    string firma_cifrada;
    int digitos_N_r = obtener_digitos(N_r);
    string bloque, string_potencia, aux, aux_2;
    ZZ res_potencia;
    for(int i = 0; i < firma.size(); i++)
    {
        res_potencia = potencia(to_ZZ(alfabeto.find(firma[i])), d, N);
        string_potencia = to_string(res_potencia);
        if(string_potencia.size() > (digitos_N_r - 1))
        {
            for(int i = 0; i < ((digitos_N_r - 1) - modulo(to_ZZ(string_potencia.size()), to_ZZ(digitos_N_r - 1))); i++)
            {
                bloque += '0';
            }
        }
        bloque += string_potencia;
    }
    res_potencia = 0;
    int count = 0;
    for(int i = 0; i < bloque.size(); i++)
    {
        aux += bloque[i];
        count++;
        if(count == (digitos_N_r - 1))
        {
            res_potencia = potencia(string_toZZ(aux), e_r, N_r);
            if(obtener_digitos(res_potencia) < digitos_N_r)
            {
                for(int i = 0; i < (digitos_N_r - obtener_digitos(res_potencia)); i++)
                {
                    aux_2 += '0';
                }
            }
            aux_2 += to_string(res_potencia);
            firma_cifrada += aux_2;
            count = 0;
            res_potencia = 0;
            aux_2 = "";
        }
    }
    while(firma_cifrada.size() % (digitos_N_r - 1) != 0)
    {
            firma_cifrada += to_string(to_ZZ(alfabeto.find("w")));
    }
    tam_firma = firma_cifrada.size();
    return firma_cifrada;
}
RSA::RSA(int bits, ZZ e_r, ZZ N_receptor) //EMISOR
{
    generar_claves(bits);
    this->e_r = e_r;
    N_r = N_receptor;
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
int RSA::get_tamfirma()
{
    return tam_firma;
}
ZZ RSA::get_e()
{
    return e;
}
ZZ RSA::get_n()
{
    return N;
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
string RSA::bloques(string mensaje, ZZ exponente, ZZ N)
{
    ///COMPLETA CON 0 SI LA LETRA NO TIENE TAMAÑO LETRA MAS SIGNIFICATIVA
    string mensaje_bloques;
    string mensaje_alfabeto; //mensaje completado segun la letra mas significativa del alfabeto
    int digitos_alfabeto = obtener_digitos(to_ZZ(alfabeto.size() - 1)); //digitos de la letra mas significativa
    for(int i = 0; i < mensaje.size(); i++)
    {
        string num_letra = to_string(to_ZZ(alfabeto.find(mensaje[i])));
        int tam_letra = num_letra.size();
        if(tam_letra < digitos_alfabeto)
        {
            for(int i = 0; i < (digitos_alfabeto - tam_letra); i++)
            {
                mensaje_alfabeto += '0';
            }
        }
        mensaje_alfabeto += num_letra;
    }

    ///COMPLETANDO CON LA LETRA MENOS SIGNIFICATIVA PARA BLOQUES DE TAMAÑO N - 1
    int digitos_N = obtener_digitos(N);
    while(mensaje_alfabeto.size() % (digitos_N - 1) != 0)
    {
            mensaje_alfabeto += to_string(to_ZZ(alfabeto.find("w")));
    }

    ///BLOQUES DE TAMAÑO N
    int k = 0;
    string temp;
    for(int i = 0; i < mensaje_alfabeto.size(); i++)
    {
        string bloque;
        temp += mensaje_alfabeto[i];
        k++;
        if(k == (digitos_N - 1))
        {
           bloque = to_string(potencia(string_toZZ(temp), exponente, N));
           if(bloque.size() < digitos_N)
           {
               for(int i = 0; i < (digitos_N - bloque.size()); i++)
               {
                   mensaje_bloques += '0';
               }
           }
           mensaje_bloques += bloque;
           temp.clear();
           k = 0;
        }
    }
    return mensaje_bloques;
}
string RSA::cifrar_firma(string firma)
{
    ///COMPLETA CON 0 SI LA LETRA NO TIENE TAMAÑO LETRA MAS SIGNIFICATIVA
    string firma_alfabeto; //firma completado segun la letra mas significativa del alfabeto
    int digitos_alfabeto = obtener_digitos(to_ZZ(alfabeto.size() - 1)); //digitos de la letra mas significativa
    for(int i = 0; i < firma.size(); i++)
    {
        string num_letra = to_string(to_ZZ(alfabeto.find(firma[i])));
        int tam_letra = num_letra.size();
        if(tam_letra < digitos_alfabeto)
        {
            for(int i = 0; i < (digitos_alfabeto - tam_letra); i++)
            {
                firma_alfabeto += '0';
            }
        }
        firma_alfabeto += num_letra;
    }
    ///HALLA LA POTENCIA DE CADA R,G,B O LETRA Y LOS CONCATENA COMPLETANDO CON CEROS A LA IZQUIERDA CADA BLOQUE
    int digitos_N_r = obtener_digitos(N_r);
    int p = 0;
    string temp;
    string mensaje_conteo;
    for(int i = 0; i < firma_alfabeto.size(); i++)
    {
        string R;
        temp += firma_alfabeto[i];
        p++;
        if(p == digitos_alfabeto)
        {
           R = to_string(potencia(string_toZZ(temp), d, N));
           if(R.size() > (digitos_N_r - 1))
           {
               for(int i = 0; i < ((digitos_N_r - 1) - modulo(to_ZZ(R.size()), to_ZZ(digitos_N_r - 1))); i++)
               {
                   mensaje_conteo += '0';
               }
           }
           mensaje_conteo += R;
           temp.clear();
           p = 0;
        }
    }

    string firma_bloques;
    int k = 0;
    for(int i = 0; i < mensaje_conteo.size(); i++)
    {
        string bloque;
        temp += mensaje_conteo[i];
        k++;
        if(k == (digitos_N_r - 1))
        {
           bloque = to_string(potencia(string_toZZ(temp), e_r, N_r));
           if(bloque.size() < digitos_N_r)
           {
               for(int i = 0; i < (digitos_N_r - bloque.size()); i++)
               {
                   firma_bloques += '0';
               }
           }
           firma_bloques += bloque;
           temp.clear();
           k = 0;
        }
    }
    return firma_bloques;
}
string RSA::cifrar(string mensaje, string firma)
{
    string firma_cifrada = cifrar_firma(firma);
    tam_firma = firma_cifrada.size();
    string mensaje_cifrado = bloques(mensaje, e_r, N_r);
    return firma_cifrada + mensaje_cifrado;

}
string RSA::descifrar(string mensaje, int tam_firma_cifrada, int tam_firma_original, ZZ e_firma, ZZ N_firma)
{
   /* int p = 0;
    int digitos_N = obtener_digitos(N);
    string rubrica;
    string temp;
    for(int i = 0; i < tam_firma_cifrada; i++)
    {
        string bloque;
        temp += tam_firma_cifrada[i];
        p++;
        if(p == digitos_N)
        {
            bloque = to_string(potencia(string_toZZ(temp), d, N));
            if(bloque.size() < digitos_N - 1)
            {
                for(int i = 0; i < (digitos_N - bloque.size()); i++)
                {
                    rubrica += '0';
                }
            }
            rubrica += bloque;
            temp.clear();
            p = 0;
        }
    }

    string firma_descifrada;
    for(int i = 0; i < rubrica.size(); i++)
    {
        string bloque;
        temp += rubrica[i];
        p++;
        if(p == tam_firma_original)
        {
             firma_descifrada += alfabeto.find(to_int(potencia(string_toZZ(temp), e_firma, N_firma)));
             temp.clear();
             p = 0;
        }
    }




    string mensaje_descifrado, mensaje_temporal;
    int digitos_alfabeto = obtener_digitos(to_ZZ((alfabeto.length() - 1)));

    ZZ letra_descifrada;

    int count = 0;
    for(int i = 0; i < (mensaje.size() - tam_firma); i++)
    {
        aux += mensaje[i];
        count += 1;
        if(count == digitos_N)
        {
            letra_descifrada = resto_chino(string_toZZ(aux));
            if(obtener_digitos(letra_descifrada) < (digitos_N - 1))
            {
                for(int i = 0; i < digitos_N - obtener_digitos(letra_descifrada) - 1; i++)
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
        if(count == digitos_alfabeto)
        {
            letra_reemplazada = string_toZZ(aux);
            mensaje_descifrado += alfabeto[to_int(letra_reemplazada)];
            count = 0;
            aux = "";
        }
    }


    return mensaje_descifrado;*/
}



