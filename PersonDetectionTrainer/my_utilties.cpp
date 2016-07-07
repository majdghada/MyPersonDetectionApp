#include "my_utilties.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include <QMessageBox>
#include "mypersondetector.h"
#include "svmparameters.h"
#include <list>
#include <set>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

QTime myTimer;
void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    hog_detector.clear();

    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}



bool isNegativeClass(double weight)
{
    return weight<=-0.05;
}

bool isPositiveClass(double weight)
{
    return weight>=0.05;
}
cv::Mat getDispImg(MyPersonDetector *detector,Mat img){
    vector<DetectionWindow> boxes=detector->detectMultiScale(img);
    vector<Rect> rects;
    vector<double> weights;
    for (DetectionWindow box:boxes){
        rects.push_back(box.getROI());
        weights.push_back(box.getPrediction());
    }

    Mat dispImg=img.clone();
    for (Rect rect:rects){
        cv::Mat roi = dispImg(rect);
        cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(0, 255,0));
        double alpha = 0.01;
        cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
           // rectangle(dispImg,rect,cv::Scalar(0,255,0.1));
    }
    m_dbg<<"before group"<<rects.size();
    //groupRectangles(rects,0,0.8);
    rects=nonMaximumSupression(rects,0.5,weights);
    m_dbg<<"after group"<<rects.size();
    addNMSRects(rects,dispImg);
    return dispImg;
}
void addNMSRects(vector<Rect> rects, Mat dispImg)
{
    for (Rect rect:rects){
            rectangle(dispImg,rect,cv::Scalar(255,10,10));
    }
}
cv::Mat getDispImg(HOGDescriptor *hog,Mat img){
    vector<Rect> rects;
    hog->detectMultiScale(img,rects);
//    vector<double> weights;
//    for (DetectionWindow box:boxes){
//        rects.push_back(box.getROI());
//        weights.push_back(box.getPrediction());
//    }

    Mat dispImg=img.clone();
    for (Rect rect:rects){
        cv::Mat roi = dispImg(rect);
        cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(0, 255,0));
        double alpha = 0.1;
        cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
           // rectangle(dispImg,rect,cv::Scalar(0,255,0.1));
    }
//    m_dbg<<"before group"<<rects.size();
    //groupRectangles(rects,0,0.8);
//    rects=nonMaximumSupression(rects,0.5,weights);
//    m_dbg<<"after group"<<rects.size();
    addNMSRects(rects,dispImg);
    return dispImg;
}

void setAppTheme(QApplication *a)
{
    a->setStyle(QStyleFactory::create("fusion"));
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53).darker(200));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    palette.setColor(QPalette::Highlight, QColor(142,45,197).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    a->setPalette(palette);
}

void loadDetectorFromFile(MyPersonDetector &detector, string filename,QWidget *parent){
    detector.loadSVM(filename);
    QFile typeFile(QString("%1.type").arg((filename.c_str())));
    typeFile.open(QIODevice::ReadOnly);
    QTextStream typeStream(&typeFile);
    if ((typeStream.readAll().toInt())==1)
        detector.setFeatureExtractionStrategy(new HOG());
    else
        detector.setFeatureExtractionStrategy(new SIFT());
    typeFile.close();
    QMessageBox dia(QMessageBox::Information, "info", "Detector loaded", QMessageBox::Ok,parent);
    dia.exec();
}

vector<Rect> nonMaximumSupression(vector<Rect> rects, double overlapThreshold, vector<double> weights)
{
    int sz=rects.size();
    if (sz==0)return rects;
    vector<int>pick;
    vector<int> x1(sz),
                x2(sz),
                y1(sz),
                y2(sz),
                area(sz),
                idxsVec;
    for (int i=0;i<sz;++i){
        x1[i]=rects[i].x;
        y1[i]=rects[i].y;
        x2[i]=rects[i].x+rects[i].width-1;
        y2[i]=rects[i].y+rects[i].height-1;
        area[i]=rects[i].width*rects[i].height;
    }
    if (weights.size()==0)
        idxsVec=compute_order(y2);
    else
        idxsVec=compute_order(weights);
    list<int> idxs;
    std::copy( idxsVec.begin(), idxsVec.end(), std::back_inserter( idxs ) );
    while (idxs.size()>0){
        int last=idxs.size()-1;
        int i=idxs.back();
        pick.push_back(i);

        set<int> suppress={last};
        list<int>::iterator iter=idxs.begin();
        for (int pos=0;pos<last;++pos){
            int j=*iter;
            iter++;
            int xx1 = max(x1[i], x1[j]);
            int yy1 = max(y1[i], y1[j]);
            int xx2 = min(x2[i], x2[j]);
            int yy2 = min(y2[i], y2[j]);
            int w = max(0, xx2 - xx1 + 1);
            int h = max(0, yy2 - yy1 + 1);
            double overlap = float(w * h) / area[j];
            if (overlap > overlapThreshold)
                suppress.insert(pos);
        }
        ListRemover::removeIndexesFromList(idxs,suppress);
    }
    vector<Rect>res;
    for (int id:pick){
        res.push_back(rects[id]);
    }
    return res;
}

