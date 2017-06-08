#include "Funciones Matematicas.h"
using namespace NTL;
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
