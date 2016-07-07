#ifndef DETECTORMAINWINDOW_H
#define DETECTORMAINWINDOW_H

#include <QMainWindow>
#include "mypersondetector.h"
#include <opencv2/videoio.hpp>
#include "my_utilties.h"
namespace Ui {
class DetectorMainWindow;
}

class DetectorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DetectorMainWindow(QWidget *parent = 0);
    ~DetectorMainWindow();

public slots:
    void imagesBrowse();
    void writtenImagesPaths();
    void detectorBrowse();
    void writtenDetectorPath();
    void videoBrowse();
    void writtenVideoPath();
    void startVideo();

private:
    Ui::DetectorMainWindow *ui;
    MyPersonDetector detector;
    void showImages(QStringList selected);

    void videoDetectBF(VideoCapture *source);
    void videoDetect(VideoCapture *source);
    MyComboBox<int> trackingMethodCombo;
};

#endif // DETECTORMAINWINDOW_H
