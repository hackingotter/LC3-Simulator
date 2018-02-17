#include "computer.h"
#include "opcodes.h"
#include "Util.h"
#include <QString>
#include <QUndoCommand>
#include <QLabel>
#include "iostream"

#define UNDOS

#define TRY2PUSH(OLD,NEW,DO) {if((Computer::remember==0))Computer::Undos->add(new Action::DO);}
#define INITFUNC(TEXT,STUFF) \
{\
    setText(TEXT);\
    STUFF;\
    }

#define UNDOFUNC(STUFF)  void undo()\
{\
    STUFF\
    }
#define REDOFUNC(STUFF)  void redo()\
{\
    STUFF\
    }

#define SAVEFUNC(STUFF)  QString save()\
{\
    return STUFF\
    }

namespace Action
{
enum doPriority{
    systLevel = 0,//for when the system does something
    userLevel = 1,

};
static bool remember;
class PrioritizedCommand: public QUndoCommand
{
public:
    doPriority mode;

};
class changeTextDisplay : public QUndoCommand
{
public:
    changeTextDisplay(QLabel* target,char newchar):_target(target),_newchar(newchar)
    {
        setText("Pushed " +QString(&_newchar) + " to the display");

    }
    void undo()
    {
        Computer::getDefault()->remember++;

        Computer::getDefault()->remember--;
    }
    void redo()
    {
        Computer::getDefault()->remember++;
        _target->setText(_target->text()+(char)(_newchar));
        Computer::getDefault()->remember--;
    }
private:
    QLabel* _target;
    char _newchar;
};
class changeRegCondt: public QUndoCommand
{
public:
    changeRegCondt(cond_t ncond,cond_t ocond):newCondt(ncond),oldCondt(ocond)
    {


        setText(QString("Set Condition to "+QString().setNum(ncond)));
    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setProgramStatus(oldCondt);
        Computer::getDefault()->remember--;
    }

    void redo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setProgramStatus(newCondt);
        Computer::getDefault()->remember--;
    }
private:
    cond_t newCondt;
    cond_t oldCondt;

};
class changeMemBlock: public QUndoCommand
{
public:
    changeMemBlock(mem_loc_t* newBlock,val_t newLen,mem_loc_t* oldBlock,val_t oldBlockLen)
        :captured(newBlock),capLength(newLen),overwritten(oldBlock),overLength(oldBlockLen)
    {}
    void undo()
    {
        Computer::getDefault()->remember++;
        //        Computer::getDefault()->set
        //                Computer::getDefault()->setProgramStatus(oldCondt);
        Computer::getDefault()->remember--;
    }

    void redo()
    {
        Computer::getDefault()->remember++;
        //        Computer::getDefault()->setProgramStatus(newCondt);
        Computer::getDefault()->remember--;
    }
private:
    mem_loc_t* captured;
    val_t capLength;
    mem_loc_t* overwritten;
    val_t overLength;
};
class changeMemLoc:public QUndoCommand
{
public:
    bool fresh = true;
    changeMemLoc(mem_loc_t oldL,mem_loc_t newL):newLoc(newL),oldLoc(oldL){
        addr=oldLoc.addr;
        setText("movin'");

    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemLoc(addr,oldLoc);
        Computer::getDefault()->remember--;
    }

    void redo()
    {
        if(!fresh)
        {
            Computer::getDefault()->remember++;
            Computer::getDefault()->setMemLoc(addr,newLoc);
            Computer::getDefault()->remember--;

        }
        fresh = false;
    }
private:
    mem_loc_t newLoc;
    mem_loc_t oldLoc;
    mem_addr_t addr;
};
class changeRegValue: public QUndoCommand
{
public:
    changeRegValue(reg_t reg,val_t oval, val_t nval):regName(reg),newValue(nval),oldValue(oval)
    {

        setText(    QString("Set "+ ((regName<8)?"R"+QString().setNum(regName):" other") + " to "+QString().setNum(newValue)));
    }
    void undo()
    {
        Computer::getDefault()->remember++;

        Computer::getDefault()->setRegister(regName,oldValue);
        Computer::getDefault()->remember--;
    }
    void redo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setRegister(regName,newValue);
        Computer::getDefault()->remember--;
    }

private:
    reg_t regName;
    val_t newValue;
    val_t oldValue;
};
class changeMemValue: public QUndoCommand
{

public:
    changeMemValue(mem_addr_t addr,val_t oval,val_t nval):mem_addr(addr),oldValue(oval),newValue(nval)
    {
        qDebug("I am");
        setText(QString("Set " + getHexString(addr) + " to "+QString().setNum(newValue)));
        if(oval==nval)
        {
            this->setObsolete(true);
        }

    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemValue(mem_addr,oldValue);
        if(mem_addr == DDR)
            Computer::getDefault()->popDisplay();
        Computer::getDefault()->remember--;
    }
    void redo()
    {
        if(!fresh)
        {
            Computer::getDefault()->remember++;
            Computer::getDefault()->setMemValue(mem_addr,newValue);
            if(mem_addr == DDR)
                Computer::getDefault()->pushDisplay(newValue);
            Computer::getDefault()->remember--;
        }
        fresh = false;
    }

