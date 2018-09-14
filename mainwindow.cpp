#include "QScreen"
#include "UndoStackMasker.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QScrollArea>
#include "BetterScrollbar.h"
#include "RegisterModel.h"
//#include "Simulator.h"
#include "ModelDelegate.h"
#include "Util.h"
#include "MemWindow.h"
#include "stdio.h"
#include "hope.h"
#include <QColorDialog>
#include <QtConcurrent/QtConcurrentRun>
#include <QStatusBar>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QThread>
#include "RegisterModel.h"
#include "status.h"
#include "modeler.h"
#include "Bridge.h"
#include <QFuture>
#include "StackModeler.h"
#include "DoUndo.h"
#include "FileHandler.h"
#include "UndoStackView.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include "HelpMenu.h"
#include "MemTable.h"
#include <QDataStream>
#include "Assembler.h"
#include <QUndoView>
#include <QItemSelectionModel>
#include <map>
#include <QFile>
#include "KBRDModel.h"
#include <QProcess>
#include "Console.h"
#include "Saver.h"
#include <QCoreApplication>
#define REGISTERVIEWNUMCOLUMN 2
#define SCROLLTO(VIEW,INPUT)\
{\
    (VIEW)->scrollTo((VIEW)->model()->index(INPUT,0),QAbstractItemView::PositionAtTop);\
    }

#define MEMVIEWSETUPPERCENT 20

#define HEX_COLUMN_WIDTH 60

#define MEM_VIEW_BP_COL      0
#define MEM_VIEW_ADR_COL     1
#define MEM_VIEW_NAME_COL    2
#define MEM_VIEW_VAL_COL     3
#define MEM_VIEW_MNEM_COL    4
#define MEM_VIEW_COMMENT_COL 5

#define STACK_VIEW_ADR_COL      0
#define STACK_VIEW_OFFSET_COL   1
#define STACK_VIEW_NAME_COL     2
#define STACK_VIEW_VAL_COL      3
#define STACK_VIEW_MNEM_COL     4
#define STACK_VIEW_COMMENT_COL  5

#define UPDATEVIEW(TABLEVIEW) TABLEVIEW->hide();TABLEVIEW->show();



#define SETUPDISPLAY(UI,THIS)\
    qDebug("Setting up the display");

#define FINISHING_TOUCHES(DISP,MODEL)\
    disp->update();



#define UPDATE_REGISTER_DISPLAY(UI,REGISTER)\
    UI->RegisterView->item((int)REGISTER,REGISTERVIEWNUMCOLUMN)->setText(QString().setNum(getRegister(REGISTER)));
