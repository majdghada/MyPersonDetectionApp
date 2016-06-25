#ifndef TRAINERMAINWINDOW_H
#define TRAINERMAINWINDOW_H
#include <QMainWindow>
#include "svmparameters.h"
#include "mypersondetector.h"
#include "my_utilties.h"

namespace Ui {
class TrainerMainWindow;
}
class TrainerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TrainerMainWindow(QWidget *parent = 0);
    ~TrainerMainWindow();

private:
    Ui::TrainerMainWindow *ui;
    QStringList posData;
    QStringList negData;
    SVMParameters svmParameters;
//    MyPersonDetector detector;
    AugmentedPersonDetector detector;
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
    void detectSlidingWindowBatch();
};
#endif // TRAINERMAINWINDOW_H
