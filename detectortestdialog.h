#ifndef DETECTORTESTDIALOG_H
#define DETECTORTESTDIALOG_H
#include <mypersondetector.h>
#include <QDialog>
#include <thread>
namespace Ui {
class detectorTestDialog;
}

class detectorTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit detectorTestDialog(QWidget *parent = 0);
    ~detectorTestDialog();

    detectorTestDialog(QWidget *parent, MyPersonDetector detector, QStringList posData, QStringList negData);
    void updateProgress(int correct, int all);
    bool getTerminated();
public slots:
    void cancel();
private:
    int totalCorrect;
    int total;
    bool terminated;
    int numOfThreads;
    std::thread ** threads;
    Ui::detectorTestDialog *ui;
    Mutex mtx;
signals:
    void progressChanged(int);
};

#endif // DETECTORTESTDIALOG_H
