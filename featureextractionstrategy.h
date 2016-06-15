#ifndef FEATUREEXTRACTIONSTRATEGY_H
#define FEATUREEXTRACTIONSTRATEGY_H
#include <opencv2/core.hpp>
using namespace cv;

class FeatureExtractionStrategy
{
public:
    virtual Mat getFeatures(Mat img)=0;
};
class DSIFT:public FeatureExtractionStrategy{

public:
    Mat getFeatures(Mat img);
};

class HOG:public FeatureExtractionStrategy{
    Mat getFeatures(Mat img);
};

#endif // FEATUREEXTRACTIONSTRATEGY_H
