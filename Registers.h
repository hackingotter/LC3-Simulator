//
//  Registers.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 2/27/17.
//  Copyright © 2017 EIU. All rights reserved.
//

/** @file */

#ifndef Registers_h
#define Registers_h

#include "GlobalTypes.h"

#define number_of_regs 11
/** Defines an enum for the condition codes of LC3.
*/
typedef enum cond_t {
    cond_n = -1, /** negative */
    cond_z = 0, /** zero */
    cond_p = 1, /** positive */
    cond_none, /** all three status bits are set to 0 */
    cond_invalid /** conflict of status bits set */
} cond_t;

/** An enum for the Registers.
 * Details...
 *
 *
 */
typedef enum reg_t {
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    R6 = 6,
    R7 = 7,
    SPR = 7,
    PC = 8,
    MPR = 9,//Memory Protection Register
    PSR = 10
} reg_t;




/** Returns the contents of a register
 * \param reg The register whose value is requested.
 * \return The contens of register reg.
 */
val_t getRegister(reg_t reg);

/** Returns the contents of all registers
 * \return An array of length 10 containg copies of the contents of all registers. Not null terminated.
 */
val_t* getAllRegisters();

/** Sets the value of a register
 * \param reg the register to set
 * \param val the value that reg will be set to
 */
void setRegister(reg_t reg, val_t val);

/** Sets the value of a register
 * \param vals An array of length 10 containg the values of all registers. Does not have to be null terminated. Order is R0-R6,SPR,PC,PSR
 */
void setAllRegister(val_t vals[]);

/** Gets the PSR condition code
 * \return the condition of the PSR.
 */
cond_t getProgramStatus();

/** Gets the PSR condition code
 * \param stat the new condition. The PSR's condition bits will be set accordingly.
 */
void setProgramStatus(cond_t stat);

#endif /* Registers_h */
