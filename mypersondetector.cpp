#include "mypersondetector.h"
#include <vector>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "mainwindow.h"
#include "detectionwindow.h"
#include <QtDebug>
using namespace std;
MyPersonDetector::MyPersonDetector()
{
//    QStringList pos=QDir("/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/pos/").entryList(QDir::Files);
//    for (QString &filename : pos){
//        filename="/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/pos/"+filename;
//    }
//    QStringList neg=QDir("/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/neg/").entryList(QDir::Files);
//    for (QString &filename : neg){
//        filename="/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/neg/"+filename;
//    }
//    while (pos.size()>30)pos.pop_back();
//    while (neg.size()>30)neg.pop_back();

//    setdata(pos,neg);
}

void MyPersonDetector::setdata(QStringList pos, QStringList neg)
{
    posData=pos;
    negData=neg;
    positiveImages.clear();
    negativeImages.clear();
    m_dbg<<"started loading images";
    QStringList *filenames=&posData;
    for (QString filename:*filenames){
        Mat img=imread(filename.toStdString(),IMREAD_GRAYSCALE);
        double div=max(img.rows/300.0,img.cols/300.0);
        cv::resize(img,img,cv::Size(img.cols/div,img.rows/div));
        positiveImages.push_back(img);
    }
    m_dbg<<"finished loading positive";
    filenames=&negData;
    for (auto filename:*filenames){
        Mat img=imread(filename.toStdString(),IMREAD_GRAYSCALE);
        double div=max(img.rows/300.0,img.cols/300.0);
        cv::resize(img,img,cv::Size(img.cols/div,img.rows/div));
        negativeImages.push_back(img);
    }
    m_dbg<<"finished loading negative";
    setDataLoader(new CenterDataLoader(positiveImages,negativeImages));
}

void MyPersonDetector::loadSVM(string path)
{
    svm.load(path);
    m_dbg<<svm.getSvm()->getSupportVectors().size().height<<" "<<svm.getSvm()->getSupportVectors().size().width<<endl;
    m_dbg<<svm.getSvm()->getVarCount()<<endl;
}

void MyPersonDetector::saveSVM(string path)
{
    svm.getSvm()->save(path);
}
Mat getDSIFTDescriptors(Mat img){

    resize(img,img,Size(64,128));
    Ptr<xfeatures2d::SIFT> sift = xfeatures2d::SIFT::create();
    Ptr<xfeatures2d::SIFT> sift1 = xfeatures2d::SIFT::create(4);
    vector <KeyPoint> kps1;
    Mat des1;

    sift1->detect(img,kps1);
    sift1->compute(img,kps1,des1);

    Mat res=des1.t();
    int rows=8,cols=6;
    for (int i=0;i<rows;++i){
        for (int j=0;j<cols;++j){
            Range rowrange(i*img.rows/rows,(i+1)*img.rows/rows-1),
                  colrange(j*img.cols/cols,(j+1)*img.cols/cols-1);
            Mat subimg=img(rowrange,colrange);
            vector<KeyPoint> kps;
            Mat des,labels,centers;
            sift->detect(subimg,kps);
            if (kps.size()==0){
                kps.push_back(KeyPoint((colrange.start+colrange.end)/2.0,
                                       (rowrange.start+rowrange.end)/2.0,
                                       colrange.size()));
            }
            sift->compute(subimg,kps,des);
            if (des.rows==1)
                centers=des;
            else
                kmeans(des,1,labels,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT,
                                                  10, 1.0),1,KMEANS_PP_CENTERS, centers);
            if (i==0&&j==0){
                res=centers.t();
            }
            else {
                res.push_back(centers.t());
            }

        }
    }

    return res.t();
}

SVMParameters & MyPersonDetector::getParameters()
{
    return parameters;
}

void MyPersonDetector::setParameters(const SVMParameters &value)
{
    parameters = value;
}

void MyPersonDetector::InitSVMTrainingData(){
    std::vector<Mat> images;
    std::vector<int> labels;

    while (dataLoader->hasNext()){
        auto data=dataLoader->next();
        Mat img;
        resize(data.first,img,Size(64,128));
        images.push_back(img);
        labels.push_back(data.second);
    }
    m_dbg<<images.size()<<" images ready for feature extraction ";
    svm.clearTrainingData();
    for (unsigned i=0;i<images.size();++i){
        Mat feat=getDSIFTDescriptors(images[i]);
        m_dbg<<"finished the "<<i+1<<"th image";
        svm.addExample(feat,labels[i]);
    }
    m_dbg<<"finished extracting features";
}

void MyPersonDetector::train()
{

    InitSVMTrainingData();
    parameters.setParameters(svm);
    m_dbg<<"started trining";
    svm.train();
    svm.clearTrainingData();
    m_dbg<<"finished training";

}

void MyPersonDetector::autoTrain()
{
    InitSVMTrainingData();
    parameters.setParameters(svm);
    m_dbg<<"started autotraining";
    ParamGrid Cgrid(0.001,1,3);
    svm.autoTrain();
    svm.clearTrainingData();
    m_dbg<<"finished autotraining";

}

int MyPersonDetector::test()
{
    vector<Mat> images;
    vector<int> labels;
    QStringList filenames=posData;
    for (auto filename:filenames){
        images.push_back(imread(filename.toStdString(),IMREAD_GRAYSCALE));
        labels.push_back(1);
    }
    filenames=negData;
    for (auto filename:filenames){
        images.push_back(imread(filename.toStdString(),IMREAD_GRAYSCALE));
        labels.push_back(-1);
    }
    int res=0;
    for (unsigned i=0;i<images.size();++i){
        float pr=predict(images[i]);
        if (pr==labels[i])res++;
//        cout<<pr<<" found , expected "<<labels[i]<<endl;
    }
    cout<<res<<" of "<<images.size()<<endl;
    return (res);
}

float MyPersonDetector::predict(Mat img)
{
    Mat feat=getDSIFTDescriptors(img);
    return svm.predict(feat);
}

void MyPersonDetector::setDataLoader(DataLoader *loader)
{
    dataLoader=loader;
}


vector<Rect> MyPersonDetector::detectMultiScale(Mat img)
{
    vector<Rect> res;
    vector<DetectionWindow> slidingWindows=applySlidingWindow(img);
    m_dbg<<"found "<<slidingWindows.size()<<"sliding windows";
    int id=0;
    for (DetectionWindow window:slidingWindows){
        id++;
        int ans=-1;
        if (predict(window.getImageWindow())==1){
            res.push_back(window.getROI());
            ans=1;
        }

        m_dbg<<"checking "<<id<<"th window, found "<<ans;
    }
    return res;
}



void AugmentedPersonDetector::train()
{
    MyPersonDetector::train();
    setDataLoader(new AugmentedDataLoader(positiveImages,negativeImages,this));
    MyPersonDetector::train();
}
