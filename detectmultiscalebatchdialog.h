#ifndef DETECTMULTISCALEBATCHDIALOG_H
#define DETECTMULTISCALEBATCHDIALOG_H

#include <QDialog>
#include "imagewidget.h"
#include "mypersondetector.h"
#include "scalableimagewidget.h"
namespace Ui {
class DetectMultiScaleBatchDialog;
}

class DetectMultiScaleBatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DetectMultiScaleBatchDialog(QWidget *parent = 0,QStringList images=QStringList(),MyPersonDetector *detector=NULL);
    ~DetectMultiScaleBatchDialog();

private:
    Ui::DetectMultiScaleBatchDialog *ui;
    QStringList images;
    ScalableImageWidget *imageWidget;
    MyPersonDetector *detector;
    int id;
    Mat highlighted,original;
    Mat detect(Mat img);
public slots:
    void NextImage();
    void PreviousImage();
    void Close();
    void showOriginal();
    void showHighlighted();
};

#endif // DETECTMULTISCALEBATCHDIALOG_H
