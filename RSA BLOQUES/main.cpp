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
    ZZ a = ga(4,8,2,1);
    cout << a;
}
