#include "dstbin.h"
#include <cmath>

//Construtor
dstBin::dstBin() {}

//Seters
void dstBin::setN(int n)
{
    this->n = n;
}

void dstBin::setSuc(int suc)
{
    this->suc = suc;
}

//Geters

float dstBin::getMed()
{
    return med;
}

float dstBin::getVar()
{
    return var;
}

float dstBin::getDesvPad()
{
    return desvPad;
}

float dstBin::getCv()
{
    return cv;
}

//Metodos

void dstBin::calc()
{
    p = static_cast<double>(suc)/n;
    q = 1-p;
    med = n*p;
    var = med*q;
    desvPad = sqrt(var);
    cv = (100*desvPad)/med;
    P.resize(n+1);

    for(int x = 0; x <= n; x++)
        {
            int k = x;
            if (x > n-x) k = n-x;
            unsigned long res = 1;
            for (int i = 1; i <= k; i++) res = (res*(n-k + i))/i;
            P[x] = res*pow(p, x)*pow(q, n-x);
        }

}



