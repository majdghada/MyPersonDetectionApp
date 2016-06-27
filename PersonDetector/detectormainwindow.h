#ifndef DETECTORMAINWINDOW_H
#define DETECTORMAINWINDOW_H

#include <QMainWindow>
#include "mypersondetector.h"
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
    void writtenPaths();

private:
    Ui::DetectorMainWindow *ui;
    MyPersonDetector detector;
    void showImages(QStringList selected);
};

#endif // DETECTORMAINWINDOW_H
