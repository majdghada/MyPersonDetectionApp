#ifndef SVMPARAMETERS_H
#define SVMPARAMETERS_H

#include "mysvm.h"
class SVMParameters
{
private:
    double svm_c;
    double svm_Gamma;
    int svm_type;
    int svm_kernel;
    int svm_MaxIter;
    double svm_EPS;
    double svm_degree;
    double svm_P;
    double svm_Nu;
    double svm_Coef0;



public:
    SVMParameters();
    SVMParameters(const SVMParameters & other);
    void setParameters(MySVM &svm);
    void getParameters(const MySVM &svm);

    double getSvm_c() const;
    void setSvm_c(double value);
    double getSvm_Gamma() const;
    void setSvm_Gamma(double value);
    int getSvm_type() const;
    void setSvm_type(int value);
    int getSvm_kernel() const;
    void setSvm_kernel(int value);
    int getSvm_MaxIter() const;
    void setSvm_MaxIter(int value);
    double getSvm_EPS() const;
    void setSvm_EPS(double value);
    double getSvm_degree() const;
    void setSvm_degree(double value);
    double getSvm_P() const;
    void setSvm_P(double value);
    double getSvm_Nu() const;
    void setSvm_Nu(double value);
    double getSvm_Coef0() const;
    void setSvm_Coef0(double value);
};


#endif // SVMPARAMETERS_H
