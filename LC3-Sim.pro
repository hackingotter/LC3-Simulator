#-------------------------------------------------
#
# Project created by QtCreator 2017-08-24T18:28:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =  LC3-Sim

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#QMAKE_CXXFLAGS += -Wunused-parameter
# -mpreferred-stack-boundary=2 -finline-small-functions -momit-leaf-frame-pointer
#DEFINES += QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += console

RESOURCES += \
    resources.qrc

FORMS += \
    mainwindow.ui \
    frame.ui \
    loading.ui \
    dialog.ui \
    savedialog.ui \
    form.ui

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
    Console.h \
    Util.h \
    GlobalTypes.h \
    computer.h \
    opcodes.h \
    FileHandler.h \
    ScrollBarHandler.h \
    ModelDelegate.h \
    KBRDModel.h \
    InOutSet.h \
    WatchWatcher.h \
    WatchModel.h \
    MemTable.h \
    MemWindow.h \
    FollowButton.h \
    shortcutslisting.h \
    UndoStackView.h \
    HelpMenu.h \
    StateMachine.h \
    UndoStackMasker.h \
    Preferences.h \
    Saver.h


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
    Simulator.cpp \
    util.cpp \
    computer.cpp \
    utility.cpp \
    assembler.cpp \
    FileHandler.cpp \
    ScrollBarHandler.cpp \
    ModelDelegate.cpp \
    KBRDModel.cpp \
    InOutSet.cpp \
    WatchWatcher.cpp \
    WatchModel.cpp \
    MemTable.cpp \
    MemWindow.cpp \
    FollowButton.cpp \
    globaltypes.cpp \
    UndoStackView.cpp \
    HelpMenu.cpp \
    StateMachine.cpp \
    UndoStackMasker.cpp \
    Preferences.cpp \
    Saver.cpp


DISTFILES += \
    Test.txt \
    Test_copy.txt \
    lc3os.asm \
    Commitment \
    pennCompilertest.asm \
    Priority.txt

STATECHARTS += \
    State Chart.scxml \
    United.scxml