    ~changeMemValue() {;}
private:
    mem_addr_t mem_addr;
    val_t oldValue;
    val_t newValue;
    bool fresh = true;
};
class changeMemLabel: public QUndoCommand
{
public:
    changeMemLabel(mem_addr_t addr,label_t* oldLabel,label_t* newLabel):mem_addr(addr),oldLabelPtr(oldLabel),newLabelPtr(newLabel)
    {
        setText("set Label");
        QUndoCommand::setObsolete(oldLabelPtr && // null check
                                  (newLabelPtr->name == oldLabelPtr->name) &&
                                  (newLabelPtr->addr==oldLabelPtr->addr));
    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemLabel(mem_addr,oldLabelPtr);
        Computer::getDefault()->remember--;
    }
    void redo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemLabel(mem_addr,newLabelPtr);
        Computer::getDefault()->remember--;
    }

private:
    mem_addr_t mem_addr;
    label_t* oldLabelPtr;
    label_t* newLabelPtr;
};
class changeMemBreak: public QUndoCommand
{
public:
    changeMemBreak(mem_addr_t addr,breakpoint_t* obreakPtr, breakpoint_t* nbreakPtr):mem_addr(addr),oldBreak(obreakPtr),newBreak(nbreakPtr)
    {
        setText("set Break");
        QUndoCommand::setObsolete(newBreak==oldBreak);
    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemBreakPoint(mem_addr,oldBreak);
        Computer::getDefault()->remember--;
    }
    void redo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemBreakPoint(mem_addr,newBreak);
        Computer::getDefault()->remember--;
    }
private:
    mem_addr_t mem_addr;
    breakpoint_t* oldBreak;
    breakpoint_t* newBreak;
};
class changeMemComment:public QUndoCommand
{
public:
    changeMemComment(mem_addr_t addr,QString oldCom, QString newCom):mem_addr(addr),oldComment(oldCom),newComment(newCom)
    {
        setText("Changed Comment at " + getHexString(addr)+ " to " + newComment);
        QUndoCommand::setObsolete(newComment==oldComment);
    }
    void undo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemComment(mem_addr,oldComment);
        Computer::getDefault()->remember--;
    }
    void redo()
    {
        Computer::getDefault()->remember++;
        Computer::getDefault()->setMemComment(mem_addr,newComment);
        Computer::getDefault()->remember--;
    }
private:
    mem_addr_t mem_addr;
    QString oldComment;
    QString newComment;
};





}

Computer::Computer(QObject *parent) : QObject(parent)
{
    Undos = new HistoryHandler();
    Undos->setUndoLimit(65535);
    //critical for being able to undo the moves
    for(int i = 0;i<=65535;i++)
    {
        _memory[i].addr=i;
    }

    savedSSP = 0;
    savedUSP = 0;
    activeStack = supervisorStack;
}

// default

static Computer* defaultComputer = 0;

Computer* Computer::getDefault() {
    if (defaultComputer) {
        return defaultComputer;
    }
    defaultComputer = new Computer();
    return defaultComputer;
}

// registers

val_t Computer::getRegister(reg_t reg) {
    return registers[reg];
}

val_t* Computer::getAllRegisters() {
    val_t *ret = (val_t*)malloc(NUM_OF_REGS * sizeof(val_t));

    if (!ret) {
        throw("Malloc error");
        return ret;
    }

    memcpy(ret,registers,NUM_OF_REGS * sizeof(val_t));
    return ret;
}

void Computer::setRegister(reg_t reg, val_t val) {
    //will implement an identification method
    val_t oval = registers[reg];
    registers[reg] = val;
    TRY2PUSH(oval, val,changeRegValue(reg,oval,val));

    IFNOMASK(emit update();)
}

void Computer::setAllRegister(val_t vals[]) {
    if (!vals) {
        throw "Null pointer error";
    } else {
        memcpy(registers, vals, NUM_OF_REGS * sizeof(val_t));
    }
    //needs to have a way of remebering
    IFNOMASK(emit update();)
}


// N is 2 bit
// Z is 1 bit
// P is 0 bit
cond_t Computer::getProgramStatus() {

    int p = getRegister(PSR) & 1;
    int z = getRegister(PSR) & 2;
    int n = getRegister(PSR) & 4;

    if (p + (z>>1) + (n>>2) > 1) {
        // multiple are set
        return cond_invalid;
    }

    if (p) {
        return cond_p;
    } else if (z) {
        return cond_z;
    } else  if (n) {
        return cond_n;
    } else {
        return cond_none;
    }
}

void Computer::setProgramStatus(cond_t stat) {

    val_t curr = getRegister(PSR);

    curr &= (!7);
    switch (stat) {
    case cond_p:
        curr ^= 1;
        break;
    case cond_z:
        curr ^= 2;
        break;
    case cond_n:
        curr ^= 4;
        break;
    default:
        throw "Invalid arg error";
        return;
        break;
    }

    this->setRegister(PSR, curr);

    IFNOMASK(emit update();)
}

void Computer::setPriviliged(bool priv)
{
    val_t oldPSR = getRegister(PSR);
    if (priv) {
        setActiveStack(supervisorStack);
        oldPSR |= 0x8000; // this will force bit 15 to 1 but maintain all others
    } else {
        setActiveStack(userStack);
        oldPSR &= 0x7FFF; // this will force bit 15 to 0 but maintain all others
    }

    setRegister(PSR,oldPSR);
}

