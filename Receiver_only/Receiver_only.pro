QT += core
QT += core serialport
QT -= gui

TARGET = Receiver_only
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    receiver.cpp \
    serialport.cpp

HEADERS += \
    receiver.h \
    serialport.h

