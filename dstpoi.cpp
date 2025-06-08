#include "dstpoi.h"
#include <cmath>

dstPoi::dstPoi() {}

//Seters
void dstPoi::setLambMedVar(float lambMedVar)
{
    this->lambMedVar = lambMedVar;
}

void dstPoi::setN(int n)
{
    this->n = n;
}

//Geters
float dstPoi::getLambMedVar()
{
    return lambMedVar;
}

float dstPoi::getDesvPad()
{
    return desvPad;
}

float dstPoi::getCv()
{
    return cv;
}

double dstPoi::getP(int x)
{
    return P[x];
}

//Métodos

void dstPoi::calc()
{
    lambMedVar;
    desvPad = sqrt(lambMedVar);
    cv = (100*desvPad)/lambMedVar;
    P.resize(n+1);

    int fatx = 1;
    for(int x = 0; x <= n; x++)
    {
        fatx *= x;
        P[x] = (pow(M_E, (-lambMedVar))* pow(lambMedVar, x))/fatx;
    }

}
