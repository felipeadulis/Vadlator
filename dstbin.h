#ifndef DSTBIN_H
#define DSTBIN_H
#include<vector>

class dstBin
{
private:
    int n, suc, x;
    float p, q, med, var, desvPad, cv;
    std::vector<double> P;

    //Metodos


public:
    //Construtor
    dstBin();

    //Seters
    void setN(int n);

    void setSuc(int suc);

    //Geters
    float getMed();

    float getVar();

    float getDesvPad();

    float getCv();

    double getP(int i);

    //Metodos
    void calc();





};

#endif // DSTBIN_H
