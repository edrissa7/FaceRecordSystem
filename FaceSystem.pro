#-------------------------------------------------
#
# Project created by QtCreator 2019-04-07T16:08:18
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += C:\Users\user\Downloads\opencv\RELEASE\install\include
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_core401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_calib3d401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_highgui401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_imgproc401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_imgcodecs401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_ml401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\opencv_ffmpeg401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_videoio401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_objdetect401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_face401.dll
LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\libopencv_reg401.dll

#LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\
#LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\
#LIBS += C:\Users\user\Downloads\opencv\RELEASE\bin\
include(C:\Users\user\Desktop\dlib-19.17\dlib_project_library_path.pri)

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        menu.cpp \
    login.cpp \
    mainpage.cpp \
    users.cpp \
    facerecognizer.cpp \
    datasettrainer.cpp \
    imagesave.cpp \
    controller.cpp

HEADERS += \
        menu.h \
    login.h \
    mainpage.h \
    users.h \
    facerecognizer.h \
    datasettrainer.h \
    imagesave.h \
    controller.h

FORMS += \
        menu.ui \
    login.ui \
    mainpage.ui \
    users.ui \
    imagesave.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
