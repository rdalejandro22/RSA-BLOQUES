#include "ElGamal.h"
void ElGamal::generar_claves_receptor(int bits)
{
    ZZ q;
    q = ga(100, bits, 7, 3);
    while(ProbPrime(q, 10) != 1)
    {
        q = ga(100, bits, 7, 3);
    }
    P = (2 * q) + 1;
    e_1 = raiz_primitiva(P);
    d = ga(100, bits, 7, 3);
    while(d >= (P-2))
    {
        d = ga(100, bits, 7, 3);
    }
    e_2 = potencia(e_1, d, P);
}
ElGamal::ElGamal(ZZ e_1, ZZ e_2, ZZ P, int bits)
{
    r = ga(100, bits, 7, 3);
    while((r < 2) && (r > (P - 2)))
    {
        r = ga(100, bits, 7, 3);
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
    string c1 = to_string(C);
    string digitos_P = to_string(P);
    if(c1.length() < digitos_P.size())
    {
        for(int i = 0; i < digitos_P.size() - c1.size(); i++)
        {
            c1 += "0";
        }
    }
    mensaje_cifrado += c1;
    string letra_significativa = to_string(to_ZZ(alfabeto.length() - 1));
    ZZ digitos_letra_significativa = to_ZZ(letra_significativa.length());
    string digitos_N = to_string(P);
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
            string_letra_cifrada = to_string(modulo((letra_cifrada * K), P));
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
string ElGamal::descifrar(string mensaje)
{
    string digitos_P = to_string(P);
    string c_string;
    for(int i = 0; i < digitos_P.size(); i++)
    {
        c_string += mensaje[i];
    }
    ZZ C_1 = string_toZZ(c_string);
    K = potencia(C_1, d, P);
    ZZ InversaK = inversa(K, P);
    string mensaje_descifrado, mensaje_temporal;
    ZZ digitos_N = to_ZZ(to_string(P).length());
    string string_letra_sig = to_string(to_ZZ(alfabeto.length()-1));
    ZZ digitos_letra_significativa = to_ZZ(string_letra_sig.length());
    ZZ letra_descifrada;
    string aux;
    int count = 0;
    for(int i = digitos_P.size(); i < mensaje.size(); i++)
    {
        aux += mensaje[i];
        count += 1;
        if(count == digitos_N)
        {
            letra_descifrada = modulo((string_toZZ(aux)*InversaK), P);;
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
            mensaje_descifrado += alfabeto[to_int(letra_reemplazada)];
            count = 0;
            aux = "";
        }
    }
    return mensaje_descifrado;
}
