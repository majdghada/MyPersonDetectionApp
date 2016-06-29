#include "detectmultiscalebatchdialog.h"
#include "ui_detectmultiscalebatchdialog.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "my_utilties.h"
using namespace cv;
DetectMultiScaleBatchDialog::DetectMultiScaleBatchDialog(QWidget *parent, QStringList images, MyPersonDetector *detector) :
    QDialog(parent),
    ui(new Ui::DetectMultiScaleBatchDialog)
{
    ui->setupUi(this);
    this->images=images;
    id=0;
    NMS=ui->checkBox->isChecked();
//    imageWidget=new ImageWidget();
    imageWidget=ui->widget;
    this->detector=detector;
//    ui->widget=imageWidget;
    if (images.size()>0){
        original=imread(images[id].toStdString());
        highlighted=detect(original.clone());
        showHighlighted();
    }
}

DetectMultiScaleBatchDialog::~DetectMultiScaleBatchDialog()
{
    delete ui;
}

void DetectMultiScaleBatchDialog::NextImage()
{
    id++;
    if (images.size()>0){
        id=(id+images.size())%images.size();
        original=imread(images[id].toStdString());
        highlighted=detect(original.clone());
        showHighlighted();
    }
}

void DetectMultiScaleBatchDialog::PreviousImage()
{
    id--;
    if (images.size()>0){
        id=(id+images.size())%images.size();
        original=imread(images[id].toStdString());
        highlighted=detect(original.clone());
        showHighlighted();
    }

}

void DetectMultiScaleBatchDialog::Close()
{
    this->close();
}


Mat DetectMultiScaleBatchDialog::detect(Mat img){

    vector<DetectionWindow> boxes=detector->detectMultiScale(img);
    vector<Rect> rects;
    vector<double> weights;
    for (DetectionWindow box:boxes){
        rects.push_back(box.getROI());
        weights.push_back(box.getPrediction());
    }

    Mat dispImg=img;
    for (Rect rect:rects){
        cv::Mat roi = dispImg(rect);
        cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(0, 255,0));
        double alpha = 0.01;
        cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
           // rectangle(dispImg,rect,cv::Scalar(0,255,0.1));
    }
    m_dbg<<"before group"<<rects.size();
    //groupRectangles(rects,0,0.8);
    rects=nonMaximumSupression(rects,0.5,weights);
    m_dbg<<"after group"<<rects.size();
    noNMS=dispImg.clone();
    addNMSRects(rects,dispImg);

    return dispImg;


}
void DetectMultiScaleBatchDialog::showOriginal(){
    m_dbg<<"show original";
    imageWidget->showImage(original);
    imageWidget->repaint();
}
void DetectMultiScaleBatchDialog::showHighlighted(){
    m_dbg<<"show highlighted";
    if (NMS)
        imageWidget->showImage(highlighted);
    else
        imageWidget->showImage(noNMS);
    imageWidget->repaint();
}
void DetectMultiScaleBatchDialog::showNMS(bool b){
    NMS=b;
    showHighlighted();
}
