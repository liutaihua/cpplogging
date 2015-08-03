TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Logging.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Logging.h \
    singleton.h