bool Computer::getPriviliged()
{
    return getRegister(PSR) & 0x8000; // bit 15 is privilige bit
}

void Computer::setActiveStack(stack_type s)
{
    if (s == activeStack) return;

    if (s == userStack) {
        savedSSP = getRegister(R6);
        setRegister(R6,savedUSP);
    } else {
        savedUSP = getRegister(R6);
        setRegister(R6,savedSSP);
    }
}

stack_type Computer::getActiveStackType()
{
    return activeStack;
}

bool Computer::isRunning()
{
    return getMemValue(MCR) & bitMask(15); // bit 15 of the MCR is clock enabled
}

void Computer::setRunning(bool run)
{
    val_t mcr = getMemValue(MCR);
    if (run) {
        mcr |= 0x8000; // set bit 15
    } else {
        mcr &= 0x7FFF; // unset bit 15
    }
    setMemValue(MCR,mcr);
}

// memory

mem_loc_t Computer::getMemLocation(mem_addr_t addr)
{
    return _memory[addr];
}

mem_loc_t *Computer::getMemLocationsBlock(mem_addr_t addr, val_t blockSize)
{
    mem_loc_t * block = (mem_loc_t*)malloc(sizeof(mem_loc_t)*15);
    for(val_t i =0;i<blockSize;i++)
    {
        block[i]=_memory[addr+i+1];
    }
    return block;
}


void Computer::setMemValue(mem_addr_t addr, val_t val)
{
    qDebug("Settin' Mem");
    val_t oval = _memory[addr].value;
    _memory[addr].value = val;

    if(addr>=0xfe00)    qDebug(getHexString(addr).toLocal8Bit());


    TRY2PUSH(oval,val,changeMemValue(addr,oval,val));

    IFNOMASK(emit update();)
}

void Computer::setMemValuesBlock(mem_addr_t addr, size_t blockSize, val_t *vals)
{
    //Implemented for compiler's compliance.
    // TODO
    Undos->beginMacro("set Values Block");
    MASK
            for (size_t i = 0; i < blockSize; i ++) {
        setMemValue(addr + i,vals[i]);
    }
    UNMASK
            Undos->endMacro();
}
void Computer::fillBlock(mem_addr_t begin, mem_addr_t end, val_t val)
{
    Undos->beginMacro("fill block");
    MASK
            for (mem_addr_t i = begin;i<end;i++)
    {
        setMemValue(i,val);
    }
    UNMASK
            IFNOMASK(emit update();)
            Undos->endMacro();
}
val_t Computer::getMemValue(mem_addr_t addr)
{
    return _memory[addr].value;
}

val_t* Computer::getAllMemValues()
{
    mem_addr_t addr;
    val_t * ret = (val_t*)malloc((65535+1) * sizeof(val_t));

    if (!ret)
    {
        throw("Malloc error");
        return ret;
    }
    for(addr =0;addr<=0xFFFF;addr++)
    {
        ret[addr] = _memory[addr].value;
    }

    return ret;
}

void Computer::setMemLabel(mem_addr_t addr,label_t* newLabel)
{
    label_t* oldLabel = _memory[addr].label;
    _memory[addr].label = newLabel;
    TRY2PUSH(oldLabel,newLabel,changeMemLabel(addr,oldLabel,newLabel));

    IFNOMASK(emit update();)
}

void Computer::setMemLabelText(mem_addr_t addr,QString labelString)
{
    qDebug("setting memLabel");
    // allocate new label
    label_t* label = new label_t();
    label->addr = addr;
    label->name = labelString;

    MASK
            // free old label
            label_t* oldLabel = getMemLabel(addr);


    // store new label
    TRY2PUSH(addr,oldLabel,changeMemLabel(addr,oldLabel,label));
    UNMASK
            qDebug("all good here");
    IFNOMASK(emit update();)
}

label_t* Computer::getMemLabel(mem_addr_t addr)
{
    return _memory[addr].label;
}

void Computer::setMemBreakPoint(mem_addr_t addr,breakpoint_t* breakpt){
    breakpoint_t* obreakptr =_memory[addr].breakpt;
    _memory[addr].breakpt = breakpt;
    TRY2PUSH(obreakptr,breakpt,changeMemBreak(addr,obreakptr,breakpt));

    IFNOMASK(emit update();)
}

breakpoint_t* Computer::getMemBreakPoint(mem_addr_t addr)
{
    return _memory[addr].breakpt;
}

void Computer::setMemComment(mem_addr_t addr, QString comment)
{
    QString oldComment = _memory[addr].comment;
    bool b = oldComment.isNull();
    _memory[addr].comment = comment;

    TRY2PUSH(oldComment,comment,changeMemComment(addr,oldComment,comment));

    IFNOMASK(emit update();)
}

QString Computer::getMemComment(mem_addr_t addr)
{
    //return "To be figured out. Issues with value vs. reference";
    return _memory[addr].comment;
}

