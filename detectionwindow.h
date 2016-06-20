#ifndef DETECTIONWINDOW_H
#define DETECTIONWINDOW_H

#include <opencv2/core.hpp>
#include <vector>
using namespace cv;
class DetectionWindow
{
private:
    Mat image;
    Rect roi;
public:
    DetectionWindow(const Mat &img,const Rect & roi);
    DetectionWindow(const DetectionWindow & detectionWindow);
    DetectionWindow(const Mat &img,const int &startRow,const int &startCol,const int &width,const int &height);
    DetectionWindow(const Mat &img,const Range &rowRange,const Range &colRange);
    Mat getImageWindow();
    Rect getROI();
    Mat getFullImage();
};
Mat getCentral64x128Image(const Mat & img);
DetectionWindow getCentral64x128Window(const Mat & img);

/*!
  \fn std::vector<DetectionWindow> applySlidingWindow(const Mat &img,int minwidth=64,int minheight=128,int stride=8,float scaleRatio=1.2,float heightwidthratio=2)
    returns a vector of \c {DetectionWindow}s
    \a img : the image you want to get its windows
    \a minheight : minimum window height
    \a minwidth : minimum window height
    \a stride : step size ( number of pixels )
    \a scaleRatio : the scale ratio between two consecutive scale levels  ( should be more than one )
    \a heigtwidthratio : the ratio (heigt / width) of each window

*/
std::vector<DetectionWindow> applySlidingWindow(const Mat &img,int minwidth=64,int minheight=128,int stride=8,float scaleRatio=1.02,float heightwidthratio=2,int maxLevels=13);

#endif // DETECTIONWINDOW_H
