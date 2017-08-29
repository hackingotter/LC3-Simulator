#include "computer.h"
#include "opcodes.h"


Computer::Computer(QObject *parent) : QObject(parent)
{

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

void Computer::setUndoStack(QUndoStack* stack)
{
    Computer::Undos = stack;
}

// registers

val_t Computer::getRegister(reg_t reg) {
    return registers[reg];
}

val_t* Computer::getAllRegisters() {
    val_t *ret = (val_t*)malloc(NUM_OF_REGS * sizeof(val_t));

    if (!ret) {
        throw "Malloc error";
        return ret;
    }

    memcpy(ret,registers,NUM_OF_REGS * sizeof(val_t));
    return ret;
}

void Computer::setRegister(reg_t reg, val_t val) {
    //will implement an identification method
    Undos->push(new Action::changeRegValue(reg,val));
    registers[reg] = val;
}

void Computer::setAllRegister(val_t vals[]) {
    if (!vals) {
        throw "Null pointer error";
    } else {
        memcpy(registers, vals, NUM_OF_REGS * sizeof(val_t));
    }
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
    if(!Action::doing)Undos->push(new Action::changeRegCondt(stat));
    this->setRegister(PSR, curr);
}

// memory

mem_loc_t Computer::getMemLocation(mem_addr_t addr)
{
    return _memory[addr];
}

void Computer::setMemValue(mem_addr_t addr, val_t val)
{
    if(!Action::doing)Undos->push(new Action::changeMemValue(addr,val));
    _memory[addr].value = val;
}

void Computer::setMemValuesBlock(mem_addr_t addr, size_t blockSize, val_t *vals)
{
    //Implemented for compiler's compliance.
    // TODO
}

val_t Computer::getMemValue(mem_addr_t addr)
{
    return _memory[addr].value;
}

val_t* Computer::getAllMemValues()
{
    mem_addr_t addr;
    val_t * ret = (val_t*)malloc(65535 * sizeof(val_t));

    if (!ret)
    {
        throw "Malloc error";
        return ret;
    }
    for(addr =0;addr<0xFFF;addr++)
    {
        ret[addr] = _memory[addr].value;
    }

    return ret;
}

void Computer::setMemLabel(mem_addr_t addr,label_t* newLabel)
{
    if(!Action::doing)Undos->push(new Action::changeMemLabel(addr,newLabel));
    _memory[addr].label=newLabel;
}
void Computer::setMemLabelText(mem_addr_t addr,QString labelString)
{
    label_t* label = (label_t*)malloc(sizeof (label_t));
    label->addr = addr;
    label->name = labelString;
    _memory[addr].label = label;
}

label_t* Computer::getMemLabel(mem_addr_t addr)
{
    return _memory[addr].label;
}

void Computer::setMemBreakPoint(mem_addr_t addr,breakpoint_t* breakpt){
    _memory[addr].breakpt = breakpt;
}

breakpoint_t* Computer::getMemBreakPoint(mem_addr_t addr)
{
    return _memory[addr].breakpt;
}

void Computer::setMemComment(mem_addr_t addr, QString comment)
{
    _memory[addr].comment = comment;
}

QString Computer::getMemComment(mem_addr_t addr)
{
    return _memory[addr].comment;
}

// loading

size_t Computer::loadProgramFile(char* path) {

    FILE *file = fopen(path, "r");

    if (!file)
        return 0;

    fseek(file, 0, SEEK_END);
    size_t fileLen = ftell(file);
    rewind(file);

    mem_addr_t startingAddr;
    val_t *buffer = (val_t *)malloc((fileLen/2 - 1)*sizeof(val_t));

    fread(&startingAddr, sizeof(mem_addr_t), 1, file);
    fread(buffer, sizeof(val_t), fileLen/2 - 1, file);

    setMemValuesBlock(startingAddr, fileLen/2 - 1, buffer);

    fclose(file);

    return fileLen/2 - 1;
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
            // bit 4 should be 0
            // TODO invaild op error
            return;
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
        r = a-b;
    }
    else
    {
        r = a+b;
    }

    setRegister(dr, r);
    if (r > 0) {
        setProgramStatus(cond_p);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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
    if (r > 0) {
        setProgramStatus(cond_p);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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

    val_t r = a * b;
    setRegister(dr, r);
    if (r > 0) {
        setProgramStatus(cond_p);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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

    if (r > 0) {
        setProgramStatus(cond_p);
    } else if (r == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
    }
}

void Computer::br(val_t inst) {

    if (bitMask(11) & inst) {
        // n
        if (getProgramStatus() != cond_n)
            return;
    } else if (bitMask(10) & inst) {
        // z
        if (getProgramStatus() != cond_z)
            return;
    } else if (bitMask(9) & inst) {
        // p
        if (getProgramStatus() != cond_p)
            return;
    }

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

void Computer::jmp(val_t inst) {

    if (inst & 0x0E3F) {
        // bits 11-9 and 5-0 should be 0
        // TODO invalid op error
        return;
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

    if (memVal > 0) {
        setProgramStatus(cond_p);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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

    if (memVal > 0) {
        setProgramStatus(cond_p);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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

    if (memVal > 0) {
        setProgramStatus(cond_p);
    } else if (memVal == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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

    if (val > 0) {
        setProgramStatus(cond_p);
    } else if (val == 0) {
        setProgramStatus(cond_z);
    } else {
        setProgramStatus(cond_n);
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
        // TODO invalid op error
        return;
    }

    setRegister(R7, getRegister(PC));

    val_t trapVect = getTrap8(inst);
    setRegister(PC, getMemValue(trapVect));
}

void Computer::executeSingleInstruction() {

    mem_addr_t pcAddr = getRegister(PC);
    mem_loc_t instLoc = getMemLocation(pcAddr);
    val_t inst = instLoc.value;

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