void Computer::setMemLoc(mem_addr_t addr, mem_loc_t loc_val)
{
    //    qDebug("I am going to try to set memory");
    if(_memory[addr]!= loc_val)
    {
        qDebug("settin' loc");


        mem_loc_t displaced = _memory[addr];
        _memory[addr] = loc_val;

        //    printf("%x\n%x\n",&displaced,&loc_val);
        _memory[addr].addr = addr;


        TRY2PUSH(displaced,loc_val,changeMemLoc(displaced,loc_val));

        IFNOMASK(emit update();)
    }
    else
    {
                qDebug("Guess not");
    }
}

void Computer::setMemLocBlock(mem_addr_t addr, mem_loc_t *loc_val, val_t blockLen)
{
    Undos->beginMacro("Setting Block");
    MASK
            for(mem_addr_t i = 0;i<blockLen;i++)
    {
        setMemLoc(addr+i,loc_val[i]);
    }
    UNMASK
            Undos->endMacro();
}
void Computer::moveRow(mem_addr_t origin, mem_addr_t destination)
{
    setMemLoc(destination,_memory[origin]);
}

void Computer::shiftMemory(mem_addr_t origin, val_t length,mem_addr_t destination )
{

}
// loading

size_t Computer::loadProgramFile(char* path) {


    FILE *file = fopen(path, "r");

    if (!file)
        throw "ERROR: could not open file";

    fseek(file, 0, SEEK_END);
    size_t fileLen = ftell(file);
    size_t programSize = fileLen/2 - 1;

    if (fileLen == 0) {
        throw "ERROR: input file empty!";
    }

    rewind(file);

    mem_addr_t startingAddr;
    val_t *buffer = (val_t *)malloc(programSize*sizeof(val_t));

    fread(&startingAddr, sizeof(mem_addr_t), 1, file);
    flipBytes(&startingAddr);
    fread(buffer, sizeof(val_t), programSize, file);
    for (int i = 0; i < programSize; i ++) {
        flipBytes(&buffer[i]);
    }

    setMemValuesBlock(startingAddr, programSize, buffer);

    free(buffer);

    fclose(file);

    return programSize;
}

// execution

reg_t getRegister_9_10_11(val_t inst) {
    val_t mask = 0x0E00;

    val_t num = (inst & mask) >> 9;
    return (reg_t)num;
}

reg_t getRegister_6_7_8(val_t inst) {
    val_t mask = 0x01C0;

    val_t num = (inst & mask) >> 6;
    return (reg_t)num;
}

reg_t getRegister_0_1_2(val_t inst) {
    val_t mask = 0x0007;

    val_t num = (inst & mask) >> 0;
    return (reg_t)num;
}

mem_addr_t getOffset11(val_t inst) {
    return inst & 0x07FF;
}

mem_addr_t getOffset9(val_t inst) {
    return inst & 0x01FF;
}

mem_addr_t getOffset6(val_t inst) {
    return inst & 0x003F;
}

mem_addr_t getTrap8(val_t inst) {
    return inst & 0x00FF;
}

val_t getImm5(val_t inst) {
    return inst & 0x001F;
}

