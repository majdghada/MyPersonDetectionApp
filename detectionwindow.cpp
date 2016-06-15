#include "detectionwindow.h"
#include "mainwindow.h"
using namespace std;
DetectionWindow::DetectionWindow(const Mat & image,const Rect &roi){
    this->image=image;
    this->roi=roi;
}

DetectionWindow::DetectionWindow(const DetectionWindow &detectionWindow)
{
    image=detectionWindow.image;
    roi=detectionWindow.roi;
}

DetectionWindow::DetectionWindow(const Mat &image,const int & startRow,const int & startCol,const int & height,const int & width){
    this->image=image;
    this->roi.x=startCol;
    this->roi.y=startRow;
    this->roi.width=width;
    this->roi.height=height;

}


Mat getCentral64x128Image(const Mat & img){

    return getCentral64x128Window(img).getImageWindow();
}
DetectionWindow getCentral64x128Window(const Mat & img){
    Rect roi;
    roi.width=64;
    roi.height=128;
    roi.x=img.cols/2-roi.width/2;
    roi.y=img.rows/2-roi.height/2;
    return(DetectionWindow(img,roi));
}

DetectionWindow::DetectionWindow(const Mat &image,const Range & rowRange,const Range & colRange){
    this->image=image;
    this->roi.x=colRange.start;
    this->roi.y=rowRange.start;
    this->roi.width=colRange.end-colRange.start+1;
    this->roi.height=rowRange.end-rowRange.start+1;

}

Mat DetectionWindow::getImageWindow(){
    try{
        return image(roi);
    }
    catch(cv::Exception e){
        m_dbg<< "roi ="<<roi.x<<roi.y<<roi.height<<roi.width;
        m_dbg<< "img ="<<image.rows<<image.cols;
        throw e;
    }
}
Mat DetectionWindow::getFullImage(){
    return image;
}
Rect DetectionWindow::getROI(){
    return roi;
}




std::vector<DetectionWindow> applySlidingWindow(const Mat &img,int minwidth,int minheight,int stride,float scaleRatio,float heightwidthratio,int maxLevels)
{
    vector<DetectionWindow> res;
//    scaleRatio=img.cols/(double)minheight;
//    for (int i=0;i<4;++i)scaleRatio=sqrt(scaleRatio);
    int level=0;

    for (float scale=1.0;level<maxLevels;scale*=scaleRatio,level++){
        int cols=img.cols/scale;
        int rows=cols*heightwidthratio;
        if (rows<minheight||cols<minwidth)break;
        int rowShift=((img.rows-rows)%stride)/2;
        int colShift=((img.cols-cols)%stride)/2;
        for (int row=rowShift;row+rows<=img.rows;row+=stride){
            for (int col=colShift;col+cols<=img.cols;col+=stride){
                res.push_back(DetectionWindow(img,row,col,rows,cols));
            }
        }
    }
    return res;
}
