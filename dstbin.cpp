#include "dstbin.h"
#include <cmath>

//Construtor
dstBin::dstBin() {}

//Seters
void dstBin::setN(int n)
{
    this->n = n;
}

void dstBin::setSuc(int p)
{
    this->p = p;
}

void dstBin::calc()
{
    p = suc/n;
    q = 1-p;
    med = n*p;
    var = med*q;
    desvPad = sqrt(var);
    cv = (100*desvPad)/med;
    P.resize(n+1);

    for(int x = 0; x <= n; x++)
    {

    }
}

//Metodos
