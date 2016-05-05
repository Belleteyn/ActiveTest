TEMPLATE = app

CONFIG += console

QT += qml quick axcontainer

PRECOMPILED_HEADER = pch.h

CONFIG += c++11 precompile_header

#TYPELIBS = $$system(dumpcpp -nometaobject -o SMSObjectManager -compat -getfile {4892F679-C799-439A-828D-3C7592D4D201})

#isEmpty(TYPELIBS) {
#  message("no lib founf")
#}
#else {
#  message("all is ok")

#  SOURCES += main.cpp \
#    stuff.cpp

#  RESOURCES += qml.qrc

#  HEADERS += \
#    stuff.h \
#    pch.h \
#    smscallback.h \
#    SMSObjectManager.h
#}

  SOURCES += main.cpp \
    \# stuff.cpp # \
    SMSObjectManager.cpp \
    NetworkManager.cpp
    #SMSManager.cpp

  RESOURCES += qml.qrc

  HEADERS += \
    pch.h \
    smscallback.h \# \
    SMSObjectManager.h \
    NetworkManager.h
    #SMSManager.h

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
