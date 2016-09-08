QT += core
QT += core serialport
QT -= gui

TARGET = Generator_only
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    generator.cpp \
    serialport.cpp \
    device.cpp \
    controlcenter.cpp

HEADERS += \
    generator.h \
    serialport.h \
    device.h \
    controlcenter.h

