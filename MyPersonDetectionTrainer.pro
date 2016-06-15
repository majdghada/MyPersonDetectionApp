#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T15:26:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyPersonDetectionTrainer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    svmparametersdialog.cpp \
    svmparameters.cpp \
    mysvm.cpp \
    my_utilties.cpp \
    mypersondetector.cpp \
    detectionwindow.cpp \
    dataloader.cpp \
    detectortestdialog.cpp \
    featureextractionstrategy.cpp

HEADERS  += mainwindow.h \
    svmparametersdialog.h \
    svmparameters.h \
    mysvm.h \
    my_utilties.h \
    mypersondetector.h \
    detectionwindow.h \
    dataloader.h \
    detectortestdialog.h \
    featureextractionstrategy.h

FORMS    += mainwindow.ui \
    svmparametersdialog.ui \
    detectortestdialog.ui

CONFIG += c++11

LIBS += -L/usr/local/lib \
        -L/usr/lib/x86_64-linux-gnu \
        -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_objdetect \
        -lopencv_nonfree \
        -lopencv_xfeatures2d \
        -lopencv_ml
