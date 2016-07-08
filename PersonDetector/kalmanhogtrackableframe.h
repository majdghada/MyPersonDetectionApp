#ifndef KALMANHOGTRACKABLEFRAME_H
#define KALMANHOGTRACKABLEFRAME_H
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
#include "kalmanhogvideo.h"
#include <opencv2/objdetect.hpp>

class KalmanHogVideo;

class KalmanHogTrackableFrame
{
private:
    cv::HOGDescriptor *hog;
    cv::Point3f curState;
    cv::KalmanFilter KF;
    std::thread th;
    KalmanHogVideo * video;
    void matchingThread();
    bool terminated;
    int noMatchCount;
    cv::Mat_<float> measurement;
    cv::Mat_<float> state;
    void initKalman(float x, float y,float z);
    cv::Point3f kalmanPredict();
    cv::Point3f kalmanCorrect(float x, float y, float z);
    cv::Mutex mtx;


public:
    KalmanHogTrackableFrame(KalmanHogVideo * video, cv::Rect roi);
    cv::Rect getKalmanPrediction();
    bool isOutOfSync();
    void setTerminated();
    bool checkContains(KalmanHogTrackableFrame * f);
    ~KalmanHogTrackableFrame();
    float getArea();
};

#endif // KALMANHOGTRACKABLEFRAME_H
