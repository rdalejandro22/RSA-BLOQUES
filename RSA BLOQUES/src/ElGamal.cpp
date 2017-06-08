#include "ElGamal.h"
void ElGamal::generar_claves_receptor(int bits)
{
    P = ga(bits/4, bits, 20, 3);
    while(ProbPrime(P, 10) != 1)
    {
        P = ga(bits/4, bits, 20, 3);
    }
    e_1 = raiz_primitiva(P);
    d = ga(bits/4, bits, 20, 3);
    while((d > (P - 2)) && (d < 2))
    {
        d = ga(bits/4, bits, 20, 3);
    }
    e_2 = potencia(e_1, d, P);
}
ElGamal::ElGamal(ZZ e_1, ZZ e_2, ZZ P, int bits)
{
    r = ga(bits/4, bits, 20, 3);
    while((r < 2) && (r > P - 2))
    {
        r = ga(bits/4, bits, 20, 3);;
    }
    C = potencia(e_1, r, P);
    K = potencia(e_2, r, P);
    this->P = P;
}
ElGamal::ElGamal(int bits)
{
    generar_claves_receptor(bits);
    ofstream claves;
    claves.open ("clavesmias_gamal.txt");
    claves << "e_1: " << e_1 << endl;
    claves << "e_2: " << e_2 << endl;
    claves << "P: " << P << endl;
    claves << "d: " << d << endl;
    claves.close();
    ofstream claves_o;
    claves_o.open ("claves_gamal.txt");
    claves_o << "e_1: " << e_1 << endl;
    claves_o << "e_2: " << e_2 << endl;
    claves_o << "P: " << P << endl;
    claves_o.close();
}
ZZ ElGamal::get_e1()
{
    return e_1;
}
ZZ ElGamal::get_e2()
{
    return e_2;
}
ZZ ElGamal::get_P()
{
    return P;
}
ZZ ElGamal::get_C1()
{
    return C;
}
void ElGamal::set_d(ZZ d)
{
    this->d = d;
}
void ElGamal::set_P(ZZ P)
{
    this->P = P;
}
string ElGamal::cifrar(string mensaje)
{
    string mensaje_cifrado, mensaje_temporal;
    string letra_significativa = to_string(to_ZZ(alfabeto.length() - 1));
    ZZ digitos_letra_significativa = to_ZZ(letra_significativa.length());
    string digitos_P = to_string(P);
    int bloques = digitos_P.length();
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
            string_letra_cifrada = to_string(modulo((letra_cifrada * K), P));
            if(string_letra_cifrada.length() < digitos_P.length())
            {
                for(int i = 0; i < (digitos_P.length() - string_letra_cifrada.length()); i++)
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
string ElGamal::descifrar(ZZ C_1, string mensaje)
{
    K = potencia(C_1, d, P);
    ZZ InversaK = inversa(K, P);
    string mensaje_descifrado, mensaje_temporal;
    ZZ digitos_P = to_ZZ(to_string(P).length());
    string string_letra_sig = to_string(to_ZZ(alfabeto.length()-1));
    ZZ digitos_letra_significativa = to_ZZ(string_letra_sig.length());
    ZZ letra_descifrada;
    string aux;
    int count = 0;
    for(int i = 0; i < mensaje.size(); i++)
    {
        aux += mensaje[i];
        count += 1;
        if(count == digitos_P)
        {
            letra_descifrada = modulo((string_toZZ(aux)*InversaK), P);
            if(to_string(letra_descifrada).length() < digitos_P)
            {
                for(int i = 0; i < digitos_P - to_string(letra_descifrada).length(); i++)
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
            mensaje_descifrado += alfabeto[to_int(letra_reemplazada)];
            count = 0;
            aux = "";
        }
    }
    return mensaje_descifrado;
}
