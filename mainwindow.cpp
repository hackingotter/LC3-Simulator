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
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include "MemTable.h"
#include <QDataStream>
#include "Assembler.h"
#include <QUndoView>
#include <QItemSelectionModel>
#include <map>
#include <QFile>
#include "KBRDModel.h"
#include <QProcess>
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
    qDebug("Setting up the display");\
    disp = new Hope(THIS);\
    UI->verticalLayout_11->addWidget(disp,0,Qt::AlignCenter);\
    disp->autoFillBackground();\
    disp->setMinimumSize(SCREEN_WIDTH,SCREEN_HEIGHT);\
    disp->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);\

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

    Computer::getDefault()->setProgramStatus(cond_z);

    Utility::systemInfoDebug();//Just some fun info

    setupThreadManager();//QED


    QFuture<void> f1 = QtConcurrent::run(threadTest,QString("1"));
    f1.waitForFinished();

    QProcess* th = new QProcess(this);


    qDebug(QString().setNum(th->processId()).toLocal8Bit());



    qDebug("About to setup ui");
    ui->setupUi(this);//this puts everything in place

    SETUPDISPLAY(ui,this)
    setupMenuBar();
    setupRegisterView();
    setupViews();
    setupControlButtons();
    setupInOut();
    setupWatches();



    Bridge::doWork();
    qDebug("Connecting Disp");

//    QObject::connect(Computer::getDefault() ,SIGNAL(update()),disp,SLOT(update()));
    QObject::connect(Computer::getDefault() ,SIGNAL(update()),this,SLOT(update()));
    QObject::connect(disp,SIGNAL(mouseMoved(QString)),ui->statusBar,SLOT(showMessage(QString)));
    qDebug("Connecting ");

    QObject::connect(ui->actionClear,SIGNAL(triggered()),disp,SLOT(clearScreen()));

    ui->undoStackSpot->addWidget(new QUndoView(Computer::getDefault()->Undos));
//    QObject::connect(ui->NextButton,SIGNAL(on_NextButton_pressed()),ui->RegisterView,SLOT(update()));
    readSettings();
//    setupMenuBar();
//    Computer::getDefault()->loadProgramFile(QString("testing.asm").toLocal8Bit().data());

    update();
//    qDebug(QString().setNum(th->processId()).toLocal8Bit());




//    ui->MemView1->layout()->addWidget(new MemWindow(model));




}
MainWindow::~MainWindow()
{
    delete ui;
}

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
//    ui->MemorySplitter->addWidget(new MemWindow(model));
//    setupMemView(ui->MemView1View);
    setupMemView(ui->MemView2View);
    setupMemView(ui->MemView3View);
    setupStackView(ui->StackViewView);
