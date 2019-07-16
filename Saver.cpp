#include "Saver.h"
#include "computer.h"
#include "ctime"
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include "opcodes.h"
/*
 * Here's the plan.
 * 
 * For saving, there will be a set of steps
 * 
 *  Step 1:  Find out 
 * 
 * 
 */


Saver::Saver()
{

}
enum LabelType{
    DataLabel,
    NonDataLabel
};
void generateNonDataLabelDictionary(LabelType classification[],mem_addr_t start, mem_addr_t end)
{
    //look at all of the values between the start and end.
    //if they point to something,
    //mark that something as either a data labeled row or a nondata labeled row in the dictionary
    //that this method will return

    //Step 1: clean the list
    for(mem_addr_t index = start ; index <= end ; index++)
    {
        classification[index-start] = DataLabel;
    }
    //Step 2: look for NonDataLabels
    for(mem_addr_t index = start ; index <= end ; index++)
    {
        mem_loc_t loc = Computer::getDefault()->getMemLocation(index);
        const mem_addr_t addr = loc.addr;
        val_t val = loc.value;
        val_t masked = val & 0xF000;
        switch (masked) {
        case brOpCode:
        case jmpOpCode:
            mem_addr_t target = Computer::getDefault()->connectedAddress(addr);
            if(target != addr)
            {
                qDebug("It is non-navel gazing");
                if(start <= target && target <= end)
                {
                    qDebug(QString("It turns out that %1 is connected to %2").arg(getHexString(addr)).arg(getHexString(Computer::getDefault()->connectedAddress(addr))).toLocal8Bit());
                    classification[target-start]= NonDataLabel;
                }
            }
            break;

        }
        if(index == end)
        {
            break;
        }
    }
}
void Saver::getSaveBounds(mem_addr_t & start, mem_addr_t &end)
{

}
void Saver::handleLabels(std::ofstream & destination, mem_addr_t addr)
{
    data_t data = Computer::getDefault()->getMemDataType(addr);
    std::string labelString = Computer::getDefault()->getMemNameSafe(addr).toStdString();

    if(labelString != "")
    {
//        if(Computer::getDefault()->getMemLocation(addr).connectors !=)
        destination<<labelString;
        if(data == INSTRUCTION)
        {
            destination<<std::endl;
        }
    }
    destination<<"    ";

}
void Saver::handleValues(std::ofstream & destination, mem_addr_t addr)
{
    data_t typey = Computer::getDefault()->getMemDataType(addr);
    if(typey != INSTRUCTION)
    {
        destination << ".FILL ";
    }

    destination << Computer::getDefault()->displayAddressValue(addr,false,true).toStdString()<<"\t";
}
void Saver::handleComments(std::ofstream & destination, mem_addr_t addr)
{

    QString comment = Computer::getDefault()->getMemComment(addr);

    comment.replace(QRegExp("\n"),"\n\t");
    comment.replace(QRegExp("\b"),"");//fixes bug
//    comment.replace(QRegExp("\b"),Computer::getDefault()->getMemLabel(addr)->name);

    destination << comment.toStdString()<< std::endl;
}
void Saver::handleEnd(std::ofstream & destination)
{
    destination << ".END" << std::endl;
}
void Saver::savePortable( mem_addr_t beginning, mem_addr_t end, bool takeCommentBefore, QString fileName)
{

    std::ofstream destination;
    destination.open(fileName.toLocal8Bit().toStdString());
    if(destination.is_open())
    {
        qDebug(QString("The destination (%1) is open!").arg(fileName).toLocal8Bit());
    }
    else
    {
        qDebug(QString("The destination (%1) is not open!").arg(fileName).toLocal8Bit());
    }
    //First, we need to scan for Labels that are mark sub routines
    LabelType labelTypes[end-beginning];
    generateNonDataLabelDictionary(labelTypes,beginning,end);

    for(mem_addr_t index = beginning ; index <= end ; index++)
    {
        if(labelTypes[index-beginning]==NonDataLabel)
        {
            qDebug(QString("%1 points at somewhere it can go").arg(getHexString(index+beginning)).toLocal8Bit());
        }
    }
    if(takeCommentBefore)
    {
        handleComments(destination,beginning-1);
    }
    destination << "\t.ORIG " << getHexString(beginning).toStdString() <<"\n";
    for(mem_addr_t index = beginning; index <= end; index +=1)
    {
        handleLabels(destination,index);
        handleValues(destination,index);
        handleComments(destination,index);
    }
    handleEnd(destination);
    destination.close();
}

void Saver::vanguard()
{
//    qInfo("Saving and Loading");

    std::ifstream myfileSource;
    saveState();
    loadState();

}
//void Saver::prettySave()
//{

//    savePortable();
//}
void Saver::saveState()
{
    std::time_t timer;
    timer = std::time(NULL);
    std::ofstream myfile;
    myfile.open("TestSave.env");
    Computer::getDefault()->saveComputer(&myfile);
    myfile.close();
    std::time_t t = std::time(NULL);

    std::cout<<"Saving State took:"<<t- timer<<std::endl;
}
void Saver::loadState()
{
    std::time_t timer;
    timer = std::time(NULL);

    std::ifstream myfileSource;

    myfileSource.open("TestSave.env");
    Computer::getDefault()->loadComputer(&myfileSource);
    myfileSource.close();
    std::time_t t = std::time(NULL);
    std::cout<<"Loading State took:"<<t- timer<<std::endl;

}

