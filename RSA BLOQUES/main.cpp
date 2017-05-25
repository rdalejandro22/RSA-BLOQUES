#include <iostream>
#include "PRNG.h"
#include <NTL/ZZ.h>
#include <iostream>
#include <NTL/ZZ.h>
#include <string>
#include "RSA.h"
#include <time.h>
using namespace std;
using namespace NTL;

int main()
{
    srand(time(NULL));
    //RSA mango(to_ZZ(3), to_ZZ(1003));
    RSA wango;
    /*wango.set_e(to_ZZ(3));
    wango.set_N(to_ZZ(1003));
    wango.set_d(to_ZZ(619));
    wango.set_p(to_ZZ(17));
    wango.set_q(to_ZZ(59));*/
    string mensaje = "comehere#";
    string c = mango.cifrar(mensaje);
    cout << wango.descifrar(c);
}
