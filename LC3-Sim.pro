#-------------------------------------------------
#
# Project created by QtCreator 2017-08-24T18:28:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LC3-Sim
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += \
    resources.qrc

FORMS += \
    mainwindow.ui \
    frame.ui \
    loading.ui

HEADERS += \
    status.h \
    Assembler.h \
    mainwindow.h \
    HistoryHandler.h \
    Utility.h \
    Bridge.h \
    ThreadManager.h \
    StackModeler.h \
    modeler.h \
    TrainFlagged.h \
    BetterScrollbar.h \
    RegisterModel.h \
    DoUndo.h \
    Thought.h \
    hope.h \
    Registers.h \
    Memory.h \
    Console.h \
    Util.h \
    GlobalTypes.h \
    Breakpoints.h

SOURCES += \
    Bridge.cpp \
    mainwindow.cpp \
    ThreadManager.cpp \
    modeler.cpp \
    StackModeler.cpp \
    TrainFlagged.cpp \
    main.cpp \
    RegisterModel.cpp \
    BetterScrollbar.cpp \
    hope.cpp \
    Thought.cpp \
    HistoryHandler.cpp \
    status.cpp \
    Console.cpp \
    Registers.cpp \
    Memory.cpp \
    Simulator.cpp \
    util.cpp

