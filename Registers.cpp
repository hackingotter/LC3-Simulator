//
//  Registers.c
//  LC3-Sim-Core
//
//  Created by Lars Ott on 4/15/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "Registers.h"
#include "GlobalTypes.h"

val_t registers[11];

val_t getRegister(reg_t reg) {
    return registers[reg];
}

val_t* getAllRegisters() {
    val_t *ret = (val_t*)malloc(number_of_regs * sizeof(val_t));

    if (!ret) {
        throw "Malloc error";
        return ret;
    }

    memcpy(ret,registers,number_of_regs * sizeof(val_t));
    return ret;
}

void setRegister(reg_t reg, val_t val) {
    registers[reg] = val;
}

void setAllRegister(val_t vals[]) {
    if (!vals) {
        throw "Null pointer error";
    } else {
        memcpy(registers, vals, number_of_regs * sizeof(val_t));
    }
}


// N is 2 bit
// Z is 1 bit
// P is 0 bit
cond_t getProgramStatus() {

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

void setProgramStatus(cond_t stat) {
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

    setRegister(PSR, curr);
}
