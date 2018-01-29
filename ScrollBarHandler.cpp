#include "ScrollBarHandler.h"
#include "computer.h"
#include "QLinkedList"
ScrollBarHandler::ScrollBarHandler(QObject *parent) : QObject(parent){}

HighlightScrollBar* ScrollBarHandler::generateBar(QWidget* parent)
{
    HighlightScrollBar* newBar = new HighlightScrollBar(Qt::Vertical,parent);
    bars.append(newBar);
    return newBar;
}
void ScrollBarHandler::addScrollBar(HighlightScrollBar* Bar)
{
    bars.append(Bar);
}
void ScrollBarHandler::removeScrollBar(HighlightScrollBar* Bar)
{
    bars.removeOne(Bar);
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
