#include "kalmansifttrackableframe.h"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"


void KalmanSiftTrackableFrame::matchingThread(){

    cv::Mat currentFrame;
    std::vector<cv::KeyPoint> frameKps;
    cv::Mat frameDesc;
    std::vector<std::vector<cv::DMatch> > matches;
    cv::BFMatcher matcher;
    std::vector<cv::KeyPoint> matchKps;
    float x=0,y=0;
    while (!terminated){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
//        m_dbg<<"from matchingthread";
        currentFrame=video->getCurrentFrame();
        cv::cvtColor(currentFrame,currentFrame,cv::COLOR_BGR2GRAY);
        frameKps.clear();
        sift->detect(currentFrame,frameKps);
        sift->compute(currentFrame,frameKps,frameDesc);
        matcher.knnMatch(objSiftDescriptor,frameDesc,matches,2);
        matchKps.clear();
        std::vector<std::vector<cv::DMatch> > toDraw(1);
        for (std::vector<cv::DMatch> a:matches){
            if (a.size()<2)continue;
            if (a[0].distance<a[1].distance*0.75){
                matchKps.push_back(frameKps[a[0].trainIdx]);
                toDraw[0].push_back(a[0]);
            }
        }
//        sort(toDraw[0].begin(),toDraw[0].end(),[](cv::DMatch a,cv::DMatch b){
//            return a.distance<b.distance;
//        });
        m_dbg<<matchKps.size()<<"matches";

        if (matchKps.size()<4){
            noMatchCount++;
            continue;
        }

//        toDraw[0]=std::vector<cv::DMatch> (toDraw[0].begin(),toDraw[0].begin()+3);
        noMatchCount=0;
        x=0,y=0;
        for (cv::KeyPoint p:matchKps){
            x+=p.pt.x;
            y+=p.pt.y;
        }
        x/=matchKps.size();
        y/=matchKps.size();
        mtx.lock();
        cv::Point center=kalmanCorrect(x,y);
        mtx.unlock();




        cv::Mat out;
//        mtx.lock();
//        try{
//        cv::drawMatches(ObjectToTrack,kps,currentFrame,frameKps,toDraw,out,
//                        cv::Scalar::all(-1),cv::Scalar::all(-1),std::vector<std::vector<char> > (),
//                        cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS|cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//        //-- Localize the object
//        std::vector<cv::Point2f> obj;
//        std::vector<cv::Point2f> scene;
//        std::vector<cv::DMatch>&good_matches=toDraw[0];
//        std::vector<cv::KeyPoint>&keypoints_object=kps;
//        std::vector<cv::KeyPoint>&keypoints_scene=frameKps;
//        cv::Mat & img_object=ObjectToTrack;
//        cv::Mat & img_matches=out;
//        for( size_t i = 0; i < good_matches.size(); i++ )
//        {
//          //-- Get the keypoints from the good matches
//          obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
//          scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
//        }
//        cv::Mat H = cv::findHomography( obj, scene, cv::RANSAC );
//        //-- Get the corners from the image_1 ( the object to be "detected" )
//        std::vector<cv::Point2f> obj_corners(4);
//        obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( img_object.cols, 0 );
//        obj_corners[2] = cvPoint( img_object.cols, img_object.rows ); obj_corners[3] = cvPoint( 0, img_object.rows );
//        std::vector<cv::Point2f> scene_corners(4);
//        cv::perspectiveTransform( obj_corners, scene_corners, H);
//        //-- Draw lines between the corners (the mapped object in the scene - image_2 )
//        cv::line( img_matches, scene_corners[0] + cv::Point2f( img_object.cols, 0), scene_corners[1] + cv::Point2f( img_object.cols, 0), cv::Scalar(0, 255, 0), 4 );
//        cv::line( img_matches, scene_corners[1] + cv::Point2f( img_object.cols, 0), scene_corners[2] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );
//        cv::line( img_matches, scene_corners[2] + cv::Point2f( img_object.cols, 0), scene_corners[3] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );
//        cv::line( img_matches, scene_corners[3] + cv::Point2f( img_object.cols, 0), scene_corners[0] + cv::Point2f( img_object.cols, 0), cv::Scalar( 0, 255, 0), 4 );
//        //-- Show detected matches
//        cv::imshow( "Good Matches & Object detection", img_matches );
//        }
//        catch( cv::Exception e){

//        }

////        mtx.unlock();

//        ObjectToTrack=currentFrame(cv::Range(max(0,center.y-ObjectToTrack.rows/2),min(currentFrame.rows,center.y+ObjectToTrack.rows/2)),
//                                   cv::Range(max(0,center.x-ObjectToTrack.cols/2),min(currentFrame.cols,center.x+ObjectToTrack.cols/2)));
//        kps.clear();
//        sift->detect(ObjectToTrack,kps);
//        sift->compute(ObjectToTrack,kps,this->objSiftDescriptor);




    }
}

