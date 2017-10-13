#include "ScrollBarHandler.h"
#include "computer.h"
#include "QLinkedList"
ScrollBarHandler::ScrollBarHandler(QObject *parent) : QObject(parent){}
void ScrollBarHandler::addScrollBar(HighlightScrollBar* bar)
{
    bars.append(bar);
}
void ScrollBarHandler::update()
{
    //get new data
    val_t* regvals = Computer::getDefault()->getAllRegisters();
    foreach(HighlightScrollBar* bar, bars)
    {
        //update registers
        bar->removeHighlights(Highlight::regpoint);

        for(int i = 0;i<= 9;i++)
        {
            bar->addHighlight(Highlight(Highlight::regpoint,regvals[i],i,Highlight::NormalPriority));
        }
    }
    for(mem_addr_t addr = 0; addr < 0xFFFF;addr++)
    {
        //update registers
        breakpoint_t breaker = Computer::getDefault()->getMemBreakPoint(addr);
        if(breaker!=nullptr)
        {
            foreach(HighlightScrollBar* bar, bars)
            {
                bar->addHighlight(Highlight(Highlight::breakpoint,addr,2,Highlight::NormalPriority));
            }
        }
    }
}
