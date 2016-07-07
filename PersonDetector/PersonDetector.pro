#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T14:59:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PersonDetector
TEMPLATE = app


SOURCES += \
    main.cpp \
    $$PWD/../PersonDetectionTrainer/trainermainwindow.cpp \
    $$PWD/../PersonDetectionTrainer/svmparametersdialog.cpp \
    $$PWD/../PersonDetectionTrainer/svmparameters.cpp \
    $$PWD/../PersonDetectionTrainer/mysvm.cpp \
    $$PWD/../PersonDetectionTrainer/my_utilties.cpp \
    $$PWD/../PersonDetectionTrainer/mypersondetector.cpp \
    $$PWD/../PersonDetectionTrainer/detectionwindow.cpp \
    $$PWD/../PersonDetectionTrainer/dataloader.cpp \
    $$PWD/../PersonDetectionTrainer/detectortestdialog.cpp \
    $$PWD/../PersonDetectionTrainer/featureextractionstrategy.cpp \
    $$PWD/../PersonDetectionTrainer/detectmultiscalebatchdialog.cpp \
    $$PWD/../PersonDetectionTrainer/imagewidget.cpp \
    $$PWD/../PersonDetectionTrainer/scalableimagewidget.cpp \
    detectormainwindow.cpp \
    kalmansifthogvideo.cpp \
    kalmansifttrackableframe.cpp


HEADERS  += $$PWD/../PersonDetectionTrainer/trainermainwindow.h \
    $$PWD/../PersonDetectionTrainer/svmparametersdialog.h \
    $$PWD/../PersonDetectionTrainer/svmparameters.h \
    $$PWD/../PersonDetectionTrainer/mysvm.h \
    $$PWD/../PersonDetectionTrainer/my_utilties.h \
    $$PWD/../PersonDetectionTrainer/mypersondetector.h \
    $$PWD/../PersonDetectionTrainer/detectionwindow.h \
    $$PWD/../PersonDetectionTrainer/dataloader.h \
    $$PWD/../PersonDetectionTrainer/detectortestdialog.h \
    $$PWD/../PersonDetectionTrainer/featureextractionstrategy.h \
    $$PWD/../PersonDetectionTrainer/detectmultiscalebatchdialog.h \
    $$PWD/../PersonDetectionTrainer/imagewidget.h \
    $$PWD/../PersonDetectionTrainer/scalableimagewidget.h \
    detectormainwindow.h \
    kalmansifthogvideo.h \
    kalmansifttrackableframe.h


FORMS    +=$$PWD/../PersonDetectionTrainer/trainermainwindow.ui \
    $$PWD/../PersonDetectionTrainer/svmparametersdialog.ui \
    $$PWD/../PersonDetectionTrainer/detectortestdialog.ui \
    $$PWD/../PersonDetectionTrainer/detectmultiscalebatchdialog.ui \
    $$PWD/../PersonDetectionTrainer/scalableimagewidget.ui \
    detectormainwindow.ui

CONFIG += c++11

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_flann \
        -lopencv_calib3d \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_video \
        -lopencv_features2d \
        -lopencv_objdetect \
        -lopencv_xfeatures2d \
        -lopencv_ml


INCLUDEPATH += $$PWD/../PersonDetectionTrainer
DEPENDPATH += $$PWD/../PersonDetectionTrainer
