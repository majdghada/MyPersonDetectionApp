#include "detectormainwindow.h"
#include "ui_detectormainwindow.h"
#include <QFileDialog>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <detectmultiscalebatchdialog.h>
#include <QLayout>
DetectorMainWindow::DetectorMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DetectorMainWindow)
{
    ui->setupUi(this);
    detector.loadSVM("/home/majd/Desktop/SVM-History/regression/hog2.xml");
    detector.setFeatureExtractionStrategy(new HOG());
}

DetectorMainWindow::~DetectorMainWindow()
{
    delete ui;
}
void DetectorMainWindow::showImages(QStringList selected)
{
    ui->lineEdit->setText(selected.join(';'));
    QMdiArea * mdiarea = new QMdiArea() ;
    //ui->imagesViewWidget=new QWidget(this);
    ui->imagesViewWidget->setLayout(new QVBoxLayout(ui->imagesViewWidget));
    ui->imagesViewWidget->layout()->removeItem(ui->imagesViewWidget->layout()->itemAt(0));
    ui->imagesViewWidget->layout()->addWidget(mdiarea);

    DetectMultiScaleBatchDialog *d = new DetectMultiScaleBatchDialog (this,selected,&detector);
    QMdiSubWindow *w=mdiarea->addSubWindow(d);
    mdiarea->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    w->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mdiarea->setViewMode(QMdiArea::TabbedView);
    w->show();
    ui->imagesViewWidget->show();
}

void DetectorMainWindow::imagesBrowse(){
    QStringList selected;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setDirectory(QDir("/home/majd/Downloads/inr/INRIAPerson/"));
    if (dialog.exec()){

        selected=dialog.selectedFiles();
//        ui->imagesViewWidget=new DetectMultiScaleBatchDialog (this,selected,&detector);
//        ui->imagesViewWidget->repaint();

        showImages(selected);

    }
}
void DetectorMainWindow::writtenPaths(){
    QStringList selected=ui->lineEdit->text().split(';');
    showImages(selected);

}
