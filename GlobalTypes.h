//
//  GlobalTypes.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 3/29/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#ifndef GlobalTypes_h
#define GlobalTypes_h
#include <inttypes.h>
#include <QString>



#define NUM_OF_REGS 11

/** Registers and memory contain 16 bits. Defined for readability of code.
 */
typedef uint16_t val_t;

/** the type used to pass memory addresses. They are 16 bit.
 */
typedef uint16_t mem_addr_t;


// TODO breakpoint
/**
 * describes a breakpoint
 */
typedef void * breakpoint_t;

// enums

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
    MPR = 9,//Memory Protection Register: actually in memory!!!!
    PSR = 10
} reg_t;


// structs

/** type used for labels.
 */
typedef struct label_t {
    QString name; /** the labels name string. */
    mem_addr_t addr; /** the address the label is at. */
    label_t() : name(QString()), addr(0) {}
} label_t;


/** type used to get memory.
 */
typedef struct mem_loc_t {
    mem_addr_t addr; /** the address of the location */
    val_t value; /** the value stored at that location */
    label_t *label; /** a pointer to the label for that position. might be null. */
    breakpoint_t *breakpt; /** a pointer to the breakpoint set at that position. might be null. */
    QString comment; /** a pointer to a possible comment string. might be null. */
    mem_loc_t() :
        addr(0),
        value(0),
        label(nullptr),
        breakpt(nullptr),
        comment(QString()) {}
} mem_loc_t;


// helpers

#define bitMask(x) (1 << x)

#endif /* GlobalTypes_h */
