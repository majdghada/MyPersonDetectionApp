#ifndef MYPERSONDETECTOR_H
#define MYPERSONDETECTOR_H

#include "mysvm.h"
#include "svmparameters.h"
#include "dataloader.h"
#include "featureextractionstrategy.h"
#include <QDir>
class DataLoader;
class MyPersonDetector
{
protected:
    QStringList posData;
    QStringList negData;
    std::vector<Mat> positiveImages;
    std::vector<Mat> negativeImages;
    SVMParameters parameters;
    DataLoader * dataLoader;
    void InitSVMTrainingData();
    FeatureExtractionStrategy * fxs;
public:
    MyPersonDetector();

    MySVM svm;
    void setdata(QStringList pos,QStringList neg);
    void loadSVM(std::string path);
    void saveSVM(std::string path);
    void setFeatureExtractionStrategy(FeatureExtractionStrategy * fxs);
    virtual void train();
    void autoTrain();
    int test();
    float predict(Mat img);
    void setDataLoader(DataLoader *loader);
    SVMParameters &getParameters();
    void setParameters(const SVMParameters &value);
    std::vector<DetectionWindow> detectMultiScale(Mat img);
};

class AugmentedPersonDetector:public MyPersonDetector{
public :
    void train();

};

#endif // MYPERSONDETECTOR_H
