#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T17:55:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prog4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ip_algorithms.cpp \
    chris_algorithms.cpp \
    ian_algorithms.cpp

HEADERS  += mainwindow.h \
    ip_algorithms.h \
    chris_algorithms.h \
    ian_algorithms.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

RESOURCES += \
    images.qrc
