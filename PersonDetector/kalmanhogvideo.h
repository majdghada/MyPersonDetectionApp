#ifndef KALMANHOGVIDEO_H
#define KALMANHOGVIDEO_H

#include "kalmanhogtrackableframe.h"

class KalmanHogTrackableFrame;

class KalmanHogVideo
{
private:
    cv::HOGDescriptor *hog;
    std::set<KalmanHogTrackableFrame*> frames;
    cv::Mat lastFrame;
    cv::VideoCapture *source;
    void playThread();
    cv::Mutex mtx;
    std::thread th1,th2;
    cv::Mat drawRects(cv::Mat frame);
    void HOGDMSThread();
    bool terminated;
public:
    KalmanHogVideo(cv::HOGDescriptor * hog, cv::VideoCapture * src);
    ~KalmanHogVideo();
    cv::Mat getCurrentFrame();
    void setTerminated();

    cv::HOGDescriptor *getHog() const;
};

#endif // KALMANHOGVIDEO_H
