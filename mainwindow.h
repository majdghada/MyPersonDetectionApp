#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMap>
#include <QMainWindow>
#include "svmparameters.h"
#include "mypersondetector.h"
#include "my_utilties.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringList posData;
    QStringList negData;
    SVMParameters svmParameters;
    MyPersonDetector detector;
    void updateLabels();
    MyComboBox<FeatureExtractionStrategy * > FeatureType;
public slots:
    void addPos();
    void addNeg();

    void clearPos();
    void clearNeg();

    void setSVMParams();

    void train();
    void test();
    void crossValidate();

    void saveSVM();
    void loadSVM();

    void detectSingleWindow();
    void detectSlidingWindow();

    void setFeaturesType(int index);
};
#endif // MAINWINDOW_H
