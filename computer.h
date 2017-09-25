#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>
#include <QString>
#include "GlobalTypes.h"
#include "HistoryHandler.h"
#include <QUndoStack>

#define MASK\
{\
Computer::getDefault()->updateMask++;\
qDebug("Masking" + QString().setNum(Computer::getDefault()->updateMask).toLocal8Bit());\
}

#define UNMASK {(Computer::getDefault()->updateMask==0)?:Computer::getDefault()->updateMask--;qDebug("Unmasking"+ QString().setNum(Computer::getDefault()->updateMask).toLocal8Bit());}

#define SINGFORME(OPERA) {if(Computer::getDefault()->updateMask==0){OPERA;qDebug("Higher");}}


class Computer : public QObject
{
    Q_OBJECT
public:
    explicit Computer(QObject *parent = 0);

    static Computer* getDefault();

    HistoryHandler* Undos;//will have to work out how to handle this when not in gui

    int updateMask;


   // registers

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

    // status

    /** Gets the PSR condition code
     * \return the condition of the PSR.
     */
    cond_t getProgramStatus();

    /** Gets the PSR condition code
     * \param stat the new condition. The PSR's condition bits will be set accordingly.
     */
    void setProgramStatus(cond_t stat);

    // TODO
    void setPriviliged(bool priv);
    bool getPriviliged();

    // memory
    // #########################

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

    /** Gets the value at the memory location
     * \param addr the address the user wants the value from
     * \return the value inside that register
     */
    val_t getMemValue(mem_addr_t addr);

    val_t* getAllMemValues();

    /** Gets the label on the memory location
     * \param addr the location whose label is being gotten
     * \return a pointer to the label at the location
     */
    label_t *getMemLabel(mem_addr_t addr);

    /** Determines if the given address has an enabled breakpoint
     * \param addr the location which is being checked for a break point
     * \return the enabledness of the breakpoint at that location
     */
    breakpoint_t* getMemBreakPoint(mem_addr_t addr);

    /** Gets the comment attached to that line of code
     * \param addr the location being queried for the comment
     * \return the comment on the correspondent line of code
     */
    QString getMemComment(mem_addr_t addr);

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

    // TODO comment
    void setMemLabelText(mem_addr_t addr,QString labelName);

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


    void fillBlock(mem_addr_t begin, mem_addr_t end, val_t val);
    /** sets the break pointer for the address
     * \param addr the address whose break point is being set
     * \param breakpt the break pointer being set
     */
    void setMemBreakPoint(mem_addr_t addr,breakpoint_t* breakpt);

    /** sets the comment attached to that address
     * \param addr the address whose comment is being set
     * \param comment the comment being attached
     */
    void setMemComment(mem_addr_t addr,QString comment);

    /** loads a file program file into memory. the files first line will be used as starting address. File must be hex
     * \param path the path to the file to load
     * \return the number of instructions loaded from the file
     */
    size_t loadProgramFile(char* path);

     void executeSingleInstruction();
signals:
     void update();

public slots:

private:
    val_t registers[11];
    mem_loc_t _memory[0xFFFF];

    void add(val_t inst);
    void and_op(val_t inst);
    void mul(val_t inst);
    void not_op(val_t inst);
    void br(val_t inst);
    void jmp(val_t inst);
    void jsr(val_t inst);
    void ld(val_t inst);
    void ldi(val_t inst);
    void ldr(val_t inst);
    void lea(val_t inst);
    void rti(val_t inst);
    void st(val_t inst);
    void sti(val_t inst);
    void str(val_t inst);
    void trap(val_t inst);

    void executeBr(val_t inst);
};

#endif // COMPUTER_H
