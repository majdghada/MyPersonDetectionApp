#ifndef DETECTORMAINWINDOW_H
#define DETECTORMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class DetectorMainWindow;
}

class DetectorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DetectorMainWindow(QWidget *parent = 0);
    ~DetectorMainWindow();

private:
    Ui::DetectorMainWindow *ui;
};

#endif // DETECTORMAINWINDOW_H
