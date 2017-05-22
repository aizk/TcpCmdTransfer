QT += core network
QT -= gui

TARGET = TcpCmdTransfer
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += include

LIBS += lib/libcurl.lib

DEFINES -= UNICODE

Debug:LIBS += lib/log4cplusD.lib

Release:LIBS += lib/log4cplus.lib

TEMPLATE = app

SOURCES += \
    alarmdev.cpp \
    cmd.cpp \
    cmdbuffer.cpp \
    cmdfactory.cpp \
    cmdrecv.cpp \
    cmdrunnable.cpp \
    cmdtranslator.cpp \
    crc16.cpp \
    devcmdrecv.cpp \
    httphandler.cpp \
    icmdhandler.cpp \
    icmdrecv.cpp \
    icmdtranslator.cpp \
    main.cpp \
    nochangetranslator.cpp \
    phonecmdrecv.cpp \
    platformcmdrecv.cpp \
    platformtodevhandler.cpp \
    platformtranslator.cpp \
    recvthread.cpp \
    replytodevtranslator.cpp \
    tcpappcmdrecv.cpp \
    tcpcmdfactory.cpp \
    tcpcmdrecv.cpp \
    tcpdevcmdrecv.cpp \
    tcphandler.cpp \
    tcpserver.cpp \
    tcpsocket.cpp \
    tcpthread.cpp \
    timerthread.cpp \
    udpcmdrecv.cpp \
    utils.cpp

HEADERS += \
    alarmdev.h \
    cmd.h \
    cmdbuffer.h \
    cmdfactory.h \
    cmdrecv.h \
    cmdrunnable.h \
    cmdtranslator.h \
    const.h \
    crc16.h \
    devcmdrecv.h \
    httphandler.h \
    icmdhandler.h \
    icmdrecv.h \
    icmdtranslator.h \
    nochangetranslator.h \
    phonecmdrecv.h \
    platformcmdrecv.h \
    platformtodevhandler.h \
    platformtranslator.h \
    recvthread.h \
    replytodevtranslator.h \
    tcpappcmdrecv.h \
    tcpcmdfactory.h \
    tcpcmdrecv.h \
    tcpdevcmdrecv.h \
    tcphandler.h \
    tcpserver.h \
    tcpsocket.h \
    tcpthread.h \
    timerthread.h \
    udpcmdrecv.h \
    utils.h

