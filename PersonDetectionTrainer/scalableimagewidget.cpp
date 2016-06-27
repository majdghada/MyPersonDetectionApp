#include "scalableimagewidget.h"
#include "ui_scalableimagewidget.h"
#include <opencv2/imgproc.hpp>
ScalableImageWidget::ScalableImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::scalableImageWidget)
{
    ui->setupUi(this);
}
ScalableImageWidget::~ScalableImageWidget()
{
    delete ui;
}
void ScalableImageWidget::showImage(Mat img)
{
    this->img=img;
    updateZoom();
}
void ScalableImageWidget::updateZoom()
{
    double scale=
        ui->horizontalSlider->value()/100.0;
    Mat tmp;
    cv::resize(img,tmp,Size(img.cols*scale,img.rows*scale));
    ui->widget->showImage(tmp);

}
void ScalableImageWidget::internalWidgetMouseLeft(){
    mouseLeft();
}
void ScalableImageWidget::internalWidgetMouseEntered(){
    mouseEntered();
}
