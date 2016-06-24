#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "svmparametersdialog.h"
#include "detectortestdialog.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include "detectmultiscalebatchdialog.h"
#include <QtDebug>
using namespace cv;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FeatureType.setComboBox(ui->FeaturesTypeCombo);
    FeatureType.add_item("SIFT Scale Invariant Feature Transform",new SIFT());
    FeatureType.add_item("HOG Histogram of Oriented Gradients",new HOG());

    updateLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::updateLabels(){
    ui->PosLabel->setText(QString("%1 pos selected").arg(posData.size()));
    ui->NegLabel->setText(QString("%1 neg selected").arg(negData.size()));
}

void MainWindow::addPos()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/pos/"));
    if (dialog.exec()){
        posData+=dialog.selectedFiles();
    }
    updateLabels();
}

void MainWindow::addNeg()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/train_64x128_H96/neg/"));
    if (dialog.exec()){
        negData+=dialog.selectedFiles();
    }
    updateLabels();
}

void MainWindow::clearPos()
{
    posData.clear();
    updateLabels();
}

void MainWindow::clearNeg()
{
    negData.clear();
    updateLabels();
}

void MainWindow::setSVMParams()
{
    SVMParametersDialog dialog(this,svmParameters);
    if (dialog.exec()){
        svmParameters=dialog.getParameters();
    }
}

void MainWindow::train()
{
    detector.setParameters(svmParameters);
    detector.setdata(posData,negData);
    detector.train();
    QMessageBox dia(QMessageBox::Information, "info", "training finished ", QMessageBox::Ok, this);
    dia.exec();
}

void MainWindow::test()
{
    detectorTestDialog dia(this,&detector,posData,negData);
    dia.exec();
}

void MainWindow::crossValidate()
{
    QStringList wholepos,wholeneg,trainpos,testpos,trainneg,testneg;
       wholepos=posData;wholeneg=negData;
       posData.clear();negData.clear();
       for (int i=0;i<wholepos.size();++i){
           if (i<0.6*wholepos.size())trainpos.append(wholepos.at(i));
           else testpos.append(wholepos.at(i));
       }
       for (int i=0;i<wholeneg.size();++i){
           if (i<0.66*wholeneg.size())trainneg.append(wholeneg.at(i));
           else testneg.append(wholeneg.at(i));
       }
       posData=trainpos;negData=trainneg;
       train();
       posData=testpos;negData=testneg;
       test();
       posData=wholepos;negData=wholeneg;
}

void MainWindow::saveSVM()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDirectory("/home/majd/Desktop/SVM-History/");
    QStringList filenames;
    if (dialog.exec()){
        filenames=dialog.selectedFiles();
        detector.saveSVM(filenames[0].toStdString());
        QFile typeFile(QString("%1.type").arg(filenames[0]));
        typeFile.open(QIODevice::ReadWrite);
        QTextStream typeStream(&typeFile);
        typeStream<<FeatureType.getSelectedIndex();
        typeFile.close();
        QMessageBox dia(QMessageBox::Information, "info", "SVM saved", QMessageBox::Ok, this);
        dia.exec();
    }
}

void MainWindow::loadSVM()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory("/home/majd/Desktop/SVM-History/");
    QStringList filenames;
    if (dialog.exec()){
        filenames=dialog.selectedFiles();
        detector.loadSVM(filenames[0].toStdString());
        svmParameters=detector.getParameters();
        QFile typeFile(QString("%1.type").arg(filenames[0]));
        typeFile.open(QIODevice::ReadOnly);
        QTextStream typeStream(&typeFile);
        FeatureType.setSelectedIndex((typeStream.readAll().toInt()));
        typeFile.close();
        QMessageBox dia(QMessageBox::Information, "info", "SVM loaded", QMessageBox::Ok, this);
        dia.exec();

    }
}

void MainWindow::detectSingleWindow()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QStringList filenames;
    if (dialog.exec()){
        filenames=dialog.selectedFiles();
        float pr=detector.predict(imread(filenames[0].toStdString()));
        imshow(filenames[0].toStdString(),imread(filenames[0].toStdString()));
        QMessageBox dia(QMessageBox::Information, "info", QString("%1").arg(isPositiveClass(pr)?"positive":"negative"), QMessageBox::Ok, this);
        dia.exec();
    }

}

void MainWindow::detectSlidingWindow()
{
    QFileDialog dialog(this);
        dialog.setDirectory("/home/majd/Downloads/inr/INRIAPerson/");
        dialog.setFileMode(QFileDialog::ExistingFile);
        QStringList filenames;
        if (dialog.exec()){
            filenames=dialog.selectedFiles();
            Mat img=imread(filenames[0].toStdString());
            double div=1;//max(img.rows/300.0,img.cols/300.0);
            cv::resize(img,img,cv::Size(img.cols/div,img.rows/div));


            vector<DetectionWindow> boxes=detector.detectMultiScale(img);
            vector<Rect> rects;
            for (DetectionWindow box:boxes){
                rects.push_back(box.getROI());
            }
            m_dbg<<"before grouping "<<rects.size();
            groupRectangles(rects,0);
            m_dbg<<"after grouping "<<rects.size();
    //        imshow(filenames[0].toStdString(),imread(filenames[0].toStdString()));
            QMessageBox dia(QMessageBox::Information, "info", QString("%1").arg(boxes.size()), QMessageBox::Ok, this);
            dia.exec();
            Mat dispImg=imread(filenames[0].toStdString());

            cv::resize(dispImg,dispImg,cv::Size(dispImg.cols/div,dispImg.rows/div));
            for (Rect rect:rects){
                cv::Mat roi = dispImg(rect);
                cv::Mat color(roi.size(), CV_8UC3, cv::Scalar(0, 255,0));
                double alpha = 0.01;
                cv::addWeighted(color, alpha, roi, 1.0 - alpha , 0.0, roi);
    //            rectangle(dispImg,box,cv::Scalar(0,255,0.1),);
            }
            imshow("detectMultiScale",dispImg);
        }
}
void MainWindow::setFeaturesType(int index){
    m_dbg<<"selected "<<index;
    detector.setFeatureExtractionStrategy(FeatureType.getValue(index));
}
void MainWindow::detectSlidingWindowBatch(){
    QStringList selected;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/"));
    if (dialog.exec()){
        selected=dialog.selectedFiles();
        DetectMultiScaleBatchDialog dia(this,selected,&detector);
        dia.exec();
    }


}
