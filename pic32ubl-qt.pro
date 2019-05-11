#-------------------------------------------------
#
# Project created by QtCreator 2013-12-30T09:00:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pic32ubl-qt
TEMPLATE = app


SOURCES +=\
    stdafx.cpp \
    pic32ubldlg.cpp \
    bootloader.cpp \
    ethernet.cpp \
    comport.cpp \
    hex.cpp \
    usbhid.cpp \
    pic32ubl.cpp \
    hid.c

HEADERS  += \
    stdafx.h \
    pic32ubldlg.h \
    bootloader.h \
    ethernet.h \
    comport.h \
    hex.h \
    usbhid.h \
    hidapi.h

FORMS    += \
    pic32ubldlg.ui

unix:!macx: LIBS += -lserial

unix:!macx: LIBS += -lusb

unix:!macx: LIBS += -lusb-1.0