void Computer::add(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t sr1 = getRegister_6_7_8(inst);
    val_t a = getRegister(sr1);
    val_t b;

    bool sub = false;

    if (bitMask(5) & inst) {
        // add imm
        b = getImm5(inst);
        // sign extend
        if (bitMask(4) & b) {
            b |= 0xFFE0;
        }

    } else {
        if ( (bitMask(3)) & inst ) {
            // bit 3 should be 0
            throw("Invalid op (read as ADD/SUB): " + getHexString(inst) + "bit 3 should be 0");
        }

        reg_t sr2 = getRegister_0_1_2(inst);
        b = getRegister(sr2);

        // if bit 4 is set and we are using registers we subtract
        if (bitMask(4) & inst) {
            sub = true;
        }
    }
    val_t r;

    if (sub)
    {
        // cannot do straight add because we are using unsigned types
        r = a + (~b+1);
    }
    else
    {
        r = a+b;
    }

    setRegister(dr, r);
    if (r & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::and_op(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t sr1 = getRegister_6_7_8(inst);
    val_t a = getRegister(sr1);
    val_t b;


    if (bitMask(5) & inst) {
        // add imm
        b = getImm5(inst);
        // sign extend
        if (bitMask(4) & b) {
            b |= 0xFFE0;
        }
    } else {
        if ( (bitMask(4) | bitMask(3)) & inst ) {
            // bits 3 and 4 should be 0
            // TODO invaild op error
            return;
        }
        reg_t sr2 = getRegister_0_1_2(inst);
        b = getRegister(sr2);
    }

    val_t r = a & b;
    setRegister(dr, r);
    if (r & 0x8000) {
        setProgramStatus(cond_n);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::mul(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t sr1 = getRegister_6_7_8(inst);
    val_t a = getRegister(sr1);
    val_t b;


    if (bitMask(5) & inst) {
        // add imm
        b = getImm5(inst);
        // sign extend
        if (bitMask(4) & b) {
            b |= 0xFFE0;
        }
    } else {
        if ( (bitMask(4) | bitMask(3)) & inst ) {
            // bits 3 and 4 should be 0
            // TODO invaild op error
            return;
        }
        reg_t sr2 = getRegister_0_1_2(inst);
        b = getRegister(sr2);
    }

    // we cannot do straigt mul since we have unsigned values
    // TODO make this better
    // I don't know how c++ casts exactly work so her a quick hack
    bool resultPositive = true;
    if (a & bitMask(15)) {
        resultPositive ^= true;
        a = (~a + 1);
    }
    if (b & bitMask(15)) {
        resultPositive ^= true;
        b = (~b + 1);
    }
    val_t r = a * b;
    if (!resultPositive) {
        r = (~r + 1);
    }

    setRegister(dr, r);
    if (r & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::not_op(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t sr = getRegister_6_7_8(inst);
    val_t a = getRegister(sr);

    if ( getOffset6(inst) != 0x3F ) {
        // bits 0 to 5 should be 1
        // TODO invaild op error
        return;
    }

    val_t r = ~a;
    setRegister(dr, r);

    if (r & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::br(val_t inst) {

    if (bitMask(11) & inst) {
        // n
        if (getProgramStatus() == cond_n) {
            executeBr(inst);
            return;
        }
    }
    if (bitMask(10) & inst) {
        // z
        if (getProgramStatus() == cond_z) {
            executeBr(inst);
            return;
        }
    }
    if (bitMask(9) & inst) {
        // p
        if (getProgramStatus() == cond_p) {
            executeBr(inst);
            return;
        }
    }
    // no op
}

void Computer::executeBr(val_t inst) {
    // execute branch
    val_t offset = getOffset9(inst);
    mem_addr_t pc = getRegister(PC);
    if (offset & bitMask(8)) {
        // sign extend
        offset |= 0xFE00;
    }
    pc += offset;
    setRegister(PC,pc);
}

void Computer::executeCycle()
{
    mem_addr_t pcAddr = getRegister(PC);
    mem_loc_t instLoc = getMemLocation(pcAddr);
    val_t inst = instLoc.value;
    Undos->beginMacro("Executing "+getHexString(pcAddr));
    setRegister(PC, pcAddr + 1);
    switch (inst & opMask) {
    case addOpCode:
        add(inst);
        break;
    case  andOpCode:
        and_op(inst);
        break;
    case mulOpCode:
        mul(inst);
        break;
    case notOpCode:
        not_op(inst);
        break;
    case brOpCode:
        br(inst);
        break;
    case jmpOpCode:
        jmp(inst);
        break;
    case jsrOpCode:
        jsr(inst);
        break;
    case ldOpCode:
        ld(inst);
        break;
    case ldiOpCode:
        ldi(inst);
        break;
    case ldrOpCode:
        ldr(inst);
        break;
    case leaOpCode:
        lea(inst);
        break;
        //case retOpCode:
        //ret(inst);
        //break;
        // done as a jump
    case rtiOpCode:
        rti(inst);
        break;
    case stOpCode:
        st(inst);
        break;
    case stiOpCode:
        sti(inst);
        break;
    case strOpCode:
        str(inst);
        break;
    case trapOpCode:
        trap(inst);
        break;
    default:
        break;
    }
    Undos->endMacro();
}

void Computer::jmp(val_t inst) {

    if (inst & bitMask(0)) {
        // RTT exit priviliged mode
        setPriviliged(false);
    }

    if (inst & 0x0E3E) {
        // bits 11-9 and 5-1 should be 0
        throw "INVALID OP: bits 11-9 & 5-1 should be 0 in JMP/RET/RTT";
    }

    reg_t baseReg = getRegister_6_7_8(inst);
    mem_addr_t jmpAddr = getRegister(baseReg);

    setRegister(PC, jmpAddr);
}

void Computer::jsr(val_t inst) {

    mem_addr_t jsrAddr;
    mem_addr_t tmpPc = getRegister(PC);

    if (bitMask(11) & inst) {
        // offset mode
        val_t offset = getSignedOffset11(inst);
        jsrAddr += offset;

    } else {
        // link mode
        if (inst & 0x063F) {
            // bits 10,9 and 5-0 should be 0
            // TODO invalid op error
            return;
        }

        reg_t baseR = getRegister_6_7_8(inst);
        jsrAddr = getRegister(baseR);
    }

    setRegister(PC, jsrAddr);
    setRegister(R7, tmpPc);
}

void Computer::ld(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    val_t offset = getOffset9(inst);


    mem_addr_t addr = getRegister(PC) + offset;
    checkMemAccess(addr);
    val_t memVal = getMemValue(addr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }

    checkSpecialAddressRead(addr);
}

void Computer::ldi(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    val_t offset = getSignedOffset9(inst);

    mem_addr_t addr = getRegister(PC) + offset;
    checkMemAccess(addr);
    mem_addr_t innerAddr = getMemValue(addr);
    checkMemAccess(addr);
    val_t memVal = getMemValue(innerAddr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }

    checkSpecialAddressRead(innerAddr);
}

void Computer::ldr(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t baseR = getRegister_6_7_8(inst);
    mem_addr_t baseAddr = getRegister(baseR);

    val_t offset = getSignedOffset6(inst);


    mem_addr_t addr = baseAddr + offset;
    checkMemAccess(addr);
    val_t memVal = getMemValue(addr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }

    checkSpecialAddressRead(addr);
}

void Computer::lea(val_t inst) {

    reg_t dr = getRegister_9_10_11(inst);
    val_t offset = getSignedOffset9(inst);

    val_t val = getRegister(PC) + offset;

    setRegister(dr, val);

    if (val & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (val == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::rti(val_t inst) {
    if (getRegister(PSR) & bitMask(15)) {
        throw "Privilege mode exception: RTI";
    } else {
        // PC=mem[R6]; R6 is the SSP
        mem_addr_t r6 = getRegister(R6);
        setRegister(PC, getMemValue(r6));
        // R6 = R6+1;
        r6 += 1;
        setRegister(R6, r6);
        // TEMP = mem[R6];
        val_t tmp = getMemValue(r6);
        // R6 = R6+1;
        r6 += 1;
        setRegister(R6, r6);
        // PSR=TEMP; the privilege mode and condition codes of the interrupted process are restored
        setRegister(PSR, tmp);
    }
}

void Computer::st(val_t inst) {
    reg_t sr = getRegister_9_10_11(inst);
    val_t offset = getSignedOffset9(inst);
    mem_addr_t addr = getRegister(PC) + offset;
    checkMemAccess(addr);
    setMemValue(addr, getRegister(sr));

    checkSpecialAddressWrite(addr);
}

void Computer::sti(val_t inst) {
    val_t offset = getSignedOffset9(inst);
    mem_addr_t pc = getRegister(PC);
    mem_addr_t innerAddr = pc + offset;
    checkMemAccess(innerAddr);
    mem_addr_t addr = getMemValue(innerAddr);
    checkMemAccess(addr);
    val_t memVal = getRegister(getRegister_9_10_11(inst));
    setMemValue(addr, memVal);

    checkSpecialAddressWrite(addr);
}

void Computer::str(val_t inst) {
    val_t offset = getSignedOffset6(inst);
    val_t srVal = getRegister(getRegister_9_10_11(inst));
    mem_addr_t baseR = getRegister(getRegister_6_7_8(inst));

    mem_addr_t addr = baseR + offset;
    checkMemAccess(addr);
    setMemValue(addr, srVal);

    checkSpecialAddressWrite(addr);
}

void Computer::trap(val_t inst) {
    if (inst & 0x0F00) {
        // bits 11-8 should be 0
        throw "INVALID OP ERROR: bits 11-8 should be 0 in TRAP";
        return;
    }

    setRegister(R7, getRegister(PC));
    setPriviliged(true);

    val_t trapVect = getTrap8(inst);
    setRegister(PC, getMemValue(trapVect));
}

val_t Computer::getSignedOffset11(val_t inst)
{
    val_t offset = getOffset11(inst);
    if (offset & bitMask(10)) {
        // sign extend
        offset |= 0xF400;
    }
    return offset;
}
val_t Computer::getSignedOffset9(val_t inst)
{
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        offset |= 0xFE00; // make bits after 9 1's
    }
    return offset;
}
val_t Computer::getSignedOffset6(val_t inst)
{
    val_t offset = getOffset6(inst);
    if (offset & bitMask(5)) {
        // sign extend
        offset |= 0xFFC0;
    }
    return offset;
}


void Computer::checkMemAccess(mem_addr_t addr)
{
    // check against the MPR
    if (!getPriviliged()) {
        val_t sector = addr & 0xF000; // select first 4 bits
        sector >>= 12; // move the bits so they become a number
        val_t sectorMap = 1 << sector;
        //        if ((sectorMap & _memory[MPR].value) == 0) {
        //            throw 'Privilege Mode Exception: Trying to address blocked memory';
        //        }
    }
}

void Computer::checkSpecialAddressRead(mem_addr_t addr)
{
    switch (addr) {
    case KBDR:
        setMemValue(KBSR,0x0000);
        break;
    default:
        break;
    }
}

void Computer::checkSpecialAddressWrite(mem_addr_t addr)
{
    switch (addr) {
    case DSR:
        if (getMemValue(DSR) == 0x8000) {
            IFNOMASK(hasCharacterToDisplay();)
        }
        break;
    default:
        break;
    }
}



void Computer::executeSingleInstruction() {

    MASK

            setRunning(true);

    executeCycle();

    setRunning(false);

    UNMASK

            IFNOMASK(update();)
            Undos->endMacro();

}

void Computer::startExecution()
{
    setRunning(true);

    while (isRunning()) {
        executeCycle();
    }

}

void Computer::executeUntilAddress(mem_addr_t addr)
{
    Undos->beginMacro("Executing from " + getHexString(getRegister(PC)) + " until "+getHexString(addr));
    setRunning(true);

    while (isRunning() && getRegister(PC) != addr) {
        executeCycle();
    }

    setRunning(false);
    Undos->endMacro();

}

bool Computer::setKeyboardCharacter(char c, bool force)
{

    val_t sr = getMemValue(KBSR);
    bool needsForce = sr == 0x8000;

    if (!needsForce || force) {
        Undos->beginMacro(QString(&c) + " key pressed");
        setMemValue(KBDR,c);
        Undos->endMacro();
    }
    return !needsForce;
}

char Computer::getKeyboardCharacter()
{
    val_t val = getMemValue(KBDR);
    setMemValue(KBSR,0x8000);
    //    val >>= 8;
    return (char)val;
}

char Computer::getDisplayCharacter()
{
    makeDisplayReady();
    val_t val = getMemValue(DDR);
    //    val >>= 8;
    return (char)val;
}

void Computer::makeDisplayReady()
{
    setMemValue(DSR,0x8000);
}


bool Computer::insertBlankRow(mem_addr_t addr)
{
    mem_addr_t queue = findSpace(addr,10);
    for(int i = queue; i >=addr;i--)
    {

    }
}
bool Computer::canShiftClean(mem_addr_t originStart, mem_addr_t originEnd,mem_addr_t destination)
{
    int32_t offset = destination-originStart;//this needs to be big because a int16_t can't contain
    //all the possible values which it could be asked to hold.
    mem_addr_t startSearch = (originStart<=1024)?0:(originStart-1024);
    mem_addr_t endSearch = (originEnd>=MEMSIZE-1023)?MEMSIZE:(originEnd+1023);
    for(mem_addr_t index = startSearch;index<=endSearch;index++)
    {

        mem_addr_t connected = connectedAddress(_memory[index]);

        if(connected!=index)//we don't care about normal lines
        {
            mem_addr_t futureCurrent = proposedNewLocation(index,originStart,originEnd,offset);
            mem_addr_t futureTarget  = proposedNewLocation(connected,originStart,originEnd,offset);
            mem_loc_t  temp ;
            temp = (_memory[index]);
            temp.addr = futureCurrent;
            if(!canConnect(temp,futureTarget))
                return false;
        }

    }
    return true;

}


bool Computer::preventShiftProblems(mem_addr_t original, mem_addr_t destination,val_t lengthGroupMoved,bool force)
{
    //It is understandable that people will want the ablility to shift their
    //code around.  This presents a problem, once labels are allowed, as
    //they don't do well when you move one end and not the other.

    mem_addr_t startSearch = (original<=1024)?0:(original-1024);
    mem_addr_t endSearch = (original>=MEMSIZE-1023)?MEMSIZE:(original+1023);




    return startSearch==destination==lengthGroupMoved==endSearch;
    /*
     * We don't need to seach outside what is visible to the original section
     * right now, we are only keeping track of the beginning.
     */

    //now, we search for anything that cares about offset.

    /* the simplest case arises when either of two things is true:
         *
         * Case 1: The line doesn't care about where anything else is.
         *
         * Case 2: The line does care about where another line is, but that
         * line is within the group being moved.
         *
         *
         * For these cases, we simply pass them by
         */

    //It turns out that it only matters when the connections go from in/out
    //side of the group to the out/inside of the
    //        if(connectedToRange())





return true;
}

bool Computer::stillInRange(mem_addr_t current, int32_t delta, mem_addr_t beginRange,mem_addr_t endRange)
{

return true;
}
bool Computer::betweenShifts(mem_addr_t addr,int32_t delta, mem_addr_t begin, mem_addr_t end)
{
    //if you are after end but before begin + delta
    //if you are after end + delta but before begin
    /*
     *
     *
     *
     *
     *
     *
     *
     *
     *
     */

    val_t  boundsMin = end + ((delta>0)?1:delta+1);
    val_t boundsMax = begin + ((delta>0)?delta-1:-1);
    return isBetween(boundsMin,boundsMax,addr);
}

mem_addr_t Computer::proposedNewLocation(mem_addr_t addr,mem_addr_t begin, mem_addr_t end, int32_t delta,QString* code)
{

    if(delta == 0) return addr;//if you aren't going anywhere, don't waste time



    if(isBetween(begin,end,addr))//if you are in the moved group, your location is just
        //the delta away
    {
        code->append("selected");
        return addr + delta;
    }
    if(delta<0)
    {
        if(addr>end)
        {
            code->append("After");
            return addr;
        }
    }
    if(delta>0)
    {
        if(addr<begin)
        {

            code->append("Before");

            return addr;
        }
    }
    if(isBetween(begin,end,addr-delta))
    {

        code->append("destination");
        return addr - ((delta>0)?1:-1)*((end - begin+1));//+((delta<0)?1:-1)*((end - begin));
    }
    if(betweenShifts(addr,delta,begin,end))
    {

        code->append("between");
//        qDebug(QString().setNum(addr-delta).toLocal8Bit());

        return addr - ((delta>0)?1:-1)*((end - begin)+1);
    }
    code->append("unaccounted");
    return addr;
}

mem_loc_t Computer::createShiftedLoc(mem_loc_t original,mem_addr_t newAddress, mem_addr_t newTarget, bool* ok)
{
    mem_loc_t out;
    memcpy(&out,&original,sizeof(original));
    out.addr = newAddress;




}
void *Computer::slideMemory(mem_addr_t begin, mem_addr_t end, int32_t delta, bool* ok)
{
    qDebug("Sliding memeory");
    //    if(canShiftClean(begin,end,begin+delta))
    if(true)
    {
        qDebug("suc");
        Undos->beginMacro("Shifting addresses " + getHexString(begin) +"-"+ getHexString(end) +" to "+getHexString(begin+delta));
        mem_addr_t startSearch = (begin<=1024)?0:(begin-1024);
        mem_addr_t endSearch = (end>=MEMSIZE-1023)?MEMSIZE:(end+1023);
        int count = 0;
        int* changed = (int*)calloc(endSearch-startSearch+1,sizeof(int));
        int** changeTracker = &changed;
        for(mem_addr_t index = startSearch;index<=endSearch;index++)
        {
            juggleShift(index,begin,end,delta,changed,startSearch);
            //            mem_loc_t holder =
            //            qDebug(QString().setNum(index).toLocal8Bit()+" " +QString().setNum(count++).toLocal8Bit());
        }
        Undos->endMacro();
    }
    else
    {
        qDebug("coulnd'");
    }
}
void Computer::juggleShift(mem_addr_t current, mem_addr_t begin, mem_addr_t end, int32_t delta, int* changed, int offset)
{
    if(changed[current-offset])
    {
        qDebug("time saved");
        return;
    }else
    {
        //        qDebug("new toy");
    }
    mem_loc_t curLoc = _memory[current];
    mem_addr_t curConnect = connectedAddress(curLoc);
    mem_addr_t proposedNext = proposedNewLocation(current,begin,end,delta);
    mem_addr_t proposedConnect = proposedNewLocation(curConnect,begin,end,delta);
    if((proposedNext==current) && (proposedConnect==curConnect))
    {

        changed[current-offset]=1;
//        std::cout<<"alternate ending"<<std::endl;
        return;
    }

    qDebug("Beinging Juggle");
    bool b=0;
    while(changed[curLoc.addr-offset]!=1)
    {
        changed[curLoc.addr-offset]=1;
        mem_addr_t nextAddr = proposedNewLocation(curLoc.addr,begin,end,delta);
        mem_addr_t target = proposedNewLocation(connectedAddress(curLoc),begin,end,delta);
        mem_loc_t nextLoc;
        memcpy(&nextLoc,&_memory[nextAddr],sizeof(mem_loc_t));
        val_t value= generateOffset(curLoc,target,&b);
//        val_t value = curLoc.value;
        qDebug(QString(QString().setNum(curLoc.addr)+"->"+QString().setNum(nextLoc.addr)+"|"+getHexString(value)).toLocal8Bit());
        setMemLoc(nextAddr, curLoc);
        setMemValue(nextAddr,value);
        //        changed[curLoc.addr-offset] = 1;
        curLoc = nextLoc;
    }
    qDebug("Ending Juggle");
}
//bool Computer::doesShiftAffect(mem_addr_t source, mem_addr_t begin, mem_addr_t end, int32_t delta)
//{
//    mem_addr_t connected= connectedAddress(source);
//    mem_addr_t future   = proposedNewLocation(source,begin,end,delta);
//    mem_addr_t connectedfuture = proposedNewLocation()
//    if(source == connected)
//    {
//        if(source == future)
//            return false;
//    }
//    if(source==proposedNewLocation(source) && connectedAddress())

//}

bool Computer::isBetween(val_t min, val_t max, val_t val )
{
    bool b = (val >= min)&&(val<= max);
    //    std::cout<<b<<std::endl;
    return b;
}
mem_addr_t Computer::connectedAddress(mem_addr_t addr)
{
    return connectedAddress(_memory[addr]);
}
mem_addr_t Computer::connectedAddress(mem_loc_t mem)
{

    int offset = getPCOffsetNumber(mem);
    val_t inst = mem.value;
    int addrOffset = 0;
    switch (offset) {
    case 11:
        addrOffset = getSignedOffset11(inst);
        break;
    case 9:
        addrOffset = getSignedOffset9(inst);
        break;
    default:

        break;
    }
    return mem.addr + addrOffset;

}

int Computer::getPCOffsetNumber(mem_loc_t mem)
{

    val_t inst = mem.value;
    switch (inst & opMask) {
    //Offset9
    case brOpCode:
    case ldOpCode:
    case ldiOpCode:
    case leaOpCode:
    case stOpCode:
    case stiOpCode:

        return 9;
        //Offset11
    case jsrOpCode:
        if(bitMask(11) & inst)//We have to differentiate between Jsr and jsrr
        {
            return 11;
        }
        return 0;
    default:
        return 0;//not connected
    }
}
bool Computer::canConnect(mem_loc_t from, mem_addr_t to)
{
    int power = getPCOffsetNumber(from);
    return (isBetween(-1*2^(power-1),2^(power-1)-1,from.addr-to));
}
val_t Computer::targetOffset(mem_loc_t mem,mem_addr_t target)
{

    int pcOffsetNumber = getPCOffsetNumber(mem);

}

val_t Computer::generateOffset(mem_loc_t mem, mem_addr_t target, bool* ok)
{
    *ok = true;
    int power =getPCOffsetNumber(mem);
//    if(!canConnect(mem,target))
//    {
//        *ok = false;
//        return 0;
//    }
    val_t difference = target-mem.addr;
    val_t mask = 0xFFFF>>(16-power);
    val_t maskedOffset = mask & difference;
    val_t nonOffsetProtector = mask<<power;
    val_t cleaned = mem.value & nonOffsetProtector;
    val_t offseted = cleaned | maskedOffset;

    return offseted;
}

mem_addr_t Computer::findSpace(mem_addr_t startSearch,int minimumSize)
{
    return startSearch+100;
    int streak = 0;
    while(streak <minimumSize)
    {
        
    }
}
QString Computer::getMemNameSafe(mem_addr_t addr)
{
    return getMemNameSafe(_memory[addr]);
}
QString Computer::getMemNameSafe(mem_loc_t loc)
{
    label_t* l = loc.label;
    if(l)
    {
        return l->name;
    }
    return QString();
}


