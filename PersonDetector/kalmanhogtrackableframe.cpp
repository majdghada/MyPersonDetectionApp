#include "kalmanhogtrackableframe.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"
template<typename T>
inline void maximizewith(T & variable, T val){
    variable=max(variable,val);
}
template<typename T>
inline void minimizewith(T & variable, T val){
    variable=min(variable,val);
}
double dist3dSqr(double x1,double y1,double z1,double x2,double y2,double z2){
    return (x1-x2)*(x1-x2)+
            (y1-y2)*(y1-y2)+
            (z1-z2)*(z1-z2);

}

void KalmanHogTrackableFrame::matchingThread(){

    cv::Mat currentFrame;
    float x=0,y=0,z=0;
    while (!terminated){
//        std::this_thread::sleep_for(std::chrono::milliseconds(1));
//        m_dbg<<"from matchingthread";
        currentFrame=video->getCurrentFrame();
        cv::cvtColor(currentFrame,currentFrame,cv::COLOR_BGR2GRAY);
        cv::Rect extendedRoi;
        extendedRoi.x=curState.x-curState.z;
        extendedRoi.y=curState.y-curState.z;
        extendedRoi.width=curState.z*3;
        extendedRoi.height=curState.z*4;
        maximizewith(extendedRoi.x,0);
        maximizewith(extendedRoi.y,0);
        maximizewith(extendedRoi.width,0);
        maximizewith(extendedRoi.height,0);
        minimizewith(extendedRoi.x,currentFrame.cols);
        minimizewith(extendedRoi.y,currentFrame.rows);
        minimizewith(extendedRoi.width,currentFrame.cols-extendedRoi.x);
        minimizewith(extendedRoi.height,currentFrame.rows-extendedRoi.y);
        std::vector<cv::Rect> foundrects;
        hog->detectMultiScale(currentFrame(extendedRoi),foundrects);
        if (foundrects.size()==0){
            noMatchCount++;
            continue;
        }
        noMatchCount=0;

        double minDist=1e9;
        int nearestidx=0;
        for (int i=0;i<foundrects.size();++i){
            cv::Rect & r=foundrects[i];
            double curdist=dist3dSqr(r.x+extendedRoi.x,r.y+extendedRoi.y,r.width,curState.x,curState.y,curState.z);
            if (curdist<minDist){
                minDist=curdist;
                nearestidx=i;
            }
        }
        cv::Rect & nearest=foundrects[nearestidx];
        x=nearest.x+extendedRoi.x;
        y=nearest.y+extendedRoi.y;
        z=nearest.width;
        mtx.lock();
        curState=kalmanCorrect(x,y,z);
        mtx.unlock();


    }
}

void KalmanHogTrackableFrame::initKalman(float x, float y, float z)
{
    // Instantate Kalman Filter with
    // 4 dynamic parameters and 2 measurement parameters,
    // where my measurement is: 2D location of object,
    // and dynamic is: 2D location and 2D velocity.
    KF.init(9, 3, 0);

    measurement = cv::Mat_<float>::zeros(3,1);
    measurement.at<float>(0, 0) = x;
    measurement.at<float>(1, 0) = y;
    measurement.at<float>(2, 0) = z;

    KF.statePre.setTo(0);
    KF.statePre.at<float>(0, 0) = x;
    KF.statePre.at<float>(1, 0) = y;
    KF.statePre.at<float>(2, 0) = z;

    KF.statePost.setTo(0);
    KF.statePost.at<float>(0, 0) = x;
    KF.statePost.at<float>(1, 0) = y;
    KF.statePost.at<float>(2, 0) = z;
    int a=2,v=2;
    KF.transitionMatrix = (cv::Mat_<float>(9, 9) << 1, 0, 0, v, 0, 0, a, 0, 0,
                                                    0, 1, 0, 0, v, 0, 0, a, 0,
                                                    0, 0, 1, 0, 0, v, 0, 0, a,
                                                    0, 0, 0, 1, 0, 0, v, 0, 0,
                                                    0, 0, 0, 0, 1, 0, 0, v, 0,
                                                    0, 0, 0, 0, 0, 1, 0, 0, v,
                                                    0, 0, 0, 0, 0, 0, 1, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0, 1, 0,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 1);
    cv::setIdentity(KF.measurementMatrix);
    cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(.005)); //adjust this for faster convergence - but higher noise
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(.1));
}

cv::Point3f KalmanHogTrackableFrame::kalmanPredict()
{
    cv::Mat prediction = KF.predict();
    cv::Point3f predictPt(prediction.at<float>(0),prediction.at<float>(1),prediction.at<float>(2));
    return predictPt;
}

cv::Point3f KalmanHogTrackableFrame::kalmanCorrect(float x, float y, float z)
{
    measurement(0) = x;
    measurement(1) = y;
    measurement(2) = z;
    cv::Mat estimated = KF.correct(measurement);
    cv::Point3f statePt(estimated.at<float>(0),estimated.at<float>(1),estimated.at<float>(2));
    return statePt;
}

KalmanHogTrackableFrame::KalmanHogTrackableFrame(KalmanHogVideo *video, cv::Rect roi)
{
    this->video=video;
    this->curState.x=roi.x;
    this->curState.y=roi.y;
    this->curState.z=roi.width;

//    m_dbg<<"from matchingthread";
    hog=video->getHog();
    initKalman(curState.x,curState.y,curState.z);

    terminated=false;
    noMatchCount=0;
    th = std::thread(&KalmanHogTrackableFrame::matchingThread,this);

}

cv::Rect KalmanHogTrackableFrame::getKalmanPrediction()
{
    mtx.lock();
    cv::Point3f p=kalmanPredict();
    mtx.unlock();
    cv::Rect res;
    res.x=p.x;
    res.y=p.y;
    res.width=p.z;
    res.height=p.z*2;
    return res;
}

bool KalmanHogTrackableFrame::isOutOfSync()
{
    return noMatchCount>6;
}

void KalmanHogTrackableFrame::setTerminated()
{
    if (terminated)return;
    terminated=true;
    //th.join();
}
float KalmanHogTrackableFrame::getArea(){
    return curState.z*curState.z*2;
}

bool KalmanHogTrackableFrame::checkContains(KalmanHogTrackableFrame *f)
{
    // TODO : check if shared area is more than 75 of one of them .
    float sx=max(f->curState.x,curState.x);
    float sy=max(f->curState.y,curState.y);
    float ex=min(f->curState.x+f->curState.z,curState.x+curState.z);
    float ey=min(f->curState.y+f->curState.z*2,curState.y+curState.z*2);
    float width=ex-sx;
    float height=ey-sy;
    if (width<=0||height<=0){
        return false ;
    }
    float sharea=width*height;
    float myarea=getArea();
    float farea=f->getArea();
    return (sharea>=0.95*farea);

}

KalmanHogTrackableFrame::~KalmanHogTrackableFrame()
{
    setTerminated();
}
