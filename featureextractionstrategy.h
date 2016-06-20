#ifndef FEATUREEXTRACTIONSTRATEGY_H
#define FEATUREEXTRACTIONSTRATEGY_H
#include <opencv2/core.hpp>
using namespace cv;

class FeatureExtractionStrategy
{

public:
    virtual Mat getFeatures(Mat img)=0;
    virtual std::string strategyName()=0;
};
class SIFT:public FeatureExtractionStrategy{

public:
    Mat getFeatures(Mat img);
    std::string strategyName();
};

class HOG:public FeatureExtractionStrategy{
    Mat getFeatures(Mat img);
    std::string strategyName();

};

#endif // FEATUREEXTRACTIONSTRATEGY_H
