#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPainter>
#include <QPicture>
#include <QThread>
#include <QMouseEvent>
#include <QStandardItemModel>
#include "BetterScrollbar.h"
#include "stdio.h"
#include "modeler.h"
#include "RegisterModel.h"
#include "hope.h"
#include "Utility.h"
#include "Bridge.h"
#include "QSettings"
#include "ThreadManager.h"
#include "StackModeler.h"
#include "ScrollBarHandler.h"
#include "InOutSet.h"
#include "WatchWatcher.h"
#include "MemWindow.h"
#define BRSLOT 0
#define ADDRSLOT 1
#define NAMESLOT 2


#define DISPLAY_WIDTH  128
#define DISPLAY_HEIGHT 124
#define DISPLAY_SCALE  2

#define SCREEN_WIDTH   DISPLAY_WIDTH *DISPLAY_SCALE
#define SCREEN_HEIGHT  DISPLAY_HEIGHT*DISPLAY_SCALE

#define DEFAULT_WINDOW_WIDTH 1163
#define DEFAULT_WINDOW_HEIGHT 694




namespace Ui {
class MainWindow;

}
/**
 * The MainWindow class
 *
 * This is the class which handles the main window.  It keeps track of the
 * models used for registers and memory, as well as facilitates the
 * connection of the sub elements.
 *
 *
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;


//    MemWindow *makeNConnectNewMemWindow(modeler *model);
    int getScreenWidth();
    int getScreenHeight();

    void saveWorkSpace();
public:
    /**
     * @brief model The main model for the memory tables
     *
     * This is used to provide easy access to the memory locations, comments,
     * values, breakpoints, and labels.
     */
    modeler* model;

    /**
     * @brief disp The main display of the screen
     *
     * This is used to provide a visual representation of the memory from
     * 0xC000 to 0xFDFF, inclusive
     */
    Hope *disp;
    
    /**
     * @brief StackModel The Stack Model
     * 
     * An alternative view of the memory contained within the scope of the
     * stack area as defined by the simulator
     */

    StackModeler * StackModel;

    /** 
     * @brief regModel The Register Model
     * 
     * This allows easy viewing of the registers, memory protection
     * register, and proccess status register. 
     */
    RegisterModel* regModel;
    
    /**
     * @brief manager The interface which allows multithreaded execution
     * 
     * This is not actually a thread, rather a way of keeping track of
     * whether the thread is running and notify the rest of the system of the
     * changes of such state     * 
     */

    ThreadManager* manager;

    /**
     * @brief Undos The Undo Recording Data Type
     * 
     * This is a sort of stack which is more of a stack-
     * doubly_linkedlist combo
     * which enables the redoing and undoing of actions.
     */
    HistoryHandler* Undos;

    ScrollBarHandler* Saturn;

    InOutSet* InOutPut;

    WatchWatcher* Clockmaker;

    /**
     * @brief threadRunning Describes the runningness of the sim thread 
     * this is given to any part of the
     * system which can change or display data.  How those pieces act is 
     * entirely up to them.
     * 
     */
    bool* threadRunning = new bool(false);//the thread isn't running on init




    MainWindow(QWidget *parent = 0);
    ~MainWindow();



    void setupConnections();
public slots:
    /**
     * on_MemView2GotoButton_pressed
     *
     * As described by the name, this function is called when the named
     * button is pressed.  It is tasked with taking the text in MemView2Input
     * , turning it into a number, and sending MemView2
     *
     * This is a name templated method, one of many that make communication
     * with the ui easy and clear.
     */
    void on_MemView3GotoButton_pressed();

    void on_StackViewGotoButton_pressed();

    void setupDisplay();
    void setupViews();
    void setupWatches();

    void setupMemView(QTableView* view, bool setmodel = true, bool setScroll =true);


    void setupInOut();

    void setupControlButtons();


    void setupUndoInterface();
    void on_continueButton_clicked();

    void setupRegisterView();

    void setupStackView();

    void setupThreadManager();

    void setupMenuBar();
    void setupScreenMenuDropdown(QMenu &menu);
    void setupConsoleInterface();
    void handleFiles();

    bool loadFile(QString path = QString());
    QString assembleFile(QString path);
    void assembleNLoadFile(QString path = QString());

    void on_MemView3Input_returnPressed();

    void on_StackViewInput_returnPressed();




    void on_MemView3PCButton_pressed();


    void on_NextButton_pressed();

    static void  threadTest(QString);



    void on_pushButton_7_clicked();

    void my_int_func(int x)
    {
        printf( "%d\n", x );
    }
    void prepWork();
    void update();
    void readSettings();
    void saveSettings();
    void closeEvent(QCloseEvent *event);
signals:
    void signalUpdate();
    void reCheck();
    void requestUndo();
    void requestHalt();
public slots:
    void onTableClicked(const QModelIndex & current);
    void gotoRunningMode();
    void gotoUserMode();

    void reloadState();
    void storeState();
private slots:
    void on_pushButton_4_pressed();
    void on_Update_Temp_pressed();
    void on_IntoButton_pressed();
    void on_MemView1_destroyed();
    void on_undoButton_pressed();
    void on_redoButton_pressed();
    void on_consoleEnterButton_pressed();
    void on_continueButton_pressed();
    void on_RestoreButton_pressed();
    void on_SaveButton_pressed();
    void on_haltButton_pressed();
    void testingSave();
};

#endif // MAINWINDOW_H
