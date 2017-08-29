#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDebug>
#include <QScrollArea>
#include "BetterScrollbar.h"
#include "RegisterModel.h"
#include "Simulator.h"
#include "Util.h"
#include "stdio.h"
#include "hope.h"
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
#include <QSettings>
#define REGISTERVIEWNUMCOLUMN 2

#define SCROLLTO(VIEW,INPUT)\
QModelIndex  a =(VIEW)->model()->index(INPUT,0);\
(VIEW)->scrollTo(a,QAbstractItemView::PositionAtTop);

#define MEMVIEWSETUPPERCENT 20

#define UPDATEVIEW(TABLEVIEW) TABLEVIEW->hide();TABLEVIEW->show();



#define SETUPDISPLAY(UI,THIS)\
    BATHTIME("Setting up the display")\
    disp = new Hope(THIS);\
    UI->verticalLayout_11->addWidget(disp,0,Qt::AlignCenter);\
    disp->autoFillBackground();\
    disp->setMinimumSize(SCREEN_WIDTH,SCREEN_HEIGHT);\
    disp->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);\

#define FINISHING_TOUCHES(DISP,MODEL)\
    disp->update();


#define CLEAR(INPUT) if(CLEARONGOTO)INPUT->clear();

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
    setUpUndoStack();//QED
    setupThreadManager();//QED


    QFuture<void> f1 = QtConcurrent::run(threadTest,QString("1"));
    f1.waitForFinished();




    BATHTIME(getBinString(0xA1B2));
    BATHTIME("About to setup ui")
    ui->setupUi(this);//this puts everything in place

    SETUPDISPLAY(ui,this)
    setupRegisterView();
    setupViews();
    Bridge::doWork();
    BATHTIME("Connecting Disp")
    QObject::connect(disp,SIGNAL(mouseMoved(QString)),ui->statusBar,SLOT(showMessage(QString)));
    BATHTIME("Connecting ")
    QObject::connect(ui->actionClear,SIGNAL(triggered()),disp,SLOT(clearScreen()));
    QObject::connect(ui->NextButton,SIGNAL(on_NextButton_pressed()),ui->RegisterView,SLOT(update_RegisterView()));
    readSettings();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setUpUndoStack()
{
    Undos =  new QUndoStack();
    Undos->setUndoLimit(65535);
}

void MainWindow::setupViews()
{
    BATHTIME("Setting Up Views")
    BATHTIME("Now will be making the model");
    this->model = new modeler(this, threadRunning);
    this->StackModel = new StackModeler(this,threadRunning);
    model->setQUndoStack(Undos);



    qInfo("Header made");

    qInfo("Size Set");


    QString str;


    setupMemView(ui->MemView1View);
    setupMemView(ui->MemView2View);
    setupMemView(ui->MemView3View);
    setupStackView(ui->StackViewView);
//    MEMVIEWSETUP(ui->MemView2View,model);
//    MEMVIEWSETUP(ui->MemView3View,model);

    ui->MemView1View->setVerticalScrollBar(new BetterScrollBar());

    BATHTIME("Model Created");
    /*
     * There is an assumption that hitting enter will cause an input to be entered.
     * These lines connect the three inputs for the views
     */
    BATHTIME("Connecting View interfaces")
    {
        connect(ui->MemView1Input,SIGNAL(returnPressed()),ui->MemView1GotoButton,SLOT(click()));
        connect(ui->MemView2Input,SIGNAL(returnPressed()),ui->MemView2GotoButton,SLOT(click()));
        connect(ui->MemView3Input,SIGNAL(returnPressed()),ui->MemView3GotoButton,SLOT(click()));
        connect(ui->StackViewInput,SIGNAL(returnPressed()),ui->StackViewGotoButton,SLOT(click()));


    }
    BATHTIME("Done Connecting Views")
    BATHTIME("Done Setting Up Views")
}



void MainWindow::setupMemView(QTableView* view)
{

    BATHTIME("Attaching the model to the views")
    BATHTIME("Showing Grid")
    view->showGrid();
    BATHTIME("Setting Model")
    view->setModel(model);
    BATHTIME("Resizing Columns")
    view->resizeColumnsToContents();
    BATHTIME("Hiding vertical Header")
    view->verticalHeader()->hide();
    BATHTIME("setting Column width")
    view->setColumnWidth(0,20);
    view->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);

    view->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);

    view->verticalHeader()->setDefaultSectionSize(20);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

}
void MainWindow::setupStackView(QTableView* view)
{
    BATHTIMEBANNER("Setting up Stack View")
    BATHTIME("Showing Grid")
    view->showGrid();
    BATHTIME("Setting Model")
    view->setModel(StackModel);
    view->resizeColumnsToContents();
    view->verticalHeader()->hide();
//    view->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);

//    view->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    view->verticalHeader()->setDefaultSectionSize(20);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    CONNECT(MainWindow::ui->actionFlip,triggered(),StackModel,flip());
}

