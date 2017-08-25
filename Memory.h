//
//  Memory.h
//  LC3-Sim-Core
//
//  Created by Lars Ott on 3/29/17.
//  Copyright © 2017 EIU. All rights reserved.
//

#ifndef Memory_h
#define Memory_h

#include "GlobalTypes.h"
#include "Breakpoints.h"
#include <stdio.h>

/** the type used to pass memory addresses. They are 16 bit.
 */
typedef uint16_t mem_addr_t;

/** type used for labels.
 */
typedef struct {
    char *name; /** the labels name string. */
    mem_addr_t addr; /** the address the label is at. */
} label_t;


/** type used to get memory.
 */
typedef struct {
    mem_addr_t addr; /** the address of the location */
    val_t value; /** the value stored at that location */
    label_t *label; /** a pointer to the label for that position. might be null. */
    breakpoint_t *breakpt; /** a pointer to the breakpoint set at that position. might be null. */
    char * comment; /** a pointer to a possible comment string. might be null. */
} mem_loc_t;

#pragma MARK - get
#pragma MARK mem_loc

/** Gets the mem location struct for the given address.
 * \param addr The address the user wants to access.
 * \return the struct with all the info about the memory location.
 */
mem_loc_t getMemLocation(mem_addr_t addr);

/** Gets a bloch of mem location structs.
 * \param addr the starting address of the block.
 * \param blockSize the size of the block the user wants.
 * \return An array of length blockSize containing the memory_loc_t objects for the requested block. Sorted by address.
 */
mem_loc_t *getMemLocationsBlock(mem_addr_t addr, size_t blockSize);

/** Gets the mem location structs for the whole address space.
 * \return An array containing mem loc structs for the whole address space. sorted by address. HAs 2^16 elements.
 */
mem_loc_t *getAllMemoryLocations();

#pragma MARK val_t

/** Gets the value at the memory location
 * \param addr the address the user wants the value from
 * \return the value inside that register
 */
val_t getMemValue(mem_addr_t addr);

val_t* getAllMemValues();

#pragma MARK label_t*

/** Gets the label on the memory location
 * \param addr the location whose label is being gotten
 * \return a pointer to the label at the location
 */
label_t *getMemLabel(mem_addr_t addr);

#pragma MARK breakpoint_t

/** Determines if the given address has an enabled breakpoint
 * \param addr the location which is being checked for a break point
 * \return the enabledness of the breakpoint at that location
 */
breakpoint_t* getMemBreakPoint(mem_addr_t addr);

#pragma MARK char*

/** Gets the comment attached to that line of code
 * \param addr the location being queried for the comment
 * \return the comment on the correspondent line of code
 */
char* getMemComment(mem_addr_t addr);

#pragma MARK


// TODO should I also do values only?

#pragma MARK - set

/** set a memory location with all the details in of a mem_loc_t struct.
 * \param addr the address of the element to set.
 * \param loc_val the mem_loc_t struct with the data to set.
 */
void setMemLoc(mem_addr_t addr, mem_loc_t loc_val);

/**
 * \param addr the address of the element to set.
 * \param label the label being set.
 */

void setMemLabel(mem_addr_t addr, label_t* label);
void setMemLabelChar(mem_addr_t addr,char* label);
/** sets a given address in memory.
 * \param addr The address to set.
 * \param val the value to set the memory at addr to
 */
void setMemValue(mem_addr_t addr, val_t val);

/** sets a block of memory.
 * \param addr The starting address of the block
 * \param blockSize the size of the block to set
 * \param vals an array containing the values the block should be set to
 */
void setMemValuesBlock(mem_addr_t addr, size_t blockSize, val_t *vals);

/** sets the break pointer for the address
 * \param addr the address whose break point is being set
 * \param breakpt the break pointer being set
 */
void setMemBreakPoint(mem_addr_t addr,breakpoint_t* breakpt);

/** sets the comment attached to that address
 * \param addr the address whose comment is being set
 * \param comment the comment being attached
 */
void setMemComment(mem_addr_t addr,char* comment);

#pragma MARK loading

/** loads a file program file into memory. the files first line will be used as starting address. File must be hex
 * \param path the path to the file to load
 * \return the number of instructions loaded from the file
 */
size_t loadProgramFile(char* path);

 // TODO get special addresses



#endif /* Memory_h */


