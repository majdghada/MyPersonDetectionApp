#include "kalmanhogvideo.h"
#include "my_utilties.h"
#include <opencv2/highgui.hpp>
#include <set>
KalmanHogVideo::KalmanHogVideo(cv::HOGDescriptor *hog, cv::VideoCapture *src)
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
    th1=std::thread(&KalmanHogVideo::playThread,this);
    th2=std::thread(&KalmanHogVideo::HOGDMSThread,this);
    m_dbg<<"created threads";
    //th1.join();th2.join();
}

KalmanHogVideo::~KalmanHogVideo()
{
    terminated=true;
    th1.join();
    th2.join();
    for (KalmanHogTrackableFrame * f:frames){
        f->setTerminated();
        delete f;
    }

}


cv::HOGDescriptor *KalmanHogVideo::getHog() const
{
    return hog;
}

void KalmanHogVideo::playThread()
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
        cv::Mat frame1=drawRects(frame);
        cv::imshow(windowName+"1",frame1);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    cv::destroyWindow(windowName+"1");
    source->release();
    terminated=true;
    for (KalmanHogTrackableFrame * f:frames){
        f->setTerminated();
//        delete f;
    }

}

cv::Mat KalmanHogVideo::drawRects(cv::Mat frame){
    cv::Mat res=frame.clone();
//    set<TrackableFrame *> s;
    for (KalmanHogTrackableFrame * f:frames){
        if (f->isOutOfSync()){
            continue;
        }
        cv::Rect p=f->getKalmanPrediction();
        cv::rectangle(res,p,cv::Scalar(0,255,0));
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

void KalmanHogVideo::HOGDMSThread(){
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
//        for (KalmanHogTrackableFrame *f:frames){
//            f->setTerminated();
//        }
//        frames.clear();

        for (cv::Rect r:rects){
            KalmanHogTrackableFrame *cur=new KalmanHogTrackableFrame(this,r);
            bool found=false;
            for (KalmanHogTrackableFrame * f:frames){
                if (f->checkContains(cur)){
                    found=true;
                    break;
                }
            }
            if (!found)
//            if (frames.size()==0)
                frames.insert(cur);
        }
        set<KalmanHogTrackableFrame *> toDel;
        for (KalmanHogTrackableFrame * f:frames){
            if (f->isOutOfSync())toDel.insert(f);
            if (toDel.count(f))continue;
            for (KalmanHogTrackableFrame *f2:frames){

                if (f==f2||(f->getArea()<f2->getArea())||toDel.count(f2))continue;
                if (f->checkContains(f2)){
                    toDel.insert(f2);
                }
            }
        }
        for (KalmanHogTrackableFrame *f : toDel){
            f->setTerminated();
            frames.erase(f);
        }
//        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    for (KalmanHogTrackableFrame * f:frames){
        f->setTerminated();
//        delete f;
    }

}



cv::Mat KalmanHogVideo::getCurrentFrame()
{
    return lastFrame.clone();
}

void KalmanHogVideo::setTerminated()
{
    terminated=true;
}
