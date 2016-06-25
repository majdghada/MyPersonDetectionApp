#ifndef NAVIGATORMAINWINDOW_H
#define NAVIGATORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class NavigatorMainWindow;
}

class NavigatorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NavigatorMainWindow(QWidget *parent = 0);
    ~NavigatorMainWindow();

public slots:
    void StartTrainer();
    void StartDetector();
private:
    Ui::NavigatorMainWindow *ui;
};

#endif // NAVIGATORMAINWINDOW_H