//    MEMVIEWSETUP(ui->MemView2View,model);
//    MEMVIEWSETUP(ui->MemView3View,model);



    qDebug("Model Created");
    /*
     * There is an assumption that hitting enter will cause an input to be entered.
     * These lines connect the three inputs for the views
     */
    qDebug("Connecting View interfaces");
    {
     //   connect(ui->MemView1Input,SIGNAL(returnPressed()),ui->MemView1GotoButton,SLOT(click()));
        connect(ui->MemView2Input,SIGNAL(returnPressed()),ui->MemView2GotoButton,SLOT(click()));
        connect(ui->MemView3Input,SIGNAL(returnPressed()),ui->MemView3GotoButton,SLOT(click()));
        connect(ui->StackViewInput,SIGNAL(returnPressed()),ui->StackViewGotoButton,SLOT(click()));


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
    QAction* actionSave_File_As= new QAction("Save File As ...");


    actionAssemble_Load_File->setShortcut(QKeySequence(tr("Ctrl+D")));


    QList<QAction*> fileActions;
    fileActions <<actionLoad_File<<actionAssemble_File<<actionAssemble_Load_File<<actionSave_File<<actionSave_File_As;
    ui->menuFile->addActions(fileActions);
    CONNECT(actionLoad_File,triggered(),this,loadFile());
    CONNECT(actionAssemble_Load_File,triggered(),this, assembleNLoadFile());

//    MainWindow::ui-

}
void MainWindow::setupControlButtons()
{
CONNECT(ui->haltButton,pressed(),manager,requestHalt());
}
bool MainWindow::loadFile(QString path)
{
    bool success = false;
    Computer::getDefault()->Undos->beginMacro("Load "+ path);
    qDebug("Attempting to load a program");
    if(path==QString())
    {
        qDebug("Looks like there was no file specified.  Time for the user to choose.");
        path = QFileDialog::getOpenFileName(this,"Select a file to load");
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
    std::cout<<"An unforseen error has occured"<<endl;
    return "";
    }

    return "Test.obj";


}
void MainWindow::assembleNLoadFile(QString path)
{

    QFileDialog* fileUI = new QFileDialog();
    fileUI->setNameFilter(QString("Assmbly (*").append(ASSEMBLY_SUFFIX).append(")"));
    fileUI->setReadOnly(true);
    fileUI->setWindowTitle("Choose a file to assemble and load into memory");


    Assembler embler = Assembler();

    if(path==QString())
    {
        qDebug("Looks like there was no file specified.  Time for the user to choose.");
        path = fileUI->getOpenFileName();
    }

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
    }
    catch(const std::string& e)
    {
        std::cout<<e<<endl;
        return;
    }
    if(loadFile(namePath))
    {
        qDebug("successfully loaded");

    }

    Computer::getDefault()->Undos->endMacro();
//    embler.assembleFile();

}
void MainWindow::handleFiles()
{
    Assembler Bill;
    QString inputPath = QFileDialog::getOpenFileName().toLocal8Bit().data();
    Bill.assembleFile(inputPath.toLatin1().data(),"LC3Maybe.obj");
    Computer::getDefault()->loadProgramFile("LC3Maybe.obj");
    IFNOMASK(emit update();)
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

//    QObject::connect(Computer::getDefault(),SIGNAL(update()),view,SLOT(repaint()));
}

void MainWindow::setupInOut()
{

    InOutPut = new InOutSet(this);
    ui->inOutHome->addWidget(InOutPut);
    CONNECT(this, reCheck(), InOutPut, update());



}
void MainWindow::onTableClicked(const QModelIndex & current)
{
    int column = current.column();
    int row = current.row();

    qDebug("Selection at (" + QString().setNum(row).toLocal8Bit() + ", " + QString().setNum(column).toLocal8Bit() + ")");

}

void MainWindow::setupStackView(QTableView* view)
{
    qDebug("Setting up Stack View");
    qDebug("Showing Grid");

    qDebug("Setting Model");
    view->setModel(StackModel);
    //view->resizeColumnsToContents();
    view->verticalHeader()->hide();

    view->setColumnWidth(STACK_VIEW_ADR_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_ADR_COL,QHeaderView::Fixed);

    view->setColumnWidth(STACK_VIEW_VAL_COL,HEX_COLUMN_WIDTH);
    view->horizontalHeader()->setSectionResizeMode(MEM_VIEW_VAL_COL,QHeaderView::Fixed);
    view->showGrid();
    // set row height and fix it
    view->verticalHeader()->setDefaultSectionSize(20);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    CONNECT(MainWindow::ui->actionFlip,triggered(),StackModel,flip());
    CONNECT(StackModel,flip(),this,update());
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
void MainWindow::on_MemView1Input_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}
void MainWindow::on_MemView2Input_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}
void MainWindow::on_MemView3Input_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}
void MainWindow::on_StackViewInput_returnPressed()
{
    //This is just here so that the corressponding GotoButton can listen to it
}

