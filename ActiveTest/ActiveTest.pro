QT += core axcontainer qml quick

CONFIG += c++11 precompile_header

PRECOMPILED_HEADER = pch.h

TARGET = ActiveTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CSMSCallback.cpp \
    NetworkManager.cpp \
    # SMSManager.cpp \
    SMSObjectManager.cpp \
    Boss.cpp

HEADERS += \
    pch.h \
    CSMSCallback.h \
    NetworkManager.h \
    # SMSManager.h \
    SMSObjectManager.h \
    Boss.h

RESOURCES += \
    qml.qrc
