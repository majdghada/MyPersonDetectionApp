#include "detectormainwindow.h"
#include "ui_detectormainwindow.h"
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <detectmultiscalebatchdialog.h>
#include <QLayout>
#include "my_utilties.h"
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include "kalmansifthogvideo.h"
#include "kalmanhogvideo.h"
using namespace cv;
DetectorMainWindow::DetectorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetectorMainWindow)
{
    ui->setupUi(this);
    detector.loadSVM("/home/majd/Desktop/SVM-History/regression/hog2.xml");
    ui->detectorLineEdit->setText("/home/majd/Desktop/SVM-History/regression/hog2.xml");
    detector.setFeatureExtractionStrategy(new HOG());
    trackingMethodCombo.setComboBox(ui->trackingMethodComboBox);
    trackingMethodCombo.add_item("Brute Force",0);
    trackingMethodCombo.add_item("SIFT Matching",1);

    trackingMethodCombo.add_item("Hog Local Detection",2);


}

DetectorMainWindow::~DetectorMainWindow()
{
    delete ui;
}
void DetectorMainWindow::showImages(QStringList selected)
{
    ui->lineEdit->setText(selected.join(';'));
    QMdiArea * mdiarea = new QMdiArea() ;
    //ui->imagesViewWidget=new QWidget(this);
    ui->imagesViewWidget->setLayout(new QVBoxLayout(ui->imagesViewWidget));
    ui->imagesViewWidget->layout()->removeItem(ui->imagesViewWidget->layout()->itemAt(0));
    ui->imagesViewWidget->layout()->addWidget(mdiarea);

    DetectMultiScaleBatchDialog *d = new DetectMultiScaleBatchDialog (this,selected,&detector);
    QMdiSubWindow *w=mdiarea->addSubWindow(d);
    mdiarea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    w->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mdiarea->setViewMode(QMdiArea::TabbedView);
    w->show();
    ui->imagesViewWidget->show();
}

void DetectorMainWindow::imagesBrowse(){
    QStringList selected;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/"));
    if (dialog.exec()){

        selected=dialog.selectedFiles();
//        ui->imagesViewWidget=new DetectMultiScaleBatchDialog (this,selected,&detector);
//        ui->imagesViewWidget->repaint();

        showImages(selected);

    }
}
void DetectorMainWindow::writtenImagesPaths(){
    QStringList selected=ui->lineEdit->text().split(';');
    showImages(selected);

}

void DetectorMainWindow::detectorBrowse()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory("/home/majd/Desktop/SVM-History/");
    QStringList filenames;
    if (dialog.exec()){
        filenames=dialog.selectedFiles();
        loadDetectorFromFile(detector,filenames[0].toStdString(),this);
        ui->detectorLineEdit->setText(filenames[0]);

    }
}

void DetectorMainWindow::videoBrowse()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec()){
        ui->videoLineEdit->setText(dialog.selectedFiles()[0]);
        ui->videoFileRadioButton->setChecked(true);
    }
}

void DetectorMainWindow::writtenVideoPath()
{

}
void DetectorMainWindow::videoDetectSiftTracker(VideoCapture * source){
    if (!source->isOpened()){
        m_dbg<<"invalid source";
        return ;
    }
    HOGDescriptor *hog=new HOGDescriptor();
    hog->setSVMDetector(hog->getDefaultPeopleDetector());
    HOGDescriptor *myHog=new HOGDescriptor();
    vector<float> hog_detector_vec;
    get_svm_detector(detector.svm.getSvm(),hog_detector_vec);
    myHog->setSVMDetector(hog_detector_vec);
    m_dbg<<"before new kalmansifthogvideo";
    kshvid=new KalmanSiftHogVideo(hog,source);
    m_dbg<<"after new kalman sift hog video";
//    while (waitKey(30)!='q'){

//    }
//    vid->setTerminated();
    return ;
}
void DetectorMainWindow::videoDetectHogTracker(VideoCapture * source){
    if (!source->isOpened()){
        m_dbg<<"invalid source";
        return ;
    }
    HOGDescriptor *hog=new HOGDescriptor();
    hog->setSVMDetector(hog->getDefaultPeopleDetector());
    HOGDescriptor *myHog=new HOGDescriptor();
    vector<float> hog_detector_vec;
    get_svm_detector(detector.svm.getSvm(),hog_detector_vec);
    myHog->setSVMDetector(hog_detector_vec);
    m_dbg<<"before new kalmanhogvideo";
    khvid=new KalmanHogVideo(hog,source);
    m_dbg<<"after new kalmanhogvideo";
//    while (waitKey(1)!='q'){
//        std::this_thread::sleep_for(chrono::milliseconds(30));
//    }
//    waitKey();
//    vid->setTerminated();
    return ;
}
void DetectorMainWindow::videoDetectBF(VideoCapture * source){
    if (!source->isOpened()){
        m_dbg<<"invalid source";
        return ;
    }
    HOGDescriptor *hog=new HOGDescriptor();
    hog->setSVMDetector(hog->getDefaultPeopleDetector());
//    HOGDescriptor *myHog=new HOGDescriptor();
//    vector<float> hog_detector_vec;
//    get_svm_detector(detector.svm.getSvm(),hog_detector_vec);
//    myHog->setSVMDetector(hog_detector_vec);
    string windowName="video detection";
//    namedWindow(windowName+"1",1);
    namedWindow(windowName+"2",1);
    Mat frame;
    while (source->read(frame)){

//        Mat frame1=getDispImg(myHog,frame);
        Mat frame2=getDispImg(hog,frame);
//        imshow(windowName+"1",frame1);
        imshow(windowName+"2",frame2);

        if(waitKey(30) >= 0) break;
    }
//    destroyWindow(windowName+"1");
    destroyWindow(windowName+"2");
    source->release();
}

void DetectorMainWindow::startVideo()
{
    if (ui->webCamRadioButton->isChecked()){
        VideoCapture *cam=new VideoCapture(0);
        if (trackingMethodCombo.getSelectedValue()==0)
            videoDetectBF(cam);
        else if (trackingMethodCombo.getSelectedValue()==1)
            videoDetectSiftTracker(cam);
        else if (trackingMethodCombo.getSelectedValue()==2)
            videoDetectHogTracker(cam);
    }
    else {
        VideoCapture *vid=new VideoCapture(ui->videoLineEdit->text().toStdString());
        if (trackingMethodCombo.getSelectedValue()==0)
            videoDetectBF(vid);
        else if (trackingMethodCombo.getSelectedValue()==1)
            videoDetectSiftTracker(vid);
        else if (trackingMethodCombo.getSelectedValue()==2)
            videoDetectHogTracker(vid);


    }
}

void DetectorMainWindow::stopVideo()
{
    if (trackingMethodCombo.getSelectedValue()==1)
        kshvid->setTerminated();
    else if (trackingMethodCombo.getSelectedValue()==2)
        khvid->setTerminated();
}


void DetectorMainWindow::writtenDetectorPath()
{
    loadDetectorFromFile(detector,ui->detectorLineEdit->text().toStdString(),this);
}
