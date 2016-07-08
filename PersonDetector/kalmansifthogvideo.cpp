#include "kalmansifthogvideo.h"
#include "my_utilties.h"
#include <opencv2/highgui.hpp>
#include <set>
KalmanSiftHogVideo::KalmanSiftHogVideo(cv::HOGDescriptor *hog, cv::VideoCapture *src)
{
    this->hog=hog;
    source=src;
    terminated=false;
    if (!source->isOpened()){
        m_dbg<<"invalid source";
        return ;
    }
    cv::Mat frame;
    source->read(frame);
    double div=max(frame.rows/600.0,frame.cols/600.0);
    int r=frame.rows/div,c=frame.cols/div;
    m_dbg<<r<<c;
    cv::resize(frame,frame,cv::Size(c,r));
    lastFrame=frame;
    th1=std::thread(&KalmanSiftHogVideo::playThread,this);
    th2=std::thread(&KalmanSiftHogVideo::HOGDMSThread,this);
    m_dbg<<"created threads";
    //th1.join();th2.join();
}

KalmanSiftHogVideo::~KalmanSiftHogVideo()
{
    terminated=true;
    th1.join();
    th2.join();
    for (KalmanSiftTrackableFrame * f:frames){
        f->setTerminated();
        delete f;
    }

}


void KalmanSiftHogVideo::playThread()
{

//    m_dbg<<"from playthread";
    string windowName="video detection";
    cv::namedWindow(windowName+"1",1);
    cv::Mat frame;
    while (source->read(frame)&&!terminated){
//        m_dbg<<"from playthread";
        double div=max(frame.rows/600.0,frame.cols/600.0);
        int r=frame.rows/div,c=frame.cols/div;
        cv::resize(frame,frame,cv::Size(c,r));
        lastFrame=frame.clone();
        cv::Mat frame1=drawCircles(frame);
        cv::imshow(windowName+"1",frame1);

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    cv::destroyWindow(windowName+"1");
    source->release();
    terminated=true;
    for (KalmanSiftTrackableFrame * f:frames){
        f->setTerminated();
//        delete f;
    }

}

cv::Mat KalmanSiftHogVideo::drawCircles(cv::Mat frame){
    cv::Mat res=frame.clone();
//    set<TrackableFrame *> s;
    for (KalmanSiftTrackableFrame * f:frames){
        if (f->isOutOfSync()){
            continue;
        }
        pair<int,int> p=f->getKalmanPrediction();
        cv::Point pp;
        pp.x=p.first;pp.y=p.second;
        cv::circle(res,pp,10,cv::Scalar(0,255,0));
    }
//    mtx.lock();
//    for (TrackableFrame *f:s){
//        f->joinThread();
//        m_dbg<<"joined, will delete";
//        //delete f;
//        //m_dbg<<"deleted";
//        frames.erase(f);
//        m_dbg<<"erased";
//    }
//    mtx.unlock();
    //    std::list<TrackableFrame *>::iterator i=frames.begin();
    //    while (i!=l.end()){
    //        if (s.count(*i)){
    //            l.erase(i++);
    //        }
    //        else {
    //            i++;
    //        }
    //    }
    return res;
}

void KalmanSiftHogVideo::HOGDMSThread(){
    while (!terminated){
//        for (TrackableFrame *f:frames){
//            f->joinThread();
//            //delete f;
//        }
//        frames.clear();
//        m_dbg<<"from hogdmsthread";
        std::vector<cv::Rect> rects;
        cv::Mat frame=getCurrentFrame();
        cv::cvtColor(frame,frame,cv::COLOR_BGR2GRAY);
        hog->detectMultiScale(frame,rects);
//        m_dbg<<rects.size();
//        mtx.lock();
        for (cv::Rect r:rects){
            bool found=false;
            for (KalmanSiftTrackableFrame *f:frames){
                if (f->isOutOfSync()){
                    continue;
                }
                if (f->chekcForMatch(frame(r))){
                    found=true;
                    break;
                }
            }
            if (!found)
                frames.insert(new KalmanSiftTrackableFrame(this,frame(r)));
        }
//        mtx.unlock();
        std::set<KalmanSiftTrackableFrame *> toDel;
        for (KalmanSiftTrackableFrame *f:frames){
            if (f->isOutOfSync()){
                f->setTerminated();
                toDel.insert(f);
            }
        }
        for (KalmanSiftTrackableFrame *f: toDel){
            frames.erase(f);
        }
//        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    for (KalmanSiftTrackableFrame * f:frames){
        f->setTerminated();
//        delete f;
    }

}



cv::Mat KalmanSiftHogVideo::getCurrentFrame()
{
    return lastFrame.clone();
}

void KalmanSiftHogVideo::setTerminated()
{
    terminated=true;
}
