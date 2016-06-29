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
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/"));
    if (dialog.exec()){
        ui->videoLineEdit->setText(dialog.selectedFiles()[0]);
        ui->videoFileRadioButton->setChecked(true);
    }
}

void DetectorMainWindow::writtenVideoPath()
{

}
void DetectorMainWindow::videoDetect(VideoCapture & source){
    if (!source.isOpened()){
        m_dbg<<"invalid source";
        return ;
    }
    string windowName="video detection";
    namedWindow(windowName+"1",1);
    namedWindow(windowName+"2",1);
    Mat frame;
    HOGDescriptor hog;
    hog.setSVMDetector(hog.getDefaultPeopleDetector());
    HOGDescriptor myHog;
    vector<float> hog_detector_vec;
    get_svm_detector(detector.svm.getSvm(),hog_detector_vec);
    myHog.setSVMDetector(hog_detector_vec);
    int cnt=0;
    while (source.read(frame)){

//        cv::resize(frame,frame,cv::Size(300,300));
        Mat frame1=getDispImg(&myHog,frame);
        Mat frame2=getDispImg(&hog,frame);
        imshow(windowName+"1",frame1);
        imshow(windowName+"2",frame2);

        if(waitKey(30) >= 0) break;
    }
    destroyWindow(windowName+"1");
    destroyWindow(windowName+"2");
    source.release();
}

void DetectorMainWindow::startVideo()
{
    if (ui->webCamRadioButton->isChecked()){
        VideoCapture cam(0);
        videoDetect(cam);
    }
    else {
        VideoCapture vid(ui->videoLineEdit->text().toStdString());
        videoDetect(vid);
    }
}

void DetectorMainWindow::writtenDetectorPath()
{
    loadDetectorFromFile(detector,ui->detectorLineEdit->text().toStdString(),this);
}
