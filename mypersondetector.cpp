#include "mypersondetector.h"
#include <vector>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "mainwindow.h"
#include "detectionwindow.h"
#include <QtDebug>
#include <thread>
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
    fxs=new SIFT();
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
        Mat img=imread(filename.toStdString());
        positiveImages.push_back(img);
    }
    m_dbg<<"finished loading positive";
    filenames=&negData;
    for (QString filename:*filenames){
        Mat img=imread(filename.toStdString());
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

void MyPersonDetector::setFeatureExtractionStrategy(FeatureExtractionStrategy *fxs)
{
    this->fxs=fxs;
    m_dbg<<" will use "<<QString(fxs->strategyName().c_str());
}
//Mat getDSIFTDescriptors(Mat img){

//    resize(img,img,Size(64,128));
//    Ptr<xfeatures2d::SIFT> sift = xfeatures2d::SIFT::create();
//    Ptr<xfeatures2d::SIFT> sift1 = xfeatures2d::SIFT::create(4);
//    vector <KeyPoint> kps1;
//    Mat des1;

//    sift1->detect(img,kps1);
//    sift1->compute(img,kps1,des1);

//    Mat res=des1.t();
//    int rows=8,cols=6;
//    for (int i=0;i<rows;++i){
//        for (int j=0;j<cols;++j){
//            Range rowrange(i*img.rows/rows,(i+1)*img.rows/rows-1),
//                  colrange(j*img.cols/cols,(j+1)*img.cols/cols-1);
//            Mat subimg=img(rowrange,colrange);
//            vector<KeyPoint> kps;
//            Mat des,labels,centers;
//            sift->detect(subimg,kps);
//            if (kps.size()==0){
//                kps.push_back(KeyPoint((colrange.start+colrange.end)/2.0,
//                                       (rowrange.start+rowrange.end)/2.0,
//                                       colrange.size()));
//            }
//            sift->compute(subimg,kps,des);
//            if (des.rows==1)
//                centers=des;
//            else
//                kmeans(des,1,labels,TermCriteria( TermCriteria::EPS+TermCriteria::COUNT,
//                                                  10, 1.0),1,KMEANS_PP_CENTERS, centers);
//            if (i==0&&j==0){
//                res=centers.t();
//            }
//            else {
//                res.push_back(centers.t());
//            }

//        }
//    }

//    return res.t();
//}

SVMParameters & MyPersonDetector::getParameters()
{
    return parameters;
}

void MyPersonDetector::setParameters(const SVMParameters &value)
{
    parameters = value;
}
void trainigData_preperation_thread(FeatureExtractionStrategy * fxs,vector<Mat>* images,vector<int>* labels,int st,int en,MySVM * svm,Mutex *mtx){
    for (int i=st;i<en;++i){
        Mat feat=fxs->getFeatures((*images)[i]);
        m_dbg<<"finished the "<<i+1<<"th image";
        mtx->lock();
        svm->addExample(feat,(*labels)[i]);
        mtx->unlock();
    }
}

void MyPersonDetector::InitSVMTrainingData(){
    std::vector<Mat> images;
    std::vector<int> labels;

    while (dataLoader->hasNext()){
        pair<Mat,int> data=dataLoader->next();
        Mat img;
        resize(data.first,img,Size(64,128));
        images.push_back(img);
        labels.push_back(data.second);
    }
    m_dbg<<images.size()<<" images ready for feature extraction ";
    svm.clearTrainingData();
    int sz=images.size()/8;
    std::thread * threads[8];
    Mutex mtx;
    for (int i=0;i<8;++i){
        threads[i]=new std::thread(trainigData_preperation_thread,fxs,&images,&labels,sz*i,(i==7)?images.size():sz*(i+1),&svm,&mtx);
    }
    for (int i=0;i<8;++i){
        threads[i]->join();
        delete threads[i];
    }
//    for (int i=0;i<images.size();++i){
//        Mat feat=fxs->getFeatures((images)[i]);
//        m_dbg<<"finished the "<<i+1<<"th image";
////        mtx->lock();
//        svm.addExample(feat,(labels)[i]);
////        mtx->unlock();
//    }
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
    for (QString filename:filenames){
        images.push_back(imread(filename.toStdString()));
        labels.push_back(1);
    }
    filenames=negData;
    for (QString filename:filenames){
        images.push_back(imread(filename.toStdString()));
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
    Mat feat=fxs->getFeatures(img);
    return svm.predict(feat);
}

void MyPersonDetector::setDataLoader(DataLoader *loader)
{
    dataLoader=loader;
}

void prediction_thread(MyPersonDetector * detector , vector<DetectionWindow>* slidingWindows,vector<DetectionWindow>*res,int st,int en,Mutex *mtx){
    for (int i=st;i<en;++i){
        int ans=-1;
        double pr=detector->predict((*slidingWindows)[i].getImageWindow());
        if (isPositiveClass(pr)){
            mtx->lock();
            res->push_back((*slidingWindows)[i]);
            mtx->unlock();
            ans=1;
        }
        m_dbg<<"checking "<<i<<"th window, found "<<ans;
    }
}

vector<DetectionWindow> MyPersonDetector::detectMultiScale(Mat img)
{

    vector<DetectionWindow> res;
    vector<DetectionWindow> slidingWindows=applySlidingWindow(img,64,128,min(img.rows/50,img.cols/50));
    m_dbg<<"found "<<slidingWindows.size()<<"sliding windows";
    int sz=slidingWindows.size()/8;
    std::thread * threads[8];
    Mutex  mtx;
    for (int i=0;i<8;++i){
        threads[i]=new std::thread(prediction_thread,this,&slidingWindows,&res,sz*i,(i==7)?slidingWindows.size():sz*(i+1),&mtx);
    }

    for (int i=0;i<8;++i){
        threads[i]->join();
        delete threads[i];
    }
    return res;
}



void AugmentedPersonDetector::train()
{
    MyPersonDetector::train();
    setDataLoader(new AugmentedDataLoader(positiveImages,negativeImages,this));
    MyPersonDetector::train();
}