void MainWindow::on_MemView1PCButton_pressed()
{
    qDebug("hello");
    bool* ok = static_cast<bool*>(malloc(sizeof(bool)));
    SCROLLTO(ui->MemView1View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView2PCButton_pressed()
{
    bool* ok;
    SCROLLTO(ui->MemView2View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView3PCButton_pressed()
{
    bool* ok;
    SCROLLTO(ui->MemView3View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView1GotoButton_pressed()
{
    qDebug("MemView1Goto or PC was pressed, checking if target is valid.");
    bool ok = true;
//    int target = Utility::unifiedInput2Val(ui->MemView1Input->text(),&ok);

//    std::cout << ok << std::endl;
//    if(ok)
//    {
//        qDebug("It was valid");
//    QModelIndex  a =(ui->MemView1View)->model()->index(target,0);
//    (ui->MemView1View)->scrollTo(a,QAbstractItemView::PositionAtTop);
//    } else {
//        qDebug("It was not valid");
//    }
    int target = Utility::unifiedInput2Val(ui->MemView1Input->text(),&ok);
    if(ok)
    {
    SCROLLTO(ui->MemView1View,target)
    }
    CLEAR(ui->MemView1Input)
}
void MainWindow::on_MemView2GotoButton_pressed()
{
    bool ok = true;
    int target = Utility::unifiedInput2Val(ui->MemView2Input->text(),&ok);
    if(ok)
    {
    SCROLLTO(ui->MemView2View,target)
    }
    CLEAR(ui->MemView2Input)
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
    bool ok = true;
    int target =Utility::unifiedInput2Val(ui->StackViewInput->text(),&ok);
    if(ok)
    {
        SCROLLTO(ui->StackViewView,target);
    }
    CLEAR(ui->StackViewInput)
}
void MainWindow::on_NextButton_pressed()
{
    qDebug("Executing Single instruction");
//    executeSingleInstruction();
    manager->activate(1);
//    update();

}
void MainWindow::update()
{
    disp->update();
    UPDATEVIEW(ui->MemView1View);
    UPDATEVIEW(ui->MemView2View);
    UPDATEVIEW(ui->MemView3View);
    UPDATEVIEW(ui->StackViewView);
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
    manager->activate(Bridge::Next);

    update();

}


void MainWindow::on_Update_Temp_pressed()
{

}

void MainWindow::on_IntoButton_pressed()
{
    qDebug("Step");
    manager->activate(Bridge::Step);
}

void MainWindow::readSettings()
{
    QSettings settings("Melberg & Ott","PennSim++");
    settings.beginGroup("MainWindow");
    qDebug("heylo");
    int width = settings.value("Window Width",QVariant(1163)).toInt();
    int height= settings.value("Window Height",QVariant(694)).toInt();
    int MemoryBoxHeight = settings.value("Memory Box Height",QVariant(635)).toInt();
    int MemoryBoxWidth  = settings.value("Memory Box Width" ,QVariant(354)).toInt();
    ui->MemorySplitter->restoreState(settings.value("Memory Splitter State").toByteArray());
    this->resize(width,height);
    ui->MemoryBox->resize(MemoryBoxWidth,MemoryBoxHeight);
    setWindowState(static_cast<Qt::WindowState>(settings.value("Window State",QVariant(Qt::WindowMaximized)).toInt()));
    settings.endGroup();
    qDebug("theylo");


}
void MainWindow::saveSettings()
{
    QSettings settings("Melberg & Ott","PennSim++");
    settings.beginGroup("MainWindow");
    settings.setValue("Window Height",this->height());
    settings.setValue("Window Width",this->width());
    settings.setValue("Memory Box Height", ui->MemoryBox->height());
    settings.setValue("Memory Box Width",ui->MemoryBox->width());
    settings.setValue("Memory Splitter State",ui->MemorySplitter->saveState());
    settings.setValue("Window State",static_cast<int>(windowState()));
    settings.endGroup();

    qDebug("done saving");



}
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
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
}