#define UPDATE_COND_DISPLAY(UI)\
    switch(getProgramStatus())\
{\
    case cond_n:UI->RegisterView->item((int)PSR,REGISTERVIEWNUMCOLUMN)->setText("N");break;\
    case cond_z:UI->RegisterView->item((int)PSR,REGISTERVIEWNUMCOLUMN)->setText("Z");break;\
    case cond_p:UI->RegisterView->item((int)PSR,REGISTERVIEWNUMCOLUMN)->setText("P");break;\
    case cond_none:UI->RegisterView->item((int)PSR,REGISTERVIEWNUMCOLUMN)->setText("ERR");break;\
    default:UI->RegisterView->item((int)PSR,REGISTERVIEWNUMCOLUMN)->setText("P");\
    }

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{



//    Computer::getDefault()->lowerBoundTimes();
    std::cout<<Computer::getDefault()->proposedNewAddress(8,5,10,-2)<<std::endl;

    Saver::savePortable();
    std::cout<<Computer::getDefault()->proposedNewAddress(9,5,10,-2)<<std::endl;
    Computer::getDefault()->setProgramStatus(cond_z);

    Utility::systemInfoDebug();//Just some fun info
    Utility::Utilit::setup();
    setupThreadManager();//QED


    QFuture<void> f1 = QtConcurrent::run(threadTest,QString("1"));
    f1.waitForFinished();


    qDebug("About to setup ui");
    ui->setupUi(this);//this puts everything in place
    setupConnections();
    setupDisplay();
    setupMenuBar();
    setupRegisterView();
    setupViews();
    setupControlButtons();
    setupInOut();
    setupWatches();
    setupUndoInterface();
    setupConsoleInterface();



    Bridge::doWork();
    qDebug("Connecting Disp");

    //    QObject::connect(Computer::getDefault() ,SIGNAL(update()),disp,SLOT(update()));
    QObject::connect(Computer::getDefault() ,SIGNAL(update()),this,SLOT(update()));
    QObject::connect(disp,SIGNAL(mouseMoved(QString)),ui->Mouseposition,SLOT(setText(QString)));
    qDebug("Connecting ");

    QObject::connect(ui->actionClear,SIGNAL(triggered()),disp,SLOT(clearScreen()));

//    ui->undoStackSpot->addWidget();f
    //    QObject::connect(ui->NextButton,SIGNAL(on_NextButton_pressed()),ui->RegisterView,SLOT(update()));
    readSettings();
    update();
//    testSave();

}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::importantInfo()
{
    HelpMenu* help = new HelpMenu();
    help->show();
    help->activateWindow();
}
//MemWindow* MainWindow::makeNConnectNewMemWindow(modeler* model)
//{
//    MemWindow* Newest = new MemWindow(model,Saturn->generateBar());
//    CONNECT(this,signalUpdate(),Newest,kick());
//}
void MainWindow::setupViews()
{
    qDebug("Setting Up Views");
    qDebug("Now will be making the model");
    this->model = new modeler(this, threadRunning);

    this->StackModel = new StackModeler(this,threadRunning);
    qInfo("Header made");
    qInfo("Size Set");
    QString str;
    Saturn = new ScrollBarHandler();
    for(int i = 0;i<3;i++)
    {
        MemWindow* memy = new MemWindow(model,Saturn->generateBar());
        CONNECT(this,signalUpdate(),memy,kick());
        ui->MemorySplitter->addWidget(memy);
    };
    setupMemView(ui->MemView3View);

    setupStackView();
    qDebug("Model Created");
    /*
     * There is an assumption that hitting enter will cause an input to be entered.
     * These lines connect the three inputs for the views
     */
    qDebug("Connecting View interfaces");
    {
        //   connect(ui->MemView1Input,SIGNAL(returnPressed()),ui->MemView1GotoButton,SLOT(click()));
        connect(ui->MemView3Input,SIGNAL(returnPressed()),ui->MemView3GotoButton,SLOT(click()));
        //        connect(ui->StackViewInput,SIGNAL(returnPressed()),ui->StackViewGotoButton,SLOT(click()));


    }
    qDebug("Done Connecting Views");
    qDebug("Done Setting Up Views");
}

void MainWindow::setupWatches()
{
    qDebug("Setting up Watch");
    Clockmaker = new WatchWatcher(0,threadRunning);
    setupMemView(Clockmaker->getTableViewPtr(),false,false);
    ui->verticalLayout_8->addWidget(Clockmaker);
}
void MainWindow::setupMenuBar()
{
    QAction* actionLoad_File = new QAction("Load File",this);
    QAction* actionAssemble_File = new QAction("Assemble File",this);
    QAction* actionAssemble_Load_File = new QAction("Assemble and Load File",this);
    QAction* actionSave_File = new QAction("Save File",this);
    QAction* actionSave_File_As= new QAction("Save File As ...",this);
    QAction* actionSave_State = new QAction("Save IDE State",this);
    QAction* actionLoad_State = new QAction("Load IDE State",this);
    QAction* actionTestingSave = new QAction("Test saving",this);

    QList<QAction*> fileActions;
    fileActions <<actionLoad_File;
//    fileActions <<actionAssemble_File;
    fileActions <<actionAssemble_Load_File;
//    fileActions <<actionSave_File;
    fileActions <<actionSave_File_As;
//    fileActions <<actionSave_State;
//    fileActions <<actionLoad_State;
//    fileActions <<actionTestingSave;
    actionAssemble_Load_File->setShortcut(QKeySequence(tr("Ctrl+D")));
    actionSave_File_As->setShortcut(QKeySequence(tr("Ctrl+S")));

//    <<actionAssemble_File<<actionAssemble_Load_File<<actionSave_File<<actionSave_File_As<<actionSave_State<<actionLoad_State;
//    fileActions << actionTestingSave;
    ui->menuFile->addActions(fileActions);

    CONNECT(actionSave_State,triggered(),this, storeState());
    CONNECT(actionLoad_State,triggered(),this, reloadState());
    CONNECT(actionLoad_File,triggered(),this,loadFile());
    CONNECT(actionAssemble_Load_File,triggered(),this, assembleNLoadFile());
    connect(actionTestingSave, &QAction::triggered, this,[=](){this->prettySave();});

    QMenu* fillMenu = new QMenu("Fill...");
    setupScreenMenuDropdown(*fillMenu);
    ui->menuScreen->addMenu(fillMenu);
}
void MainWindow::prettySave()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,"Save to *.asm",QString(),"*.asm");


}

