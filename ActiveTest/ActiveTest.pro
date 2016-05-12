QT += core axcontainer qml quick network

CONFIG += c++11 precompile_header

PRECOMPILED_HEADER = pch.h

TARGET = ActiveTest
CONFIG += console
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
  DEFINES += LOG_DEBUG
  message(debug build)
} else:CONFIG(release, debug|release) {
  DEFINES += LOG_RELEASE
  message(release build)
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
    SystemTray.cpp

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
    logger.h

RESOURCES += \
    qml.qrc
