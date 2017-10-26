#include "computer.h"
#include "opcodes.h"
#include "Util.h"
#include <QString>
#include <QUndoCommand>


#define TRY2PUSH(OLD,NEW,DO) {if(NEW!=OLD)Computer::Undos->add(new Action::DO);}
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

class changeRegCondt: public QUndoCommand
{
public:
    changeRegCondt(cond_t ncond,cond_t ocond):newCondt(ncond),oldCondt(ocond)
    {
        setText(QString("Set Condition to "+QString().setNum(ncond)));
        QUndoCommand::setObsolete(newCondt == oldCondt);
    }
    void undo()
    {
        Computer::getDefault()->setProgramStatus(oldCondt);
    }

    void redo()
    {
        Computer::getDefault()->setProgramStatus(newCondt);
    }
private:
    cond_t newCondt;
    cond_t oldCondt;

};

class changeRegValue: public QUndoCommand
{
public:
    changeRegValue(reg_t reg,val_t oval, val_t nval):regName(reg),newValue(nval),oldValue(oval)
    {
        setText(    QString("Set "+ ((regName<8)?"R"+QString().setNum(regName):" other") + " to "+QString().setNum(newValue)));
        QUndoCommand::setObsolete(newValue==oldValue);
    }
    void undo()
    {
        Computer::getDefault()->setRegister(regName,oldValue);
    }
    void redo()
    {
        Computer::getDefault()->setRegister(regName,newValue);
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
        setText(
                    QString("Set " + getHexString(addr) + " to "+QString().setNum(newValue)));
        QUndoCommand::setObsolete(newValue==oldValue);
    }
    void undo()
    {

        Computer::getDefault()->setMemValue(mem_addr,oldValue);
    }
    void redo()
    {
        Computer::getDefault()->setMemValue(mem_addr,newValue);

    }

    ~changeMemValue() {;}
private:
    mem_addr_t mem_addr;
    val_t oldValue;
    val_t newValue;
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
        Computer::getDefault()->setMemLabel(mem_addr,oldLabelPtr);
    }
    void redo()
    {
        Computer::getDefault()->setMemLabel(mem_addr,newLabelPtr);
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
        Computer::getDefault()->setMemBreakPoint(mem_addr,oldBreak);
    }
    void redo()
    {
        Computer::getDefault()->setMemBreakPoint(mem_addr,newBreak);
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
        setText("Changed Comment at " + getHexString(addr)+ "to " + newComment);
        QUndoCommand::setObsolete(newComment==oldComment);
    }
    void undo()
    {
        Computer::getDefault()->setMemComment(mem_addr,oldComment);
    }
    void redo()
    {
        Computer::getDefault()->setMemComment(mem_addr,newComment);
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
    Undos->add(new Action::changeRegValue(reg,oval,val));

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
    if (priv)
        oldPSR |= 0x8000; // this will force bit 15 to 1 but maintain all others
    else
        oldPSR &= 0x7FFF; // this will force bit 15 to 0 but maintain all others

    setRegister(PSR,oldPSR);
}

bool Computer::getPriviliged()
{
    return getRegister(PSR) & 0x8000; // bit 15 is privilige bit
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



void Computer::setMemValue(mem_addr_t addr, val_t val)
{
    val_t oval = _memory[addr].value;
    _memory[addr].value = val;
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
    if (oldLabel)
        free(oldLabel);

    // store new label
    setMemLabel(addr,label);
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

    Computer::Undos->add(new Action::changeMemComment(addr,oldComment,comment));

    IFNOMASK(emit update();)
}

QString Computer::getMemComment(mem_addr_t addr)
{
    //return "To be figured out. Issues with value vs. reference";
    return _memory[addr].comment;
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
        val_t offset = getOffset11(inst);
        jsrAddr = getRegister(PC);
        if (offset & bitMask(10)) {
            // sign extend
            offset |= 0xF400;
        }
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
    if (offset & bitMask(8)) {
        // sign extend
        offset |= 0xFE00;
    }

    mem_addr_t addr = getRegister(PC) + offset;
    val_t memVal = getMemValue(addr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::ldi(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        // sign extend
        offset |= 0xFE00;
    }

    mem_addr_t addr = getRegister(PC) + offset;
    mem_addr_t innerAddr = getMemValue(addr);
    val_t memVal = getMemValue(innerAddr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::ldr(val_t inst) {
    reg_t dr = getRegister_9_10_11(inst);
    reg_t baseR = getRegister_6_7_8(inst);
    mem_addr_t baseAddr = getRegister(baseR);

    val_t offset = getOffset6(inst);
    if (offset & bitMask(5)) {
        // sign extend
        offset |= 0xFFC0;
    }

    mem_addr_t addr = baseAddr + offset;
    val_t memVal = getMemValue(addr);

    setRegister(dr, memVal);

    if (memVal & bitMask(15)) {
        setProgramStatus(cond_n);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_p);
    }
}

void Computer::lea(val_t inst) {

    reg_t dr = getRegister_9_10_11(inst);
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        // sign extend
        offset |= 0xFE00;
    }

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
        // TODO privilege mode exception

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
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        offset |= 0xFE00;
    }

    mem_addr_t addr = getRegister(PC) + offset;

    setMemValue(addr, getRegister(sr));
}

void Computer::sti(val_t inst) {
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        offset |= 0xFE00; // make bits after 9 1's
    }
    mem_addr_t pc = getRegister(PC);
    mem_addr_t innerAddr = pc + offset;
    mem_addr_t addr = getMemValue(innerAddr);
    val_t memVal = getRegister(getRegister_9_10_11(inst));
    setMemValue(addr, memVal);
}

void Computer::str(val_t inst) {
    val_t offset = getOffset6(inst);
    if (offset & bitMask(5)) {
        offset |= 0xFFC0; // make bits after 6 1's
    }
    val_t srVal = getRegister(getRegister_9_10_11(inst));
    mem_addr_t baseR = getRegister(getRegister_6_7_8(inst));

    mem_addr_t addr = baseR + offset;
    setMemValue(addr, srVal);
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
    setRunning(true);

    while (isRunning() && getRegister(PC) != addr) {
        executeCycle();
    }

    setRunning(false);
}







