#ifndef HISTORYHANDLER_H
#define HISTORYHANDLER_H
#include <QUndoCommand>
#include <QUndoStack>
#include <QStack>
#include "Utility.h"
extern "C"
{
#include "Simulator.h"
#include "Registers.h"
#include "Memory.h"
}

class HistoryHandler: public QUndoStack
{
    Q_OBJECT
public:
    HistoryHandler();



    void update(){BATHTIME("Stack has been changed; update")};
};

#define INITFUNC(TEXT,STUFF) \
{\
    setText(TEXT);\
    STUFF\
}

#define UNDOFUNC(STUFF)  virtual void undo()\
{\
    STUFF\
}
#define REDOFUNC(STUFF)  virtual void redo()\
{\
    STUFF\
}



namespace Action
{

class changeRegCondt: public QUndoCommand
{
    changeRegCondt(cond_t cond):newCondt(cond),oldCondt(getProgramStatus()){setText("set Condition");}
    UNDOFUNC
    (
        setProgramStatus(oldCondt);
    )
    REDOFUNC
    (
        setProgramStatus(newCondt);
    )
private:
    cond_t newCondt;
    cond_t oldCondt;

};

class changeRegValue: public QUndoCommand
{
public:
    changeRegValue(reg_t reg, val_t val):regName(reg),oldValue(getRegister(reg)),newValue(val)
    INITFUNC
    (
        "set Register",
    )
    UNDOFUNC
    (
        setRegister(regName,oldValue);
    )
    REDOFUNC
    (
        setRegister(regName,newValue);
    )
private:
    reg_t regName;
    val_t newValue;
    val_t oldValue;
};
class changeMemValue: public QUndoCommand
{
public:
    changeMemValue(mem_addr_t addr,val_t val):mem_addr(addr),oldValue(getMemValue(addr)),newValue(val)
    INITFUNC
    (
        "set Memory",

    )
    UNDOFUNC
    (
        setMemValue(mem_addr,oldValue);
    )
    REDOFUNC
    (
        setMemValue(mem_addr,newValue);
    )
      virtual ~changeMemValue() {;}
private:
    mem_addr_t mem_addr;
    val_t oldValue;
    val_t newValue;
};
class changeMemLabel: public QUndoCommand
{
public:
    changeMemLabel(mem_addr_t addr,label_t* newLabel):mem_addr(addr),oldLabelPtr(getMemLabel(addr)),newLabelPtr(newLabel)
    INITFUNC
    (
        "set Label"
      ,
    )
    UNDOFUNC
    (
        setMemLabel(mem_addr,oldLabelPtr);
    )
    REDOFUNC
    (
        setMemLabel(mem_addr,newLabelPtr);
    )
private:
    mem_addr_t mem_addr;
    label_t* oldLabelPtr;
    label_t* newLabelPtr;
};
class changeMemBreak: public QUndoCommand
{
public:
    changeMemBreak(mem_addr_t addr,breakpoint_t* breakPtr):mem_addr(addr),oldBreak(getMemBreakPoint(addr)),newBreak(breakPtr){setText("set Break");}
    UNDOFUNC
    (
        setMemBreakPoint(mem_addr,oldBreak);
    )
    REDOFUNC
    (
        setMemBreakPoint(mem_addr,newBreak);
    )
private:
    mem_addr_t mem_addr;
    breakpoint_t* oldBreak;
    breakpoint_t* newBreak;
};
}
class changeMany:public QUndoCommand
{
    typedef struct
    {
        union
        {
        mem_addr_t addr;
        reg_t reg;
        };
        val_t oldVal;
        val_t newVal;
    } slice;
public:
    changeMany(val_t* old_Mem_State,val_t* new_Mem_State,val_t* old_Reg_State,val_t* new_Reg_State)
    INITFUNC("Recording Differences",

        idMemChanges(old_Mem_State,new_Mem_State);
        idRegChanges(old_Reg_State,new_Reg_State);
    )
    UNDOFUNC
    (

    )
    REDOFUNC
    (

    )
private:
        /** This function is used to find the differences between the memory
         * at two different points, trading speed for size
         * @brief idMemChanges
         * @param old_Mem_State
         * @param new_Mem_State
         */
    void idMemChanges(val_t* old_Mem_State, val_t* new_Mem_State)
    {
        changedMemCount = 0;
        //Count the number of differences
        for(int i = 0;i<65535;i++)
        {
            if(old_Mem_State[i]!=new_Mem_State[i])
            {
            changedMemCount++;

            BATHTIME(QString(getHexString(i)))
            }
        }
        //make a list that is the right size to hold the differences
        memSlices = static_cast<slice*>(malloc(sizeof(slice) * changedMemCount));
        int index = 0;
        //record said differences
        for(int i = 0;i<65535;i++)
        {
            if(old_Mem_State[i]!=new_Mem_State[i])
            {
            memSlices[index].addr = i;
            memSlices[index].newVal=new_Mem_State[i];
            memSlices[index].oldVal=old_Mem_State[i];
            index++;


            BATHTIME(QString(getHexString(i)))
            }
        }
    }
    void idRegChanges(val_t* old_Reg_State, val_t* new_Reg_State)
    {
        int index = 0;
        changedRegCount= 0;
        for(int i = 0;i<number_of_regs;i++)
        {
            if(old_Reg_State[i]!=new_Reg_State[i])
            {
                changedRegCount++;
            }
        }
        regSlices = static_cast<slice*>(malloc(sizeof(slice) * changedRegCount));
        for(int i = 0;i<number_of_regs;i++)
        {
            if(old_Reg_State[i]!=new_Reg_State[i])
            {
            regSlices[index].reg = static_cast<reg_t>(i);
            regSlices[index].newVal=new_Reg_State[i];
            regSlices[index].oldVal=old_Reg_State[i];
            index++;


            BATHTIME(QString(getHexString(i)))
            }
        }
    }
    void unRegChanges()
    {
    int i = 0;
    }

    slice *memSlices;
    slice *regSlices;
    mem_addr_t* changed_Mem;
    int changedMemCount;
    int changedRegCount;

};

#endif // HISTORYHANDLER_H
