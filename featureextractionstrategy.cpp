#include "featureextractionstrategy.h"
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <opencv2/objdetect.hpp>
using namespace std;
Mat SIFT::getFeatures(Mat img){
    resize(img,img,Size(64,128));
    cvtColor( img, img, COLOR_BGR2GRAY );
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

std::string SIFT::strategyName()
{
    return "SIFT";
}
Mat HOG::getFeatures(Mat img){

    resize(img,img,Size(64,128));
    HOGDescriptor hog;
    hog.winSize = Size(64,128);
    Mat gray;
    vector< Point > location;
    vector< float > descriptors;
    cvtColor( img, gray, COLOR_BGR2GRAY );
    hog.compute( gray, descriptors, Size( 8, 8 ), Size( 0, 0 ), location );
    return Mat( descriptors ).clone().t() ;
}

std::string HOG::strategyName()
{
    return "HOG";
}
