#include "navigatormainwindow.h"
#include "ui_navigatormainwindow.h"
#include "trainermainwindow.h"
#include "detectormainwindow.h"

NavigatorMainWindow::NavigatorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigatorMainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose);
}

NavigatorMainWindow::~NavigatorMainWindow()
{
    delete ui;
}
void NavigatorMainWindow::StartTrainer()
{
    TrainerMainWindow * w=new TrainerMainWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}
void NavigatorMainWindow::StartDetector()
{
    DetectorMainWindow * w=new DetectorMainWindow(this);
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();

}
