//
//  Simulator.c
//  LC3-Sim-Core
//
//  Created by Lars Ott on 6/5/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#include <stdio.h>
#include "GlobalTypes.h"
#include "Memory.h"
#include "Registers.h"
#include "Simulator.h"

#define opMask 0xF000

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


void add(val_t inst) {
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

void and_op(val_t inst) {
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

void mul(val_t inst) {
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

void not_op(val_t inst) {
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

void br(val_t inst) {

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

void jmp(val_t inst) {

    if (inst & 0x0E3F) {
        // bits 11-9 and 5-0 should be 0
        // TODO invalid op error
        return;
    }

    reg_t baseReg = getRegister_6_7_8(inst);
    mem_addr_t jmpAddr = getRegister(baseReg);

    setRegister(PC, jmpAddr);
}

void jsr(val_t inst) {

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

void ld(val_t inst) {
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

void ldi(val_t inst) {
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

void ldr(val_t inst) {
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

void lea(val_t inst) {

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

void rti(val_t inst) {
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

void st(val_t inst) {
    reg_t sr = getRegister_9_10_11(inst);
    val_t offset = getOffset9(inst);
    if (offset & bitMask(8)) {
        offset |= 0xFE00;
    }

    mem_addr_t addr = getRegister(PC) + offset;

    setMemValue(addr, getRegister(sr));
}

void sti(val_t inst) {
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

void str(val_t inst) {
    val_t offset = getOffset6(inst);
    if (offset & bitMask(5)) {
        offset |= 0xFFC0; // make bits after 6 1's
    }
    val_t srVal = getRegister(getRegister_9_10_11(inst));
    mem_addr_t baseR = getRegister(getRegister_6_7_8(inst));

    mem_addr_t addr = baseR + offset;
    setMemValue(addr, srVal);
}

void trap(val_t inst) {
    if (inst & 0x0F00) {
        // bits 11-8 should be 0
        // TODO invalid op error
        return;
    }

    setRegister(R7, getRegister(PC));

    val_t trapVect = getTrap8(inst);
    setRegister(PC, getMemValue(trapVect));
}

void executeSingleInstruction() {

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







