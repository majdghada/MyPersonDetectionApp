#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#pragma once
#include <QWidget>

#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>


class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = 0);


public slots:

    void showImage(const cv::Mat& image) ;

protected:
    void paintEvent(QPaintEvent* /*event*/);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    QImage _qimage;
    cv::Mat _tmp;
signals:
    void mouseEntered();
    void mouseLeft();
};

#endif // IMAGEWIDGET_H
