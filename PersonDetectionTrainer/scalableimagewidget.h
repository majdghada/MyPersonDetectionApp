#ifndef SCALABLEIMAGEWIDGET_H
#define SCALABLEIMAGEWIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>
#include <thread>
using namespace cv;
namespace Ui {
class scalableImageWidget;
}

class ScalableImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScalableImageWidget(QWidget *parent = 0);
    ~ScalableImageWidget();
    void showImage(Mat img);
private:
    Ui::scalableImageWidget *ui;
    Mat img;
public slots:
    void updateZoom();
    void internalWidgetMouseLeft();
    void internalWidgetMouseEntered();
signals:
    void mouseEntered();
    void mouseLeft();
};

#endif // SCALABLEIMAGEWIDGET_H
