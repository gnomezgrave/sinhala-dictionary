#-------------------------------------------------
#
# Project created by QtCreator 2014-08-25T21:49:00
#
#-------------------------------------------------

QT       += core gui \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dictionary
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dictionary.cpp \
    dbconnector.cpp \
    restconnector.cpp \
    ini_reader.cpp \
    settings.cpp \
    about.cpp \
    process.cpp \
    connector.cpp \
    settingsparams.cpp \
    addword.cpp

HEADERS  += mainwindow.h \
    dictionary.h \
    connector.h \
    dbconnector.h \
    headers.h \
    restconnector.h \
    ini_reader.h \
    settings.h \
    about.h \
    process.h \
    settingsparams.h \
    addword.h

FORMS    += mainwindow.ui \
    settings.ui \
    about.ui \
    addword.ui

OTHER_FILES += \
    search.png \
    help.png \
    close.png \
    down.png \
    gear.png \
    up.png \
    logo.png
