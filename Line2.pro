#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T14:17:24
#
#-------------------------------------------------

QT      += core gui
QT      += network
QT      += widgets


TARGET = LineQt
TEMPLATE = app


SOURCES +=\
    dmxreceiver.cpp \
    main.cpp \
    mainwindow.cpp \


HEADERS  += mainwindow.h \
    dmxreceiver.h \
    include/json.hpp \
    oscpkt.hh \


LIBS += -lcurl
