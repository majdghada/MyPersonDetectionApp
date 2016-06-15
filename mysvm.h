#ifndef MYSVM_H
#define MYSVM_H

#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>
using namespace cv;
using namespace ml;
class MySVM
{
private:
    Mat trainingDataMat;
    Mat labelsMat;
    Ptr<SVM> svm;
    int featcnt;
protected:
    void setFeatcnt(int featcnt);
public:

    MySVM();
    MySVM(int featcnt);
    void load (const std::string & path);
    void addExample(Mat feat,int label);
    void train();
    void clearTrainingData();
    float predict(Mat feat);
    void autoTrain(ParamGrid Cgrid=SVM::getDefaultGrid(SVM::C),ParamGrid Gammagrid=
            SVM::getDefaultGrid(SVM::GAMMA),ParamGrid Pgrid=
            SVM::getDefaultGrid(SVM::P),ParamGrid Nugrid=
            SVM::getDefaultGrid(SVM::NU),ParamGrid Coefgrid=
            SVM::getDefaultGrid(SVM::COEF),ParamGrid Degreegrid=
            SVM::getDefaultGrid(SVM::DEGREE));
    Ptr<SVM> getSvm()const ;
    Mat getTrainingDataMat() const;
    Mat getLabelsMat() const;
};



#endif // MYSVM_H
