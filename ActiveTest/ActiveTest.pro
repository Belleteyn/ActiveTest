QT += core gui axcontainer qml quick network

CONFIG += c++11 precompile_header

PRECOMPILED_HEADER = pch.h

TARGET = SMSClient

include(dependencies.pri)

CONFIG(debug, debug|release) {

  message(debug build)

  CONFIG += console
  DEFINES += LOG_DEBUG
} else:CONFIG(release, debug|release) {

  message(release build)

  DEFINES += LOG_RELEASE
}

TEMPLATE = app

RC_ICONS = icon.ico

SOURCES += main.cpp \
    #CSMSCallback.cpp \
    NetworkManager.cpp \
    # SMSManager.cpp \
    SMSObjectManager.cpp \
    Boss.cpp \
    MessageHolder.cpp \
    servertest.cpp \
    SystemTray.cpp \
    LogHelper.cpp

HEADERS += \
    pch.h \
    #CSMSCallback.h \
    NetworkManager.h \
    # SMSManager.h \
    SMSObjectManager.h \
    Boss.h \
    SMSCallback.h \
    MessageHolder.h \
    servertest.h \
    SystemTray.h \
    Message.h \
    LogHelper.h

RESOURCES += \
    qml.qrc