void MainWindow::testSave()
{
    QFrame* pictu = new QFrame();

    assembleNLoadFile("C:/Users/Jedadiah/Downloads/LC3Fill.asm");
    handleConsoleIn(QString("save x3000 x3104").toLatin1().data());
    exit(0);
}
void MainWindow::setupScreenMenuDropdown(QMenu & menu)
{
    QAction* actionFill_White = new QAction("White",this);
    QAction* actionFill_Black = new QAction("Black",this);
    QAction* actionFill_Red = new QAction("Red",this);
    QAction* actionFill_Green = new QAction("Green",this);
    QAction* actionFill_Blue = new QAction("Blue",this);
    QAction* actionFill_Puce = new QAction("Puce",this);

    QList<QAction*> fillActions;

    fillActions<<actionFill_White<<actionFill_Black<<actionFill_Red<<actionFill_Green<<actionFill_Blue<<actionFill_Puce;


    connect(actionFill_White,   &QAction::triggered, disp, [=](){ disp->fillScreen(0x7FFF);});
    connect(actionFill_Black,   &QAction::triggered, disp, [=](){ disp->fillScreen(0x0000);});
    connect(actionFill_Red,     &QAction::triggered, disp, [=](){ disp->fillScreen(0x7C00);});
    connect(actionFill_Green,   &QAction::triggered, disp, [=](){ disp->fillScreen(0x03C0);});
    connect(actionFill_Blue,    &QAction::triggered, disp, [=](){ disp->fillScreen(0x001F);});
    connect(actionFill_Puce,    &QAction::triggered, disp, [=](){ disp->fillScreen(0x3466);});
    menu.addActions(fillActions);

}

