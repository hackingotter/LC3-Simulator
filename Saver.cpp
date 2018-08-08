#include "Saver.h"
#include "computer.h"
#include "ctime"
#include <iostream>
#include <fstream>
Saver::Saver()
{

}
void Saver::savePortable( mem_addr_t beginning, mem_addr_t end, bool takeCommentBefore)
{
    QString fileName = QString("TestSaveProt.asm");
    std::ofstream destination;
    destination.open(fileName.toLocal8Bit().toStdString());

    for(mem_addr_t index = beginning; index <= end; index +=1)
    {
        //if the scan includes the final address, then the unsigned index will
        //wrap over to 0, which is still less than the end index, so this must
        // be addressed.
        if(index < beginning)
        {
            break;
        }
        {


            std::string labelString = Computer::getDefault()->getMemNameSafe(index).toStdString();
            data_t data = Computer::getDefault()->getMemDataType(index);

            if(labelString != "")
            {
                destination<<labelString;
                if(data == INSTRUCTION)
                {
                    destination<<std::endl;
                }
            }
            destination<<"    ";

            switch(Computer::getDefault()->getMemDataType(index))
            {
            case INSTRUCTION:
                destination<<Computer::getDefault()->mnemGen(index).toStdString();
                break;
            default:
                destination<<"Nothing Yet";
            }

//            std::string mnemString = (Computer::getDefault()->mnemGen(index)).toStdString();

//            destination<<mnemString;

            destination<<std::endl;

        }
    }
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

