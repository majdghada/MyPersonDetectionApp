#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T15:26:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets

TARGET = PersonDetectionTrainer
TEMPLATE = app


SOURCES += main.cpp\
    svmparametersdialog.cpp \
    svmparameters.cpp \
    mysvm.cpp \
    my_utilties.cpp \
    mypersondetector.cpp \
    detectionwindow.cpp \
    dataloader.cpp \
    detectortestdialog.cpp \
    featureextractionstrategy.cpp \
    detectmultiscalebatchdialog.cpp \
    imagewidget.cpp \
    scalableimagewidget.cpp \
    trainermainwindow.cpp

HEADERS  += \
    svmparametersdialog.h \
    svmparameters.h \
    mysvm.h \
    my_utilties.h \
    mypersondetector.h \
    detectionwindow.h \
    dataloader.h \
    detectortestdialog.h \
    featureextractionstrategy.h \
    detectmultiscalebatchdialog.h \
    imagewidget.h \
    scalableimagewidget.h \
    trainermainwindow.h

FORMS    += \
    svmparametersdialog.ui \
    detectortestdialog.ui \
    detectmultiscalebatchdialog.ui \
    scalableimagewidget.ui \
    trainermainwindow.ui

CONFIG += c++11

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_objdetect \
        -lopencv_xfeatures2d \
        -lopencv_ml