void MainWindow::setupConsoleInterface()
{
    ui->plainTextEdit->insertPlainText("Hello");
//    QFuture<void> f1 = QtConcurrent::run(startConsole(),QString("1"));
//    f1.begin();
}
void MainWindow::testingSave()
{
    Saver::savePortable();
}
void MainWindow::reloadState()
{
    Saver::loadState();
}
void MainWindow::storeState()
{
    Saver::saveState();
}
void MainWindow::setupControlButtons()
{

    Computer* comp = Computer::getDefault();
    connect(ui->haltButton,  &QPushButton::pressed,comp, [=](){ comp->setRunning(false); });
//    CONNECT(ui->haltButton,pressed(),Computer::getDefault(),);

}
void MainWindow::setupConnections()
{
    qRegisterMetaType<mem_addr_t>("mem_addr_t");
    qRegisterMetaType<val_t>("val_t");
    //    qRegisterMetaType<ProcessHandle*>("ProcessHandle*const");
}
void MainWindow::setupUndoInterface()
{
    UndoStackMasker* widge = new UndoStackMasker(Computer::getDefault()->Undos);
    ui->splitter_2->addWidget(widge);

    widge->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
}
bool MainWindow::loadFile(QString path)
{
    bool success = false;
    Computer::getDefault()->Undos->beginMacro("Load "+ path);
    qDebug("Attempting to load a program");
    if(path==QString())
    {
        qDebug("Looks like there was no file specified.  Time for the user to choose.");
        path = QFileDialog::getOpenFileName(this,"Select a file to load",QString(),"*.obj");
    }
    if(path!=QString())
    {
        qDebug("Attempting to use that choice " +path.toLocal8Bit());
        try
        {
            Computer::getDefault()->loadProgramFile(path.toLocal8Bit().data());
        }
        catch(const std::string& e)
        {
            std::cout<<e<<std::endl;
            success = false;
        }
        catch(...)
        {
            std::cout<<"An unexpected error has occurred"<<std::endl;
            success = false;
        }

    }
    else
    {
        qDebug("Seems that the user chose not to choose");
    }
    Computer::getDefault()->Undos->endMacro();
    if(!success) Computer::getDefault()->Undos->undo();
    return success;
}
QString MainWindow::assembleFile(QString path)
{
    QFileDialog* fileUI = new QFileDialog();
    fileUI->setNameFilter(QString("Assembly (*").append(ASSEMBLY_SUFFIX).append(")"));
    fileUI->setReadOnly(true);
    fileUI->setWindowTitle("Choose a file to assemble and load into memory");


    Assembler embler = Assembler();
    
    if(path==QString())
    {
        qDebug("Looks like there was no file specified.  Time for the user to choose.");
        path = fileUI->getOpenFileName();

    }
    QString target = path;
    try
    {
        QString target = path;
        target.replace(-3,3,OBJECT_SUFFIX);
        embler.assembleFile(path.toLocal8Bit().data(),target.toLocal8Bit().data());
    }
    catch(const std::string& e)
    {
        std::cout<<e<<std::endl;
        return "";
    }
    catch(...)
    {
        std::cout<<"An unforseen error has occured"<<std::endl;
        return "";
    }

    return "Test.obj";


}
void MainWindow::assembleNLoadFile(QString path)
{

    QFileDialog* fileUI = new QFileDialog();
    fileUI->setNameFilter(QString("Assembly (*").append(ASSEMBLY_SUFFIX).append(")"));
    fileUI->setReadOnly(true);
    fileUI->setWindowTitle("Choose a file to assemble and load into memory");


    Assembler embler = Assembler();

    if(path==QString())
    {
        qDebug("Looks like there was no file specified.  Time for the user to choose.");
        path = fileUI->getOpenFileName();
    }

    qDebug(path.toLocal8Bit());
    QString shortPath = path;
    shortPath.remove(0,path.lastIndexOf("/"));
    QString namePath = "test.obj";
    Computer::getDefault()->Undos->beginMacro("Assemble and Load "+shortPath);
    qDebug("assembling and loading");


    qDebug("Trying " + path.toLocal8Bit());

    try
    {
        embler.assembleFile(path.toLocal8Bit().data(),namePath.toLocal8Bit().data());
    }
    catch(const std::string& e)
    {
        std::cout<<e<<std::endl;
        Computer::getDefault()->Undos->endMacro();
        Computer::getDefault()->Undos->undo();//no need in saving this
        return;
    }
    catch(...)
    {

        std::cout<<"An unforseen error has occured"<<std::endl;
        Computer::getDefault()->Undos->endMacro();
        Computer::getDefault()->Undos->undo();//no need in saving this
        return;
    }
    try{
        embler.passLabelsToComputer(Computer::getDefault());
        embler.passCommentsToComputer(Computer::getDefault());
        embler.passDataTypesToComputer(Computer::getDefault());
    }
    catch(const std::string& e)
    {
        std::cout<<e<<endl;
        Computer::getDefault()->Undos->endMacro();
        Computer::getDefault()->Undos->undo();
        return;
    }
    if(loadFile(namePath))
    {
        qDebug("successfully loaded");

    }
    else
    {
        qDebug("didn't work");

    }

    Computer::getDefault()->Undos->endMacro();
    //    embler.assembleFile();

}
void MainWindow::handleFiles()
{
    Assembler Bill;
    QString inputPath = QFileDialog::getOpenFileName().toLocal8Bit().data();
    Bill.assembleFile(inputPath.toLatin1().data(),"LC3Maybe.obj");
    Computer::getDefault()->loadProgramFile(QString("LC3Maybe.obj").toLatin1().data());
    IFNOMASK(emit update();)
}
void MainWindow::setupDisplay()
{

    disp = new Hope();
    QHBoxLayout* qhbl= new QHBoxLayout();
    qhbl->addWidget(disp,0,Qt::AlignCenter);
    ui->verticalLayout_11->addLayout(qhbl);
    disp->autoFillBackground();
    disp->setMinimumSize(SCREEN_WIDTH,SCREEN_HEIGHT);
    disp->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    QObject::connect(Computer::getDefault(),SIGNAL(memValueChanged(mem_addr_t)),disp,SLOT(update(mem_addr_t)));

}
void MainWindow::setupMemView(QTableView* view, bool setmodel, bool setScroll)
{

    qDebug("Attaching the model to the views");
    qDebug("Showing Grid");
    view->showGrid();

    if(setmodel)
    {
        qDebug("Setting Model");
        view->setModel(model);
    }
    if(setScroll){
        HighlightScrollBar* scroll = new HighlightScrollBar(Qt::Vertical,this);
        Saturn->addScrollBar(scroll);
        view->setVerticalScrollBar(scroll);
    }


    // this is inefficient and useless since we should set those in the design
    //qDebug("Resizing Columns");
    //qDebug("model has "+QString().setNum(model->columnCount()).toLocal8Bit());
    //qDebug(QString().setNum(view->height()).toLocal8Bit());
    //view->resizeColumnsToContents();
    qDebug("Hiding vertical Header");
    view->verticalHeader()->hide();
    qDebug("setting Column width");

    view->setColumnWidth(MEM_VIEW_BP_COL,30);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_BP_COL,QHeaderView::Fixed);

    view->setColumnWidth(MEM_VIEW_ADR_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_ADR_COL,QHeaderView::Fixed);

    view->setColumnWidth(MEM_VIEW_NAME_COL,HEX_COLUMN_WIDTH);

    view->setColumnWidth(MEM_VIEW_VAL_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_VAL_COL,QHeaderView::Fixed);

    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);


    //    view->setContextMenuPolicy(Qt::CustomContextMenu);

    //    connect(view, SIGNAL(customContextMenuRequested(const QPoint &)),
    //            this, SLOT(showClickOptions(const QPoint &,view)));

        QObject::connect(Computer::getDefault(),SIGNAL(update()),view,SLOT(update()));
}

