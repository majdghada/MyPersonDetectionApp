#include "detectormainwindow.h"
#include "ui_detectormainwindow.h"

DetectorMainWindow::DetectorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetectorMainWindow)
{
    ui->setupUi(this);
}

DetectorMainWindow::~DetectorMainWindow()
{
    delete ui;
}
