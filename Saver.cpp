#include "Saver.h"
#include "computer.h"
#include "ctime"
#include <iostream>
#include <fstream>
Saver::Saver()
{

}
void Saver::handleLabels(std::ofstream & destination, mem_addr_t addr)
{
    data_t data = Computer::getDefault()->getMemDataType(addr);
    std::string labelString = Computer::getDefault()->getMemNameSafe(addr).toStdString();

    if(labelString != "")
    {
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
    destination << Computer::getDefault()->displayAddressValue(addr,false).toStdString()<<"\t";
}
void Saver::handleComments(std::ofstream & destination, mem_addr_t addr)
{

    QString comment = Computer::getDefault()->getMemComment(addr);

    comment.replace(QRegExp("\n"),"\n\t");

    destination << comment.toStdString()<< std::endl;
}
void Saver::handleEnd(std::ofstream & destination)
{
    destination << ".END" << std::endl;
}
void Saver::savePortable( mem_addr_t beginning, mem_addr_t end, bool takeCommentBefore)
{
    QString fileName = QString("TestSaveProt.asm");
    std::ofstream destination;
    destination.open(fileName.toLocal8Bit().toStdString());
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
    qInfo("Saving and Loading");

    std::ifstream myfileSource;
    saveState();
    loadState();

}
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