void MainWindow::setupInOut()
{
    InOutPut = new InOutSet(this);
    ui->inOutHome->addWidget(InOutPut);
    QMenu* InOutMenuBar = new QMenu("Text Options");

    QAction* actionClearInput= new QAction("Clear Input Space",this);

     CONNECT(actionClearInput,triggered(),InOutPut, clearText());
     InOutMenuBar->addAction(actionClearInput);
    ui->menuBar->addMenu(InOutMenuBar);
    CONNECT(this, reCheck(), InOutPut, update());
}
void MainWindow::onTableClicked(const QModelIndex & current)
{

}

void MainWindow::setupStackView()
{
    qDebug("Setting up Stack View");
    qDebug("Showing Grid");
    MemWindow* StackWindow = new MemWindow(StackModel,Saturn->generateBar());

    CONNECT(this,signalUpdate(),StackWindow,kick());
    ui->StackBox->layout()->addWidget(StackWindow);
    MemTable* view = StackWindow->getMemView();
    StackWindow->setFlipped(true);
//    view->setButtonText("SP");
    //    qDebug("Setting Model");
    //    view->hide();
    //    view->setModel(StackModel);
    //    StackModel->flip();
    view->resizeColumnsToContents();
    view->verticalHeader()->hide();

    view->setColumnWidth(STACK_VIEW_ADR_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_ADR_COL,QHeaderView::Fixed);

    view->setColumnWidth(STACK_VIEW_VAL_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_VAL_COL,QHeaderView::Fixed);
    view->showGrid();
    //    // set row height and fix it
    view->verticalHeader()->setDefaultSectionSize(20);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
//    CONNECT(MainWindow::ui->actionFlip,triggered(),StackModel,flip());
//    CONNECT(StackModel,flip(),this,update());
    QObject::connect(Computer::getDefault(),SIGNAL(memValueChanged(mem_addr_t)),StackModel,SLOT(stackFrameListener(mem_addr_t)));
//    QObject::connect(Computer::getDefault(),SIGNAL(subRoutineCalled()),StackModel,SLOT(increaseStackFrameCounter()));
//QObject::connect(Computer::getDefault(),SIGNAL(memValueChanged(mem_addr_t)),StackModel,SLOT(stackFrameListener(mem_addr_t)));
    CONNECT(this,signalUpdate(),StackWindow,kick());
}

