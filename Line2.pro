#-------------------------------------------------
#
# Project created by QtCreator 2016-10-02T14:17:24
#
#-------------------------------------------------

QT       += core gui
QT         += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = LineQt
TEMPLATE = app


SOURCES +=\
    main.cpp \
    mainwindow.cpp \
    sacn/ACNShare/CID.cpp \
    sacn/ACNShare/VHD.cpp \
    sacn/ACNShare/ipaddr.cpp \
    sacn/ACNShare/tock.cpp \
    sacn/sacnlistener.cpp \
    sacn/sacnsender.cpp \
    sacn/sacnsocket.cpp \
    sacn/sacnuniverselistmodel.cpp \
    sacn/streamcommon.cpp \
    sacn/streamingacn.cpp

HEADERS  += mainwindow.h \
    oscpkt.hh \
    sacn/ACNShare/CID.h \
    sacn/ACNShare/VHD.h \
    sacn/ACNShare/defpack.h \
    sacn/ACNShare/deftypes.h \
    sacn/ACNShare/ipaddr.h \
    sacn/ACNShare/tock.h \
    sacn/consts.h \
    sacn/sacnlistener.h \
    sacn/sacnsender.h \
    sacn/sacnsocket.h \
    sacn/sacnuniverselistmodel.h \
    sacn/streamcommon.h \
    sacn/streamingacn.h
