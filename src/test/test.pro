include(../../smpp.pri)
include(../lib/lib.pri)

TEMPLATE = app

TARGET = $$APP_TARGET

DESTDIR = $$APP_PATH

CONFIG += console

QT      -= gui
QT      -= core network

TEMPLATE = app

win32-g++ {
  QMAKE_CXXFLAGS += -std=c++11
}

INCLUDEPATH += $$PWD \
               G:/src/c++/lib/boost_1_56_0

DEPENDPATH += G:/src/c++/lib/boost_1_56_0

LIBS += -L"G:/src/c++/lib/boost_1_56_0/stage/lib/" \
        -llibboost_system-mgw48-d-1_56 \
        -lws2_32

SOURCES  += \
  test_client.cpp \
  main.cpp

HEADERS += \
  test_client.h \
  global.h

unix: {
  INSTALLS += /usr/bin
}
