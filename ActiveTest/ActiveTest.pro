QT += core gui axcontainer qml quick network

CONFIG += c++11 precompile_header copy_settings
CONFIG -= debug_and_release debug_and_release_target

PRECOMPILED_HEADER = pch.h

TARGET = SMSClient

include(dependencies.pri)

DESTDIR = bin
OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

CONFIG(debug, debug|release) {

  CONFIG += console
  DEFINES += LOG_DEBUG

} else:CONFIG(release, debug|release) {

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
    SystemTray.h \
    Message.h \
    LogHelper.h \
    HandledSink.h

RESOURCES += \
    qml.qrc

copy_settings {
  SETTINGS_FILENAME = settings.ini

  OTHER_FILES +=\
    $$SETTINGS_FILENAME

  copySettingsStep.commands = $(COPY_FILE) $$system_path($$PWD/$$SETTINGS_FILENAME) $$system_path($$OUT_PWD/$$DESTDIR)

  first.depends = $(first) copySettingsStep
  export(first.depends)
  export(copySettingsStep.commands)
  QMAKE_EXTRA_TARGETS += first copySettingsStep
}
