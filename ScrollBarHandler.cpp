#include "ScrollBarHandler.h"
#include "computer.h"
#include "QLinkedList"
ScrollBarHandler::ScrollBarHandler(QObject *parent) : QObject(parent)
{
//    bars = new QLinkedList<H/>();

}
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
}
