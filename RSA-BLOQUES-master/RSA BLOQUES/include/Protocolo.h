#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include "Funciones Matematicas.h"

class Protocolo
{
    string alfabeto = "abcdefghijklmnñopqrstuvwxyz ABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789.,;:-_()@{}+/";//"abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";////"0123456789 abcdefghijklmnopqrstuvwxyz.,ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    ZZ q, g, e, N, d, N_r, e_r, p_1 ,p_2, k, a, C_a;
    int tam_firma;
    void generar_claves(int bits);
    public:
        void set_N(ZZ n);
        void set_e(ZZ E);
        void set_d(ZZ D);
        void set_g(ZZ G);
        void set_q(ZZ Q);
        void update_k();
        void set_a(int bits);
        void set_aa(ZZ A);
        void set_C_a();
        string firmar(string firma);
        Protocolo(int bits, ZZ g_r, ZZ q_r, ZZ N_r, ZZ e_r);
        Protocolo(int bits);
        int get_tamfirma();
        ZZ get_e();
        ZZ get_n();
        ZZ get_q();
        ZZ get_g();
        ZZ resto_chino(ZZ c);
        string bloques(string mensaje, ZZ exponente, ZZ N);
        string cifrar_firma(string firma);
        string cifrar(string mensaje, string firma);
        string descifrar(string mensaje, int tam_firma_cifrada, int tam_firma_original, ZZ e_firma, ZZ N_firma);
};

#endif // PROTOCOLO_H
