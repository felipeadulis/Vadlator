#ifndef DSTPOI_H
#define DSTPOI_H
#include<vector>

class dstPoi
{
private:
    int n = 10;
    float lambMedVar, desvPad, cv;
    std::vector<double> P;

public:
    dstPoi();

    //Seters
    void setLambMedVar(float lambMedVar);

    void setN(int n);

    //Geters
    int getN();

    float getLambMedVar();

    float getDesvPad();

    float getCv();

    double getP(int x);

    //Métodos

    void calc();

    void calcP(bool inputValuesChangedPoi, int newN);

};

#endif // DSTPOI_H
