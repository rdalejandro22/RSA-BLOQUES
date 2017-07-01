#include "Funciones Matematicas.h"
using namespace NTL;
ZZ getBase10(string a)
{
    ZZ num;
    num = 0;
    int e = 0;
    for(int i = a.size() - 1; i >= 0; i--)
    {
        if(a[i] == '1')
        {
            num += power2_ZZ(e);
        }
        else
            num += to_ZZ(0);
        e += 1;
    }
    return num;
}
string rotarizquierda(string num, int vueltas)
{
    char temp;
    for(int j = vueltas; j > 0; j--)
    {
        for(int i = 0; i < num.size() - 1; i++)
        {
            temp = num[i];
            num[i] = num[i + 1];
            num[i + 1] = temp;
        }
    }
    return num;
}
string volver_56bits(string bits)
{
    int pc1[56]={57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4};
    string permutacion;
    for(int i = 0; i < 56; i++)
    {
        permutacion += bits[pc1[i] - 1];
    }
    return permutacion;
}
string volver_48bits(string bits)
{
    int pc2[]={14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32};
    string permutacion;
    for(int i = 0; i < 48; i++)
    {
        permutacion += bits[pc2[i] - 1];
    }
    return permutacion;
}
ZZ des(int bits)
{
	vector <bool> K = ga2(bits/16, bits, bits/10, 2);
	string str_K;
	for(int i = 0; i < K.size(); i++)
    {
        if(K[i] == true)
        {
            str_K += '1';
        }
        else
            str_K += '0';
    }
	int vueltas[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	string instancia_56bits;
	for(int i = 0; i < bits; i = i + 64)
    {
 			instancia_56bits += volver_56bits(str_K.substr(i,64));
    }
	for(int j = 0; j < 16; j++)
	{
		string Ks_48;
		string temp;
		for(int i = 0; i < instancia_56bits.size(); i = i + 56)
		{
 			string c = rotarizquierda(instancia_56bits.substr(i,28), vueltas[j]);
			string d = rotarizquierda(instancia_56bits.substr(i + 28, 28), vueltas[j]);
			temp += c;
			temp += d;
			Ks_48 += volver_48bits(c + d);
		}
		instancia_56bits = temp;
		if(Ks_48[Ks_48.size() - 1] == '0')
		{
			Ks_48[Ks_48.size() - 1] = '1';
		}
		ZZ num = getBase10(Ks_48);
		if(ProbPrime(num,10))
		{
			return num;
		}
	}
	return to_ZZ(0);
}
ZZ modulo(ZZ a, ZZ n)
{
    ZZ q, r;
	q = a / n;
	r = a - (q*n);
	if (r < 0)
		r += n;
	return r;
}
ZZ euclides(ZZ a, ZZ b)
{
     ZZ residuo = modulo(a,b);
     while(residuo!=0)
     {
         a = b;
         b = residuo;
         residuo = modulo(a,b);
     }
     return b;
}
vector <ZZ> euclides_extendido(ZZ a, ZZ b)
{
    std::vector <ZZ> resultados;
    ZZ r1,s1,t1,r2,s2,t2,q,r;
    r1 = a;
    r2 = b;
    s1 = 1;
    s2 = 0;
    t1 = 0;
    t2 = 1;
    while(r2>0)
    {
        q = r1/r2;

        ZZ r = r1 - q * r2;
        r1 = r2;
        r2 = r;

        ZZ s = s1 - q * s2;
        s1 = s2;
        s2 = s;

        ZZ t = t1 - q * t2;
        t1 = t2;
        t2 = t;

    }
    resultados.push_back(r1);
    resultados.push_back(s1);
    resultados.push_back(t1);
    return resultados;
}
ZZ inversa(ZZ a, ZZ n)
{
    ZZ x = euclides_extendido(a, n)[1];
    if(x<0)
    {
        return modulo(x,n);
    }
    return x;
}
vector <bool> ZZtoBinary(ZZ num)
{
    vector <bool> binario;
    while(num>0)
    {
        binario.push_back(to_int(modulo(num,to_ZZ(2))));
        num = num >> 1;
    }
    return binario;
}
ZZ potencia(ZZ a, ZZ b, ZZ n)
{
    ZZ d;
    d = 1;
    vector <bool> bin = ZZtoBinary(b);
    for(int i = bin.size(); i > 0; i--)
    {
        d = modulo((d * d), n);
        if(bin[i - 1] == 1)
        {
            d = modulo((d * a),n);
        }
    }
    return d;

}
ZZ raiz_primitiva(ZZ p)
{
    ZZ q, g;
    q = (p - 1)/2;
    for(int i = 1; i < p - 1; i++)
    {
        g = p - i;
        if((potencia(g, to_ZZ(2), p) != 1) && (potencia(g, q, p) != 1))
        {
            break;
        }
    }
    return q;
}
string to_string(ZZ x)
{
    stringstream buffer;
    buffer << x;
    return buffer.str();
}
ZZ string_toZZ(string str){
    ZZ x(INIT_VAL, str.c_str());
    return x;
}
int obtener_digitos(ZZ num)
{
    string s_num = to_string(num);
    return s_num.size();
}
