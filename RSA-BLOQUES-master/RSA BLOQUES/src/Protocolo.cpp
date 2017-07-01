#include <Protocolo.h>
void Protocolo::generar_claves(int bits)
{
    p_1 = des(bits);
    while(p_1 == 0)
    {
        p_1 = des(bits);
    }
    p_2 = des(bits);
    while(p_2 == 0)
    {
        p_2 = des(bits);
    }
    N = p_1 * p_2;
    ZZ phi_N = (p_1 - 1) * (p_2 - 1);
    e = ga(bits/16, bits, bits/10, 2);
    while(e > phi_N || euclides(e, phi_N) != 1)
    {
        e = ga(bits/16, bits, bits/10, 2);
    }
    d = inversa(e, phi_N);
}
void Protocolo::set_N(ZZ n)
{
    N = n;
}
void Protocolo::set_e(ZZ E)
{
    e = E;
}
void Protocolo::set_d(ZZ D)
{
    d = D;
}
void Protocolo::set_g(ZZ G)
{
    g = G;
}
void Protocolo::set_q(ZZ Q)
{
    q = Q;
}
void Protocolo::update_k()
{
    k = potencia(g, a, q);
}
void Protocolo::set_a(int bits)
{
    a = ga(bits/32, bits, bits/10, 2);
    while(a < q)
    {
        a = ga(bits/32, bits, bits/10, 2);
    }
}
void Protocolo::set_aa(ZZ A)
{
    a = A;
}
void Protocolo::set_C_a()
{
    C_a = potencia(a, e_r, N_r);
}
string Protocolo::firmar(string firma)
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

///EMISOR
Protocolo::Protocolo(int bits, ZZ g_r, ZZ q_r, ZZ N_r, ZZ e_r)
{
    generar_claves(bits);
    g = g_r;
    q = q_r;
    set_a(bits);
    cout << "Emisor generado correctamente" << endl;
    this->N_r = N_r;
    this->e_r = e_r;
    set_C_a();
    k = potencia(g, a, q);

}

///RECEPTOR
Protocolo::Protocolo(int bits)
{
    generar_claves(bits);
    q = des(bits);
    while(q == 0)
    {
        q = des(bits);
    }
    g = raiz_primitiva(q);
    ofstream clave_privada;
    clave_privada.open ("clave_privada.txt");
    clave_privada << "d: " << d << endl;
    clave_privada.close();
    ofstream clave_publica;
    clave_publica.open ("clave_publica.txt");
    clave_publica << "N: " << N << endl;
    clave_publica << "g: " << g << endl;
    clave_publica << "q: " << q << endl;
    clave_publica << "e: " << e << endl;
    clave_publica.close();
}
int Protocolo::get_tamfirma()
{
    return tam_firma;
}
ZZ Protocolo::get_e()
{
    return e;
}
ZZ Protocolo::get_n()
{
    return N;
}
ZZ Protocolo::get_q()
{
    return q;
}
ZZ Protocolo::get_g()
{
    return g;
}
ZZ Protocolo::resto_chino(ZZ c)
{
    /*ZZ P = p * q;
    ZZ dp = modulo(d, p - 1);
    ZZ dq = modulo(d, q - 1);
    ZZ a_1 = potencia(modulo(c, p), dp, p);
    ZZ a_2 = potencia(modulo(c, q), dq, q);
    ZZ p_1 = P/p;
    ZZ p_2 = P/q;
    ZZ q_1 = inversa(p_1, p);
    ZZ q_2 = inversa(p_2, q);
    ZZ d_0 = modulo(modulo((a_1 * p_1 * q_1), P) + modulo((a_2 * p_2 * q_2), P), P);
    return d_0;*/
}
string Protocolo::bloques(string mensaje, ZZ exponente, ZZ N_temp)
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
    int digitos_N = obtener_digitos(N_temp);
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
           bloque = to_string(modulo(string_toZZ(temp) * exponente, N_temp));
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
string Protocolo::cifrar_firma(string firma)
{
    string R;
    for(int i = 0; i < firma.size(); i++)
    {
        R += to_string(potencia(to_ZZ(alfabeto.find(firma[i])), d, N));
    }
    int digitos_N_r = obtener_digitos(N_r);
    string bloques;
    if(R.size() > (digitos_N_r - 1) || R.size() < (digitos_N_r - 1))
    {
        for(int i = 0; i < ((digitos_N_r - 1) - modulo(to_ZZ(R.size()), to_ZZ(digitos_N_r - 1))); i++)
        {
            bloques += '0';
        }
    }
    bloques += R;

    string rubrica_firmada;

    int count = 0;
    string temp;
    for(int i = 0; i < bloques.size(); i++)
    {
        temp += bloques[i];
        count++;
        if(count == digitos_N_r - 1)
        {
            string F = to_string(potencia(string_toZZ(temp), e_r, N_r));
            if(F.size() < digitos_N_r)
            {
                for(int j = 0; j < digitos_N_r - F.size(); j++)
                {
                    rubrica_firmada += '0';
                }
            }
            rubrica_firmada += F;
            count = 0;
            temp.clear();
        }
    }

    int digitos_Q = obtener_digitos(q);
    string rubrica_completa;
    if(rubrica_firmada.size() > digitos_Q - 1)
    {
        int sobrantes = to_int(modulo(to_ZZ(rubrica_firmada.size()), to_ZZ(digitos_Q - 1)));
        for(int i = 0; i < rubrica_firmada.size() - sobrantes; i++)
        {
            rubrica_completa += rubrica_firmada[i];
        }
        for(int j = 0; j < digitos_Q - sobrantes - 1; j++)
        {
            rubrica_completa += '0';
        }
        rubrica_completa += rubrica_firmada.substr(rubrica_firmada.size() - sobrantes, sobrantes);
    }
    else
    {
        for(int j = 0; j < digitos_Q - rubrica_firmada.size() - 1; j++)
        {
            rubrica_completa += '0';
        }
        rubrica_completa += rubrica_firmada;
    }

    string firma_cifrada;
    for(int i = 0; i < rubrica_completa.size(); i++)
    {
        temp += rubrica_completa[i];
        count++;
        if(count == digitos_Q - 1)
        {
            string CMF = to_string(modulo(string_toZZ(temp) * k, q));
            if(CMF.size() < digitos_Q)
            {
                for(int j = 0; j < digitos_Q - CMF.size(); j++)
                {
                    firma_cifrada += '0';
                }
            }
            firma_cifrada += CMF;
            temp.clear();
            count = 0;
        }
    }
    return firma_cifrada;
    /*///COMPLETA CON 0 SI LA LETRA NO TIENE TAMAÑO LETRA MAS SIGNIFICATIVA
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
    return firma_bloques;*/
}
string Protocolo::cifrar(string mensaje, string firma)
{
    string firma_cifrada = cifrar_firma(firma);
    int tam_firma = firma_cifrada.size();
    string mensaje_cifrado = bloques(mensaje, k, q);
    cout << "mensaje_cifrado: " << mensaje_cifrado << endl;
    int digitos_N_r = obtener_digitos(N_r);
    string C_A = to_string(C_a);
    string C_A_completo;
    if(C_A.size() < digitos_N_r)
    {
        for(int i = 0; i < (digitos_N_r - C_A.size()); i++)
        {
            C_A_completo += '0';
        }
    }
    C_A_completo += C_A;
    return firma_cifrada + mensaje_cifrado + C_A_completo;
}

