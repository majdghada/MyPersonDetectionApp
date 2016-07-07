#ifndef KALMANSIFTHOGVIDEO_H
#define KALMANSIFTHOGVIDEO_H
#include "kalmansifttrackableframe.h"
#include <QObject>

class KalmanSiftTrackableFrame;


class KalmanSiftHogVideo
{
private :
    cv::HOGDescriptor *hog;
    std::set<KalmanSiftTrackableFrame*> frames;
    cv::Mat lastFrame;
    cv::VideoCapture *source;
    void playThread();
    cv::Mutex mtx;
    std::thread th1,th2;
    cv::Mat drawCircles(cv::Mat frame);
    void HOGDMSThread();
    bool terminated;
public:
    KalmanSiftHogVideo(cv::HOGDescriptor * hog, cv::VideoCapture * src);
    ~KalmanSiftHogVideo();
    cv::Mat getCurrentFrame();

};


#endif // KALMANSIFTHOGVIDEO_H
