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

int dstPoi::getN()
{
    return n;
}

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
    desvPad = sqrt(lambMedVar);
    cv = (100*desvPad)/lambMedVar;

    P.resize(n+1);
    P[0] = exp(-lambMedVar);
    for (int x = 1; x <= n; x++) P[x] = (lambMedVar / x) * P[x - 1];
}

void dstPoi::calcP(bool inputValuesChangedPoi, int newN) //Calcula as probabilidades restantes caso n aumente
{
    if (newN > n) //Se o n dado é maior que o n atual
    {
        if (inputValuesChangedPoi) n = newN; //Se P ainda não foi calculado, atribui a n o novo valor
        else //Se P já foi calculado, calcula as distribuições que faltam
        {
            P.resize(newN+1);
            for (int x = n+1; x <= newN; x++) P[x] = (lambMedVar / x) * P[x - 1];
            n = newN;
        }
    }
}
