#include "PRNG.h"
#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
using namespace std;
using namespace NTL;
ZZ convertir_decimal(vector <bool> a, int bits_num)
{
    ZZ num;
    num = 0;
    int e = 0;
    for(int i = bits_num - 1; i >= 0; i--)
    {
        if(a[i]==1)
        {
            num += power2_ZZ(e);
        }
        else
            num += to_ZZ(0);
        e += 1;
    }
    return num;
}
void rotar_derecha(vector <bool> &vec, int indice, int elementos, int vueltas)
{
    bool temp;
    bool ultimo = vec[indice + elementos - 1];
    for(int j = vueltas; j > 0; j--)
    {
        for(int i = elementos + indice - 1; i > indice; i--)
        {
            temp = vec[i];
            vec[i] = vec[i - 1];
            vec[i - 1] = temp;
        }
    }
    vec[indice] = ultimo ^ vec[rand()%(elementos-1)];
}

void rotar_izquierda(vector <bool> &vec, int indice, int elementos, int vueltas)
{
    bool temp;
    bool primero = vec[indice];
    for(int j = vueltas; j > 0; j--)
    {
        for(int i = indice; i < indice + elementos - 1; i++)
        {
            temp = vec[i];
            vec[i] = vec[i + 1];
            vec[i + 1] = temp;
        }
    }
    vec[indice + elementos - 1] = primero ^ vec[rand()%(elementos - 1)];
}
ZZ ga(int bits_seed, int bits_num, int particiones, int vueltas)
{
    vector <bool> a;
    for(int i = 0; i < bits_num; i++)
    {
        a.push_back(0);
    }
    for(int i = 0; i < bits_seed; i++)
    {
        a[i] = rand()%2;
    }
    int s = 0;
    for(int i = bits_seed; i < bits_num; i++)
    {
        a[i] = a[s] ^ a[s+1];
        s++;
    }
    int elementos_pedazo = bits_num/particiones;
    int residuo_elementos = bits_num%particiones;
    int indice = 0;
    int cont_particiones = 0;
    while(cont_particiones < particiones - 1)
    {
        if(cont_particiones%2 == 0)
        {
            rotar_izquierda(a, indice, elementos_pedazo, vueltas);
        }
        else
            rotar_derecha(a, indice, elementos_pedazo, vueltas);
        indice += elementos_pedazo;
        cont_particiones++;
    }
    if(cont_particiones%2 == 0)
    {
        rotar_izquierda(a, indice, bits_num - ((particiones - 1) * elementos_pedazo), vueltas);
    }
    else
        rotar_derecha(a, indice, bits_num - ((particiones - 1) * elementos_pedazo), vueltas);
    ZZ num;
    num = convertir_decimal(a, bits_num);
    return num;
}