void MainWindow::setupRegisterView()
{
    QTableView* view = ui->RegisterView;

    qDebug("Initializing model");
    regModel = new RegisterModel(this,threadRunning);
    qDebug("Attaching the model to the views");
    view->setModel(regModel);
    qDebug("Showing Grid");
    view->showGrid();
    view->setColumnWidth(0,8);
    view->setColumnWidth(1,43);
    view->resizeColumnToContents(reg_value_column);
    qDebug("Setting horizantal heading options");
    {
        QHeaderView* hori = view->horizontalHeader();
        hori->hide();
        hori->setSectionResizeMode(reg_color_column,QHeaderView::Fixed);
        hori->setSectionResizeMode(reg_name_column,QHeaderView::Fixed);
        hori->setDefaultAlignment(Qt::AlignRight);
    }
    qDebug("Setting vertical heading options");
    {
        QHeaderView* vert = view->verticalHeader();
        vert->hide();
        vert->setDefaultSectionSize(DEFAULT_TEXT_HEIGHT);
        vert->setSectionResizeMode(QHeaderView::Fixed);
    }
    CONNECT(ui->RegisterView,requestChange(),this,update());







}
void MainWindow::setupThreadManager()
{
    manager = new ThreadManager();
    CONNECT(manager,started(),this, gotoRunningMode());
    CONNECT(manager,stopped(),this, gotoUserMode());
}

void MainWindow::on_continueButton_clicked()
{
    disp->repaint();
}

void MainWindow::on_MemView3Input_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}
void MainWindow::on_StackViewInput_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}

