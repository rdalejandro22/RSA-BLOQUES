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
    RSA mango(to_ZZ(3), to_ZZ(1003));
    string mensaje = "comehere#";
    cout << mango.cifrar(mensaje);

}
