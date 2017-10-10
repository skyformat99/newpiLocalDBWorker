QT += core sql network serialbus
QT -= gui

CONFIG += c++11

TARGET = piLocalDBWorkerExec
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    anLogger/src/anlogger.cpp \
    BinaryProtocol/src/binaryprotocol.cpp \
    WindowProtocol/src/windowprotocol.cpp \
    CanProtocol/src/canprotocol.cpp \
    shared/commonthings.cpp \
    shared/abstractstatemachinebasis.cpp \
    shared/directtransition.cpp \
    src/pilocaldbworker.cpp \
    src/pilocaldbworkerbasis.cpp \
    src/uninitiatedpilocaldbworker.cpp \
    src/idlepilocaldbworker.cpp \
    src/runningpilocaldbworker.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    anLogger/src/anlogger.h \
    BinaryProtocol/src/binaryprotocol.h \
    WindowProtocol/src/windowprotocol.h \
    CanProtocol/src/canprotocol.h \
    shared/commonthings.h \
    shared/abstractstatemachinebasis.h \
    shared/directtransition.h \
    src/pilocaldbworker.h \
    src/pilocaldbworkerbasis.h \
    src/uninitiatedpilocaldbworker.h \
    src/idlepilocaldbworker.h \
    src/runningpilocaldbworker.h