void KalmanSiftTrackableFrame::initKalman(float x, float y)
{
    // Instantate Kalman Filter with
    // 4 dynamic parameters and 2 measurement parameters,
    // where my measurement is: 2D location of object,
    // and dynamic is: 2D location and 2D velocity.
    KF.init(4, 2, 0);

    measurement = cv::Mat_<float>::zeros(2,1);
    measurement.at<float>(0, 0) = x;
    measurement.at<float>(0, 0) = y;


    KF.statePre.setTo(0);
    KF.statePre.at<float>(0, 0) = x;
    KF.statePre.at<float>(1, 0) = y;

    KF.statePost.setTo(0);
    KF.statePost.at<float>(0, 0) = x;
    KF.statePost.at<float>(1, 0) = y;

    KF.transitionMatrix = (cv::Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
    cv::setIdentity(KF.measurementMatrix);
    cv::setIdentity(KF.processNoiseCov, cv::Scalar::all(.05)); //adjust this for faster convergence - but higher noise
    cv::setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(KF.errorCovPost, cv::Scalar::all(.1));
}

cv::Point KalmanSiftTrackableFrame::kalmanPredict()
{
    cv::Mat prediction = KF.predict();
    cv::Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
    return predictPt;
}

cv::Point KalmanSiftTrackableFrame::kalmanCorrect(float x, float y)
{
    measurement(0) = x;
    measurement(1) = y;
    cv::Mat estimated = KF.correct(measurement);
    cv::Point statePt(estimated.at<float>(0),estimated.at<float>(1));
    return statePt;
}

KalmanSiftTrackableFrame::KalmanSiftTrackableFrame(KalmanSiftHogVideo *video, cv::Mat ObjectToTrack)
{
    this->video=video;
    this->ObjectToTrack=ObjectToTrack;

//    m_dbg<<"from matchingthread";
    sift=cv::xfeatures2d::SIFT::create();
    sift->detect(ObjectToTrack,kps);
    sift->compute(ObjectToTrack,kps,this->objSiftDescriptor);

    cv::Mat currentFrame=video->getCurrentFrame().clone();
    cv::cvtColor(currentFrame,currentFrame,cv::COLOR_BGR2GRAY);
    std::vector<cv::KeyPoint> frameKps;
    cv::Mat frameDesc;
    sift->detect(currentFrame,frameKps);
    sift->compute(currentFrame,frameKps,frameDesc);
    std::vector<std::vector<cv::DMatch> > matches;
    cv::BFMatcher matcher;
    matcher.knnMatch(objSiftDescriptor,frameDesc,matches,2);

    std::vector<cv::KeyPoint> matchKps;
    for (std::vector<cv::DMatch> a:matches){
        if (a.size()<2)continue;
        if (a[0].distance<a[1].distance*0.75){
            matchKps.push_back(frameKps[a[0].trainIdx]);
        }
    }
    if (matchKps.size()<4){
        terminated=true;
        noMatchCount=100;
        return;
    }
    float x=0,y=0;
    for (cv::KeyPoint p:matchKps){
        x+=p.pt.x;
        y+=p.pt.y;
    }
    x/=matchKps.size();
    y/=matchKps.size();

    initKalman(x,y);

    terminated=false;
    noMatchCount=0;




    th = std::thread(&KalmanSiftTrackableFrame::matchingThread,this);

}

std::pair<int, int> KalmanSiftTrackableFrame::getKalmanPrediction()
{
    mtx.lock();
    cv::Point p=kalmanPredict();
    mtx.unlock();
    return std::make_pair((int)p.x,(int)p.y);
}

bool KalmanSiftTrackableFrame::isOutOfSync()
{
    return noMatchCount>2;
}

void KalmanSiftTrackableFrame::setTerminated()
{
    if (terminated)return;
    terminated=true;
    //th.join();
}

bool KalmanSiftTrackableFrame::chekcForMatch(cv::Mat frame)
{
    cv::Mat currentFrame=frame.clone();
//    cv::cvtColor(currentFrame,currentFrame,cv::COLOR_BGR2GRAY);
    std::vector<cv::KeyPoint> frameKps;
    cv::Mat frameDesc;
    sift->detect(currentFrame,frameKps);
    sift->compute(currentFrame,frameKps,frameDesc);
    std::vector<std::vector<cv::DMatch> > matches;
    cv::BFMatcher matcher;
    matcher.knnMatch(objSiftDescriptor,frameDesc,matches,2);

    std::vector<cv::KeyPoint> matchKps;
    for (std::vector<cv::DMatch> a:matches){
        if (a.size()<2)continue;
        if (a[0].distance<a[1].distance*0.65){
            matchKps.push_back(frameKps[a[0].trainIdx]);
        }
    }
    return matchKps.size()>=4;
}

KalmanSiftTrackableFrame::~KalmanSiftTrackableFrame()
{
    setTerminated();
}
