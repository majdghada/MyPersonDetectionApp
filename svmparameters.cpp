#include "svmparameters.h"

double SVMParameters::getSvm_c() const
{
    return svm_c;
}

void SVMParameters::setSvm_c(double value)
{
    svm_c = value;
}

double SVMParameters::getSvm_Gamma() const
{
    return svm_Gamma;
}

void SVMParameters::setSvm_Gamma(double value)
{
    svm_Gamma = value;
}

int SVMParameters::getSvm_type() const
{
    return svm_type;
}

void SVMParameters::setSvm_type(int value)
{
    svm_type = value;
}

int SVMParameters::getSvm_kernel() const
{
    return svm_kernel;
}

void SVMParameters::setSvm_kernel(int value)
{
    svm_kernel = value;
}

int SVMParameters::getSvm_MaxIter() const
{
    return svm_MaxIter;
}

void SVMParameters::setSvm_MaxIter(int value)
{
    svm_MaxIter = value;
}

double SVMParameters::getSvm_EPS() const
{
    return svm_EPS;
}

void SVMParameters::setSvm_EPS(double value)
{
    svm_EPS = value;
}

double SVMParameters::getSvm_degree() const
{
    return svm_degree;
}

void SVMParameters::setSvm_degree(double value)
{
    svm_degree = value;
}

double SVMParameters::getSvm_P() const
{
    return svm_P;
}

void SVMParameters::setSvm_P(double value)
{
    svm_P = value;
}

double SVMParameters::getSvm_Nu() const
{
    return svm_Nu;
}

void SVMParameters::setSvm_Nu(double value)
{
    svm_Nu = value;
}

double SVMParameters::getSvm_Coef0() const
{
    return svm_Coef0;
}

void SVMParameters::setSvm_Coef0(double value)
{
    svm_Coef0 = value;
}

SVMParameters::SVMParameters()
{
    setSvm_c(0.01);
    setSvm_Gamma(0.000006);
    setSvm_type(SVM::C_SVC);
    setSvm_kernel(SVM::LINEAR);
    setSvm_MaxIter(100);
    setSvm_EPS(0.003);
    setSvm_degree(3);
    setSvm_P(0.1);
    setSvm_Nu(0.5);
    setSvm_Coef0(0.0);

}

SVMParameters::SVMParameters(const SVMParameters &other)
{
    setSvm_c(other.getSvm_c());
    setSvm_Gamma(other.getSvm_Gamma());
    setSvm_type(other.getSvm_type());
    setSvm_kernel(other.getSvm_kernel());
    setSvm_MaxIter(other.getSvm_MaxIter());
    setSvm_EPS(other.getSvm_EPS());
    setSvm_degree(other.getSvm_degree());
    setSvm_P(other.getSvm_P());
    setSvm_Nu(other.getSvm_Nu());
    setSvm_Coef0(other.getSvm_Coef0());
}

void SVMParameters::setParameters(MySVM &svm)
{
    svm.getSvm()->setCoef0(this->svm_Coef0);
    svm.getSvm()->setC(this->svm_c);
    svm.getSvm()->setGamma(this->svm_Gamma);
    svm.getSvm()->setType(this->svm_type);
    svm.getSvm()->setKernel(this->svm_kernel);
    svm.getSvm()->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER|TermCriteria::EPS, this->svm_MaxIter, this->svm_EPS));
    svm.getSvm()->setDegree(this->svm_degree);
    svm.getSvm()->setNu(this->svm_Nu);
    svm.getSvm()->setP(this->svm_P);
}

void SVMParameters::getParameters(const MySVM &svm)
{
    setSvm_Coef0(svm.getSvm()->getCoef0());
    setSvm_Nu(svm.getSvm()->getNu());
    setSvm_P(svm.getSvm()->getP());
    setSvm_c(svm.getSvm()->getC());
    setSvm_Gamma(svm.getSvm()->getGamma());
    setSvm_type(svm.getSvm()->getType());
    setSvm_kernel(svm.getSvm()->getKernelType());
    setSvm_MaxIter(svm.getSvm()->getTermCriteria().maxCount);
    setSvm_EPS(svm.getSvm()->getTermCriteria().epsilon);
    setSvm_degree(svm.getSvm()->getDegree());
}

