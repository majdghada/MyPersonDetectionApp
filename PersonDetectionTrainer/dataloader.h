#ifndef DATALOADER_H
#define DATALOADER_H
#include <QStringList>
#include "detectionwindow.h"
#include "mypersondetector.h"
#include <vector>
#include <opencv2/core.hpp>
#include <random>
using namespace cv;
using namespace std;
class MyPersonDetector;
class DataLoader
{
protected:
    DataLoader();
    int imageIdx;
    vector<Mat> * pos,* neg;
public:

    virtual bool hasNext()=0;
    virtual pair<Mat,int> next()=0;
};

class CenterDataLoader:public DataLoader
{
public:
    CenterDataLoader( vector<Mat>& posData, vector<Mat>& negData);
    bool hasNext();
    pair<Mat,int> next();
};

class AugmentedDataLoader:public DataLoader
{
private:

    int windowInImageidx;
    vector<DetectionWindow> subWindows;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;
    MyPersonDetector * originalDetector;
    vector<DetectionWindow> hardExamples;
    void reduceHardExamplesSize();
    int negWindows;
public:

    AugmentedDataLoader( vector<Mat>& posData, vector<Mat>& negData,MyPersonDetector * originalDetector);
    bool hasNext();
    pair<Mat,int> next();
};


#endif // DATALOADER_H
 