void MainWindow::on_MemView3PCButton_pressed()
{
    SCROLLTO(ui->MemView3View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView3GotoButton_pressed()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(ui->MemView3Input->text(),&ok);
    if(ok)
    {
        SCROLLTO(ui->MemView3View,target)
    }
    CLEAR(ui->MemView3Input)
}
void MainWindow::on_StackViewGotoButton_pressed()
{
    //    bool ok = true;
    //    int target =Utility::unifiedInput2Val(ui->StackViewInput->text(),&ok);
    //    if(ok)
    //    {
    //        SCROLLTO(ui->StackViewView,target);
    //    }
    //    CLEAR(ui->StackViewInput)
}
void MainWindow::on_NextButton_pressed()
{
    qDebug("Executing Single instruction");
    //    executeSingleInstruction();
    manager->activate(ThreadManager::Next);
    //    update();

}
void MainWindow::update()
{
    emit signalUpdate();
    disp->update();
    UPDATEVIEW(ui->MemView3View);
    //    UPDATEVIEW();
    UPDATEVIEW(ui->RegisterView);
    UPDATEVIEW(Clockmaker->Coat);
    Saturn->update();
    emit reCheck();
}

void MainWindow::threadTest(QString name)
{
    qDebug()<< name << QThread::currentThread();
    for(int i = 0;i<1000;i++)
    {

    }
    qDebug()<< name << QThread::currentThread();
}

void MainWindow::on_pushButton_7_clicked()
{

}

void MainWindow::gotoRunningMode()
{
    qDebug("Going to Running Mode");
    *threadRunning = true;
    ui->NextButton->setEnabled(false);
    IFNOMASK(emit update();)
            MASK
}
void MainWindow::gotoUserMode()
{
    qDebug("Going to User Mode");
    *threadRunning = false;
    ui->NextButton->setEnabled(true);
    UNMASK
            IFNOMASK(emit update();)
}
void MainWindow::prepWork()
{


}

void MainWindow::on_pushButton_4_pressed()
{


    qDebug("Next");
    manager->activate(ThreadManager::Next);

    update();

}


void MainWindow::on_Update_Temp_pressed()
{

}

void MainWindow::on_IntoButton_pressed()
{
    qDebug("Step");
    manager->activate(ThreadManager::Step);
}

void MainWindow::readSettings()
{
    QSettings settings("Melberg & Ott","PennSim++");
    settings.beginGroup("MainWindow");
    qDebug("heylo");

    int width = settings.value("Window Width",QVariant(DEFAULT_WINDOW_WIDTH)).toInt();
    int height= settings.value("Window Height",QVariant(DEFAULT_WINDOW_HEIGHT)).toInt();
    int defaultX = (getScreenWidth()-DEFAULT_WINDOW_WIDTH)/2;
    int defaultY = (getScreenHeight()-DEFAULT_WINDOW_HEIGHT)/4;

    int x     = settings.value("Window X",QVariant(defaultX)).toInt();
    int y     = settings.value("Window Y",QVariant(defaultY)).toInt();

    this->setGeometry(x,y,geometry().width(),geometry().height());
    int MemoryBoxHeight = settings.value("Memory Box Height",QVariant(635)).toInt();
    int MemoryBoxWidth  = settings.value("Memory Box Width" ,QVariant(354)).toInt();
    ui->MemorySplitter->restoreState(settings.value("Memory Splitter State").toByteArray());
    this->resize(width,height);
    ui->MemoryBox->resize(MemoryBoxWidth,MemoryBoxHeight);
    setWindowState(static_cast<Qt::WindowState>(settings.value("Window State",QVariant(Qt::WindowMaximized)).toInt()));
    settings.endGroup();
    qDebug("theylo");


}


int MainWindow::getScreenWidth()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    return screen->geometry().width();
}
int MainWindow::getScreenHeight()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    return screen->geometry().height();
}
void MainWindow::saveSettings()
{
    QSettings settings(ORGANIZATION,APPNAME);
    settings.beginGroup("MainWindow");
    settings.setValue("Window Height",this->height());
    settings.setValue("Window Width",this->width());
    settings.setValue("Memory Box Height", ui->MemoryBox->height());
    settings.setValue("Memory Box Width",ui->MemoryBox->width());
    settings.setValue("Memory Splitter State",ui->MemorySplitter->saveState());
    settings.setValue("Window State",static_cast<int>(windowState()));
    settings.setValue("Window X",geometry().x());
    settings.setValue("Window Y",geometry().y());
    settings.endGroup();

    qDebug("done saving ui Settings");



}
void MainWindow::closeEvent(QCloseEvent *event)
{
//    saveWorkSpace();
//    Computer::getDefault()->testUndoSpeed();
    saveSettings();
    Saver::vanguard();
    event->accept();
}

void MainWindow::saveWorkSpace()
{
//    Computer::getDefault()->saveWorkSpace();
}
void MainWindow::on_MemView1_destroyed()
{
    qDebug("Hey");
}

void MainWindow::on_undoButton_pressed()
{
    Computer::getDefault()->Undos->undo();
}

void MainWindow::on_redoButton_pressed()
{
    Computer::getDefault()->Undos->redo();
}

void MainWindow::on_consoleEnterButton_pressed()
{
    qDebug("I want to take the input");

    freopen("temp.txt","w",stdout);
    handleConsoleIn(ui->lineEdit->text().toLocal8Bit().data());
    std::fclose(stdout);


    QFile phil("temp.txt");
    phil.open(QIODevice::ReadWrite);
    qDebug("handled");
    QTextStream streamy(&phil);
    while(!streamy.atEnd())
        ui->plainTextEdit->insertPlainText(streamy.readLine()+"\n");


}

void MainWindow::on_continueButton_pressed()
{
    manager->activate(ThreadManager::Flag);

}

void MainWindow::on_RestoreButton_pressed()
{
    MASK
}

void MainWindow::on_SaveButton_pressed()
{
    UNMASK
            IFNOMASK(update();)
}

void MainWindow::on_haltButton_pressed()
{
    Computer::getDefault()->setRunning(false);
}
