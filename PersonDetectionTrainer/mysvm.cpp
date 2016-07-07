#include "mysvm.h"
#include <iostream>
using namespace std;


Ptr<SVM> MySVM::getSvm() const
{
    return svm;
}

Mat MySVM::getTrainingDataMat() const
{
    return trainingDataMat;
}

Mat MySVM::getLabelsMat() const
{
    return labelsMat;
}

void MySVM::setFeatcnt(int value)
{
    featcnt = value;
    trainingDataMat=cv::Mat(0,featcnt,CV_32FC1);
    labelsMat=cv::Mat(0,1,CV_32SC1);
}
MySVM::MySVM()
{
    svm = SVM::create();
    featcnt=-1;

}

MySVM::MySVM(int featcnt):MySVM()
{
    setFeatcnt(featcnt);
}

void MySVM::load(const string &path)
{
    svm=Algorithm::load<ml::SVM>(path);
}

void MySVM::addExample(Mat feat, int label)
{
    if (featcnt==-1){
        setFeatcnt(feat.cols);
    }
    if (feat.cols!=featcnt){
        cerr<<"Error in function MySVM::addExample, adding example with wrong number of features\n";
        cerr<<"Required "<<featcnt<<" features , found "<<feat.cols<<"features\n"<<endl;
        return ;
    }
    trainingDataMat.push_back(feat);
    Mat lbl(1,1,CV_32SC1);
    lbl.at<int>(0)=label;
    labelsMat.push_back(lbl);
}

void MySVM::train()
{
    svm->clear();
    svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
    svm->save("/home/majd/Desktop/TrainedSVM-v3.xml");
}

void MySVM::clearTrainingData()
{
    trainingDataMat.release();
    labelsMat.release();
}

float MySVM::predict(Mat feat)
{
    if (featcnt==-1){
        setFeatcnt(feat.cols);
    }
    if (feat.cols!=featcnt){
        cerr<<"Error in function MySVM::predict, wrong number of features for prediction\n";
        cerr<<"Required "<<featcnt<<" features , found "<<feat.cols<<"features\n"<<endl;
        return -1;
    }
    return svm->predict(feat);
}

void MySVM::autoTrain(ParamGrid Cgrid,ParamGrid Gammagrid,ParamGrid Pgrid,ParamGrid Nugrid,ParamGrid Coefgrid,ParamGrid Degreegrid)
{
    svm->clear();

    Ptr<TrainData> traindata=TrainData::create(trainingDataMat,ml::ROW_SAMPLE,labelsMat);
//    svm->trainAuto(traindata);
    svm->trainAuto(traindata,10,Cgrid,Gammagrid,Pgrid,Nugrid,Coefgrid,Degreegrid,true);

    svm->save("/home/majd/Desktop/TrainedSVM-v3.xml");

}
