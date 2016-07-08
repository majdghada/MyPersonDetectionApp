#ifndef KALMANSIFTTRACKABLEFRAME_H
#define KALMANSIFTTRACKABLEFRAME_H
#include <opencv2/videoio.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>
#include <vector>
#include <thread>
#include <list>
#include <opencv2/highgui.hpp>
#include "my_utilties.h"
#include "kalmansifthogvideo.h"

class KalmanSiftHogVideo;
class KalmanSiftTrackableFrame{
private:
    cv::Ptr<cv::Feature2D> sift;
    cv::KalmanFilter KF;
    cv::Mat objSiftDescriptor;
    cv::Mat ObjectToTrack;
    std::thread th;
    KalmanSiftHogVideo * video;
    void matchingThread();
    bool terminated;
    int noMatchCount;
    cv::Mat_<float> measurement;
    cv::Mat_<float> state;
    void initKalman(float x, float y);
    cv::Point kalmanPredict();
    cv::Point kalmanCorrect(float x, float y);
    cv::Mutex mtx;
    std::vector<cv::KeyPoint> kps;




public :
    KalmanSiftTrackableFrame(KalmanSiftHogVideo * video,cv::Mat ObjectToTrack);
    std::pair<int,int> getKalmanPrediction();
    bool isOutOfSync();
    void setTerminated();
    bool chekcForMatch(cv::Mat frame);
    ~KalmanSiftTrackableFrame();

};
#endif // KALMANSIFTTRACKABLEFRAME_H