string Protocolo::descifrar(string bloque_codificado, int tam_firma_cifrada, int tam_firma_original, ZZ e_firma, ZZ N_firma)
{
    string C_A_completo;
    int digitos_q = obtener_digitos(q);
    int digitos_N = obtener_digitos(N);
    for(int i = bloque_codificado.size() - digitos_N; i < bloque_codificado.size(); i++)
    {
        C_A_completo += bloque_codificado[i];
    }
    C_a = string_toZZ(C_A_completo);
    a = potencia(C_a, d, N);
    k = potencia(g, a, q);
    ZZ inversa_k = inversa(k, q);
    string mensaje_descifrado, mensaje_temporal;
    int digitos_alfabeto = obtener_digitos(to_ZZ((alfabeto.length() - 1)));

    ZZ letra_descifrada;
    string aux;
    int count = 0;
    for(int i = tam_firma_cifrada; i < (bloque_codificado.size() - digitos_N); i++)
    {
        aux += bloque_codificado[i];
        count ++;
        if(count == digitos_q)
        {
            letra_descifrada = modulo(string_toZZ(aux) * inversa_k, q);
            if(obtener_digitos(letra_descifrada) < (digitos_q - 1))
            {
                for(int i = 0; i < digitos_q - obtener_digitos(letra_descifrada) - 1; i++)
                {
                    mensaje_temporal += '0';
                }
            }
            mensaje_temporal += to_string(letra_descifrada);
            aux.clear();
            count = 0;
            letra_descifrada = 0;
        }
    }

    ZZ letra_reemplazada;
    for(int i = 0; i < mensaje_temporal.size(); i++)
    {
        aux += mensaje_temporal[i];
        count ++;
        if(count == digitos_alfabeto)
        {
            letra_reemplazada = string_toZZ(aux);
            mensaje_descifrado += alfabeto[to_int(letra_reemplazada)];
            aux.clear();
            count = 0;
        }
    }

    string firma_cifrada;
    for(int i = 0; i < tam_firma_cifrada; i++)
    {
        firma_cifrada += bloque_codificado[i];
    }
    string CMF;
    for(int i = 0; i < firma_cifrada.size(); i++)
    {
        aux += firma_cifrada[i];
        count++;
        if(count == digitos_q)
        {
            CMF += to_string(modulo(string_toZZ(aux) * inversa_k, q));
            aux.clear();
            count = 0;
        }
    }
    string rubrica;
    if(CMF.size() % digitos_N != 0)
    {
        for(int i = 0; i < digitos_N - CMF.size() % digitos_N; i++)
        {
            rubrica += '0';
        }
    }
    rubrica += CMF;

    string F;
    for(int i = 0; i < rubrica.size(); i++)
    {
        aux += rubrica[i];
        count++;
        if(count == digitos_N)
        {
            F += to_string(potencia(string_toZZ(aux), d, N));
            aux.clear();
            count = 0;
        }
    }

    string firma;
    if(F.size() % (digitos_N - 1) != 0)
    {
        for(int i = 0; i < digitos_N - 1 - (F.size() % (digitos_N - 1)); i++)
        {
            firma += '0';
        }
    }
    firma += F;

    string firma_descifrada;
    for(int i = 0; i < firma.size(); i++)
    {
        aux += firma[i];
        count++;
        if(count == tam_firma_original)
        {
            firma_descifrada += to_string(potencia(string_toZZ(aux), e_firma, N_firma));
            aux.clear();
            count = 0;
        }
    }
    return mensaje_descifrado;
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

    */
    //return "hola";
}