void MainWindow::setupRegisterView()
{
    QTableView* view = ui->RegisterView;
    BATHTIME("Initializing model")
    regModel = new RegisterModel(this,threadRunning);
    regModel->setQUndoStack(Undos);
    BATHTIME("Attaching the model to the views")
    view->setModel(regModel);
    BATHTIME("Showing Grid")
    view->showGrid();
    view->setColumnWidth(0,8);
    view->setColumnWidth(1,28);
    view->resizeColumnToContents(reg_value_column);
    BATHTIME("Setting horizantal heading options")
    {
        QHeaderView* hori = view->horizontalHeader();
        hori->hide();
        hori->setSectionResizeMode(reg_color_column,QHeaderView::Fixed);
        hori->setSectionResizeMode(reg_name_column,QHeaderView::Fixed);
        hori->setDefaultAlignment(Qt::AlignRight);
    }
    BATHTIME("Setting vertical heading options")
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
void MainWindow::on_pushButton_7_pressed()
{
//    update();
    BATHTIME("trying to undo")
//    Undos->undo();
            *threadRunning = !*threadRunning;

}
void MainWindow::on_MemView1PCButton_pressed()
{
    SCROLLTO(ui->MemView1View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView2PCButton_pressed()
{
    SCROLLTO(ui->MemView2View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView3PCButton_pressed()
{
    SCROLLTO(ui->MemView3View,Computer::getDefault()->getRegister(PC))
}
void MainWindow::on_MemView1GotoButton_pressed()
{
    SCROLLTO(ui->MemView1View,ui->MemView1Input->text().toInt(NULL,16))
    CLEAR(ui->MemView1Input)
}
void MainWindow::on_MemView2GotoButton_pressed()
{
    SCROLLTO(ui->MemView2View,ui->MemView2Input->text().toInt(NULL,16))
    CLEAR(ui->MemView2Input)
}
void MainWindow::on_MemView3GotoButton_pressed()
{
    SCROLLTO(ui->MemView3View,ui->MemView3Input->text().toInt(NULL,16))
    CLEAR(ui->MemView3Input)
}
void MainWindow::on_StackViewGotoButton_pressed()
{
    SCROLLTO(ui->StackViewView,ui->StackViewInput->text().toInt(NULL, 16))
    CLEAR(ui->StackViewInput)
}
void MainWindow::on_NextButton_pressed()
{
    BATHTIME("Executing Single instruction")
//    executeSingleInstruction();
    manager->activate(1);
    update();

}
void MainWindow::update()
{
    BATHTIME(QString().setNum(Undos->index()))
    disp->update();
    UPDATEVIEW(ui->MemView1View);
    UPDATEVIEW(ui->MemView2View);
    UPDATEVIEW(ui->MemView3View);
    UPDATEVIEW(ui->StackViewView);
    UPDATEVIEW(ui->RegisterView);
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
    *threadRunning = true;

ui->NextButton->setEnabled(false);
emit update();
}
void MainWindow::gotoUserMode()
{
    *threadRunning = false;
    ui->NextButton->setEnabled(true);
    emit update();
}
void MainWindow::prepWork()
{


}

void MainWindow::on_pushButton_4_pressed()
{
    BATHTIME(('0'+(*threadRunning)))

    BATHTIME("Next")
    manager->activate(Bridge::Next);

    update();

}


void MainWindow::on_Update_Temp_pressed()
{

}

void MainWindow::on_IntoButton_pressed()
{
    BATHTIME("Step")
    manager->activate(Bridge::Step);
}

void MainWindow::readSettings()
{
    QSettings settings("Melberg & Ott","PennSim++");

    settings.beginGroup("MainWindow");
    int width = settings.value("Window Width",QVariant(1163)).toInt();
    int height= settings.value("Window Height",QVariant(694)).toInt();
//    int memSplitterWidth = settings.value("Memory Splitter Width",QVariant(334)).toInt();

    int MemoryBoxHeight = settings.value("Memory Box Height",QVariant(635)).toInt();
    int MemoryBoxWidth  = settings.value("Memory Box Width" ,QVariant(354)).toInt();
    ui->MemorySplitter->restoreState(settings.value("Memory Splitter State").toByteArray());
    this->resize(width,height);
    ui->MemoryBox->resize(MemoryBoxWidth,MemoryBoxHeight);

    settings.endGroup();
}
void MainWindow::saveSettings()
{
    QSettings settings("Melberg & Ott","PennSim++");
    settings.beginGroup("MainWindow");
    settings.setValue("Window Height",this->height());
    settings.setValue("Window Width",this->width());
//    settings.setValue("Memory Splitter Width",this->ui->)
    settings.setValue("Memory Box Height", ui->MemoryBox->height());
    settings.setValue("Memory Box Width",ui->MemoryBox->width());
    settings.setValue("Memory Splitter State",ui->MemorySplitter->saveState());
//    settings.setValue("Undos",Undos->)
    settings.endGroup();
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

void MainWindow::on_MemView1_destroyed()
{
    BATHTIME("Hey")
}
